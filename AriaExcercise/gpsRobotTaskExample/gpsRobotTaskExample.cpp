/* ���ӻ����˺�GPS������Զ�̲���������ӡ������λ�ú�GPS���ݡ� */
#include "Aria.h"
#include "ArGPS.h"
#include "ArGPSConnector.h"

#include <assert.h>


/*  
 *  �����װ��һ��ArRobot�������������������ñ�׼�����ʽ���������ʱ����͵�ǰ��������̬�����µ�GPS����
 *  ����Ҳ����һ������������������������GPS��������ArGPS����ʱ��
 */
class GPSLogTask {

public:
  GPSLogTask(ArRobot *robot, ArGPS *gps, ArJoyHandler *joy = NULL) :
      myRobot(robot), 
      myGPS(gps),
      myTaskFunctor(this, &GPSLogTask::doTask),
      myJoyHandler(joy),
      myButtonDown(false)
  {
	  //��ͬ������Ĵ������岹�������������
    myRobot->addSensorInterpTask("GPS", ArListPos::LAST, &myTaskFunctor); 
	//��һ���ݲ��������ʲô����
    puts("RobotX\tRobotY\tRobotTh\tRobotVel\tRobotRotVel\tRobotLatVel\tLatitude\tLongitude\tAltitude\tSpeed\tGPSTimeSec\tGPSTimeMSec\tFixType\tNumSats\tPDOP\tHDOP\tVDOP\tGPSDataReceived");
  }

  void lock() { myMutex.lock(); }
  void unlock() { myMutex.unlock(); }

protected:

  void doTask()
  {
    // ���ҡ�˰�ť�����£����һ����ǣ�����1����������������
    if(myJoyHandler)
    {
      for(unsigned int b = 2; b <= myJoyHandler->getNumButtons(); ++b)
        if(myJoyHandler->getButton(b)) {
          if(!myButtonDown)
            printf("--------------- Joystick button %d pressed.\n", b);
          myButtonDown = true;
        }
        else
          myButtonDown = false;
    }

    lock();
	//�ӻ����˶�ȡ���ݣ�����ʱ��Ϊ50ms��Ϊ0ʱ��ȡ���������ٷ���
    int f = myGPS->read(50);
    printf("%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f"
           "\t%2.8f\t%2.8f\t%4.4f\t%4.4f"
           "\t%lu\t%lu\t%s"
           "\t%u\t%2.4f\t%2.4f\t%2.4f"
           "\t%s\n",
      myRobot->getX(), myRobot->getY(), myRobot->getTh(), myRobot->getVel(), myRobot->getRotVel(), (myRobot->hasLatVel())?(myRobot->getLatVel()):0,
      myGPS->getLatitude(), myGPS->getLongitude(), myGPS->getAltitude(), myGPS->getSpeed(),
      myGPS->getGPSPositionTimestamp().getSec(), myGPS->getGPSPositionTimestamp().getMSec(), myGPS->getFixTypeName(),
      myGPS->getNumSatellitesTracked(), myGPS->getPDOP(), myGPS->getHDOP(), myGPS->getVDOP(),
      ((f&ArGPS::ReadUpdated)?"yes":"no")
    );
    unlock();
  }

private:
  ArRobot *myRobot;
  ArGPS *myGPS;
  ArFunctorC<GPSLogTask> myTaskFunctor;
  ArMutex myMutex;
  ArJoyHandler *myJoyHandler;
  bool myButtonDown;
};



int main(int argc, char** argv)
{
  Aria::init();
  ArLog::init(ArLog::StdErr, ArLog::Normal);

  ArArgumentParser argParser(&argc, argv);
  ArSimpleConnector connector(&argParser);
  ArGPSConnector gpsConnector(&argParser);
  ArRobot robot;

  ArActionLimiterForwards nearLimitAction("limit near", 300, 600, 250);
  ArActionLimiterForwards farLimitAction("limit far", 300, 1100, 400);
  ArActionLimiterBackwards limitBackwardsAction;
  ArActionJoydrive joydriveAction;
  ArActionKeydrive keydriveAction;

  ArSonarDevice sonar;
  ArSick laser;

  argParser.loadDefaultArguments();
  if(!Aria::parseArgs() || !argParser.checkHelpAndWarnUnparsed())
  {
    Aria::logOptions();
    return -1;
  }

  robot.addRangeDevice(&sonar);
  robot.addRangeDevice(&laser);

  ArLog::log(ArLog::Normal, "gpsRobotTaskExample: Connecting to robot...");
  if(!connector.connectRobot(&robot))
  {
    ArLog::log(ArLog::Terse, "gpsRobotTaskExample: Could not connect to the robot. Exiting.");
    return -2;
  }
  ArLog::log(ArLog::Normal, "gpsRobotTaskExample: Connected to the robot.");


  // ���ӻ�����
  ArLog::log(ArLog::Normal, "gpsRobotTaskExample: Connecting to GPS, it may take a few seconds...");
  ArGPS *gps = gpsConnector.createGPS(&robot);
  assert(gps);		//�˴����ò���
  if(!gps || !(gps->connect()))
  {
    ArLog::log(ArLog::Terse, "gpsRobotTaskExample: Error connecting to GPS device.  Try -gpsType, -gpsPort, and/or -gpsBaud command-line arguments. Use -help for help. Exiting.");
    return -3;
  }


  // ����һ��GPSLogTask��
  GPSLogTask gpsTask(&robot, gps, joydriveAction.getJoyHandler()->haveJoystick() ? joydriveAction.getJoyHandler() : NULL);


  // ��Ӷ���
  robot.addAction(&nearLimitAction, 100);
  robot.addAction(&farLimitAction, 90);
  robot.addAction(&limitBackwardsAction, 80);
  robot.addAction(&joydriveAction, 50);
  robot.addAction(&keydriveAction, 40);

  // ���ҡ��δ������ʱ��������̲���
  joydriveAction.setStopIfNoButtonPressed(false);

  // ����������
  robot.runAsync(true);

  // ���Ӽ���
//  connector.setupLaser(&laser);
  //laser.runAsync();
 // if(!laser.blockingConnect())
  //  ArLog::log(ArLog::Normal, "gpsRobotTaskExample: Warning, could not connect to SICK laser, will not use it.");

  robot.lock();

  robot.enableMotors();
  robot.comInt(47, 1);  // ����ҡ������

  // ����˳��ص�������/���seekur�ϵķ����̣����������û�л�������Ϊ�ؼ���; ������������û���κ�����
  Aria::addExitCallback(new ArRetFunctor1C<bool, ArRobot, unsigned char>(&robot, &ArRobot::com, (unsigned char)120));
  Aria::addExitCallback(new ArRetFunctor1C<bool, ArRobot, unsigned char>(&robot, &ArRobot::com, (unsigned char)120));

  robot.unlock();

  ArLog::log(ArLog::Normal, "gpsRobotTaskExample: Running... (drive robot with joystick or arrow keys)");
  robot.waitForRunExit();


  return 0;
}