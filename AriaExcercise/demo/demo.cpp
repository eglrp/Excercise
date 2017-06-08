#include "Aria.h"


int main(int argc, char** argv)
{
  // ��ʼ������ȫ������
  Aria::init();

  // �����ArLog���"Verbose"�������Ϣʱȡ������ע��
  //ArLog::init(ArLog::StdOut, ArLog::Verbose);

  // �ö���������н�������ѡ��
  ArArgumentParser parser(&argc, argv);

  // ���������в�������ARIAARGS�м���Ĭ��ֵ��Linux����/etc/Aria.args
  parser.loadDefaultArguments();

  // ���Ķ������ڻ��������������������
  ArRobot robot;

  // ʹ��Ԥ��Ĳ������ӻ����˻�ģ����
  ArRobotConnector robotConnector(&parser, &robot);

  // �����������ģ�������ǣ���ʹ�ñ��������������������������û���Զ�ʹ�������Ǿ�������
  // ���������ArRobot��ȡ���ݲ�������̬
  ArAnalogGyro gyro(&robot);

  // ���ӻ����ˣ���ȡ��ʼ��Ϣ��Ϊ����ز����ļ�.
  if (!robotConnector.connectRobot())
  {
    // ����������״̬
    if (!parser.checkHelpAndWarnUnparsed())
    {
      ArLog::log(ArLog::Terse, "Could not connect to robot, will not have parameter file so options displayed later may not include everything");
    }
    // ��������
    else
    {
      ArLog::log(ArLog::Terse, "Error, could not connect to robot.");
      Aria::logOptions();
      Aria::exit(1);
    }
  }

  if(!robot.isConnected())
  {
    ArLog::log(ArLog::Terse, "Internal error: robot connector succeeded but ArRobot::isConnected() is false!");
  }

  // ���Ӽ���
  ArLaserConnector laserConnector(&parser, &robot, &robotConnector);

  // ����ָ����
  ArCompassConnector compassConnector(&parser);

  // ����������ѡ��
  if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
  {    
    Aria::logOptions();
    Aria::exit(1);
    return 1;
  }

  // ʹ�øö������Ӳ�������������
  ArSonarDevice sonarDev;
  
  // ʹ�øö�������������
  ArKeyHandler keyHandler;
  Aria::setKeyHandler(&keyHandler);

  // ����󶨵�������
  robot.attachKeyHandler(&keyHandler);
  printf("You may press escape to exit\n");

  // �����ɴ������
  robot.addRangeDevice(&sonarDev);

  
  // ��������������ѭ��
  robot.runAsync(true);

  // ���ӣ��������л�����ļ��������ü��⣩�����м��⴦�����
  if (!laserConnector.connectLasers(
        false,  // ����ʧ�ܺ��Ƿ�����
        false,  // ֻ�����ӵļ�����ӵ�ArRobot
        true    // ������м��⵽ArRobot
  ))
  {
    printf("Could not connect to lasers... exiting\n");
    Aria::exit(2);
  }

  // ����������ָ����
  ArTCM2 *compass = compassConnector.create(&robot);
  if(compass && !compass->blockingConnect()) {
    compass = NULL;
  }
  
  // ����1�룬�Ի�ȡ���Ի����˺�����ͷ�ĳ�ʼ��������Ϣ
  ArUtil::sleep(1000);

  // ������������ѭ��������ʱ�������Ҫ���ò�ͬģʽ����Ҫ��ס������
  robot.lock();

  // ���û����˲�ͬģʽ
  ArModeGripper gripper(&robot, "gripper", 'g', 'G');
  ArModeActs actsMode(&robot, "acts", 'a', 'A');
  ArModeTCM2 tcm2(&robot, "tcm2", 'm', 'M', compass);
  ArModeIO io(&robot, "io", 'i', 'I');
  ArModeConfig cfg(&robot, "report robot config", 'o' , 'O');
  ArModeCommand command(&robot, "command", 'd', 'D');
  ArModeCamera camera(&robot, "camera", 'c', 'C');
  ArModePosition position(&robot, "position", 'p', 'P', &gyro);
  ArModeSonar sonar(&robot, "sonar", 's', 'S');
  ArModeBumps bumps(&robot, "bumps", 'b', 'B');
  ArModeLaser laser(&robot, "laser", 'l', 'L');
  ArModeWander wander(&robot, "wander", 'w', 'W');
  ArModeUnguardedTeleop unguardedTeleop(&robot, "unguarded teleop", 'u', 'U');
  ArModeTeleop teleop(&robot, "teleop", 't', 'T');


  // ����Ĭ��ģʽ
  teleop.activate();

  // ����������
  robot.comInt(ArCommands::ENABLE, 1);

  robot.unlock();
  
  // ��������ִֹ�����̣߳����ȴ������˵�����ѭ���߳��˳�������ͨ�������˶Ͽ����˳������»����ϵͳ�źţ�
  robot.waitForRunExit();

  Aria::exit(0);
  return 0;

}