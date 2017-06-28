/* �����ǵ�ʹ��ʾ�� */
#include "Aria.h"
#include "ArAnalogGyro.h"


class GyroTask
{
public:
  // ���캯�������ڹ���ʱ��ʼ����ص�����
  GyroTask(ArRobot *robot);
  // �յ���������
  ~GyroTask(void) {}
  
  // Ŀ��
  void doTask(void);
protected:
  //double myHeading;
  ArAnalogGyro *myGyro;
  ArRobot *myRobot;
  ArFunctorC<GyroTask> myTaskCB;		//�ص�����
};


// ���캯��
GyroTask::GyroTask(ArRobot *robot) :
  myTaskCB(this, &GyroTask::doTask)		//�󶨻ص�������ú���
{
	ArKeyHandler *keyHandler;
	myRobot = robot;
	// ���ص����������������ѭ�������У��ö����еĺ���ÿ�����ڶ������һ��
	myRobot->addUserTask("GyroTask", 50, &myTaskCB);		//��һ������ΪĿ�����ƣ��ڶ�������Ϊ�������ȼ�������������Ϊ���õĺ���ָ��
	myGyro = new ArAnalogGyro(myRobot);
	//�󶨼��̲������
	if ((keyHandler = Aria::getKeyHandler()) == NULL)
	{
		keyHandler = new ArKeyHandler;
		Aria::setKeyHandler(keyHandler);
		if (myRobot != NULL)
			myRobot->attachKeyHandler(keyHandler);
		else
			ArLog::log(ArLog::Terse, "GyroTask: No robot to attach a keyHandler to, keyHandling won't work... either make your own keyHandler and drive it yourself, make a keyhandler and attach it to a robot, or give this a robot to attach to.");
	}  
	keyHandler->addKeyHandler('1', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, 10));
	keyHandler->addKeyHandler('2', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, 20));
	keyHandler->addKeyHandler('3', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, 30));
	keyHandler->addKeyHandler('4', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, 40));
	keyHandler->addKeyHandler('5', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, 50));
	keyHandler->addKeyHandler('6', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, 60));
	keyHandler->addKeyHandler('7', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, 70));
	keyHandler->addKeyHandler('8', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, 80));
	keyHandler->addKeyHandler('9', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, 90));
	keyHandler->addKeyHandler('0', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, 100));

	keyHandler->addKeyHandler('q', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, -10));
	keyHandler->addKeyHandler('w', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, -20));
	keyHandler->addKeyHandler('e', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, -30));
	keyHandler->addKeyHandler('r', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, -40));
	keyHandler->addKeyHandler('t', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, -50));
	keyHandler->addKeyHandler('y', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, -60));
	keyHandler->addKeyHandler('u', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, -70));
	keyHandler->addKeyHandler('i', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, -80));
	keyHandler->addKeyHandler('o', new ArFunctor1C<ArRobot, double>(myRobot,&ArRobot::setRotVel, -90));
	keyHandler->addKeyHandler('p', new ArFunctor1C<ArRobot, double>(myRobot, &ArRobot::setRotVel, -100));

	keyHandler->addKeyHandler('a', new ArFunctor1C<ArRobot, double>(myRobot, &ArRobot::setHeading, 0));
	keyHandler->addKeyHandler('s', new ArFunctor1C<ArRobot, double>(myRobot, &ArRobot::setHeading, 90));
	keyHandler->addKeyHandler('d', new ArFunctor1C<ArRobot, double>(myRobot, &ArRobot::setHeading, 180));
	keyHandler->addKeyHandler('f', new ArFunctor1C<ArRobot, double>(myRobot, &ArRobot::setHeading, 270));

  }

void GyroTask::doTask(void)
{
  /*
  double degrees = -((myRobot->getAnalog() * 5.0 / 255) - 2.509) * 150 / 2.5 * 1.265;
  if (fabs(degrees) < 2)
    degrees = 0;
  myHeading += degrees * .025;
  printf("%10f %10f %10f %10f\n", myRobot->getAnalog() * 5.0 / 255, degrees,
     myRobot->getRotVel(), myHeading);
  fflush(stdout);
  */
  printf("gyro th (mode 1 only):%8.4f  encoder th:%8.4f   ArRobot mixed th:%8.4f  temp:%d  ave:%g\n", myGyro->getHeading(), myRobot->getRawEncoderPose().getTh(), myRobot->getTh(), myGyro->getTemperature(), myGyro->getAverage());
}



int main(int argc, char **argv)
{
  Aria::init();
  ArRobot robot;
  
  // ң����������
  ArActionJoydrive joydriveAct;
  // ������������
  ArActionKeydrive keydriveAct;

  GyroTask gyro(&robot);

  // ���ɣ�������������ײ����
  ArSonarDevice sonar;

  ArSimpleConnector connector(&argc, argv);
  if (!connector.parseArgs() || argc > 1)
  {
    connector.logOptions();
    Aria::exit(1);
    return 1;
  }
  

  printf("This program will allow you to use a joystick or keyboard to control the robot.\nYou can use the arrow keys to drive, and the spacebar to stop.\nFor joystick control press the trigger button and then drive.\nPress escape to exit.\n");


  if (!joydriveAct.joystickInited())
    printf("Do not have a joystick, only the arrow keys on the keyboard will work.\n");
  
  // ����ң��״̬��������û�б�����ʱ�����κβ���
  joydriveAct.setStopIfNoButtonPressed(false);

  // �������
  robot.addRangeDevice(&sonar);

  if (!connector.connectRobot(&robot))
  {
    printf("Could not connect to robot... exiting\n");
    Aria::exit(1);
    return 1;
  }

  robot.comInt(ArCommands::ENABLE, 1);		//����������

  robot.addAction(&joydriveAct, 50);
  robot.addAction(&keydriveAct, 45);

  // ����ң��״̬��������û�б�����ʱ�����κβ������Ӷ������������Բ���������
  // set the joydrive action so it'll let the keydrive action fire if
  // there is no button pressed
  joydriveAct.setStopIfNoButtonPressed(false);

  
  // ���������ˣ�true��ʾ�����ӶϿ�ʱ��������
  robot.run(true);
  
  // ��������
  Aria::exit(0);
  return 0;
}