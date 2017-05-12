#include "Aria.h"

/* ��ʵ����ʾ�˶�����Ĵ�����ʹ�� */

ArActionGroup *teleop;		//ң�ض�����
ArActionGroup *wander;		//����������

//����ң��ģʽ������ģʽȫ��ֹͣ
void teleopMode(void)
{
  teleop->activateExclusive();
  printf("\n== Teleoperation Mode ==\n");
  printf("   Use the arrow keys to drive, and the spacebar to stop.\n    For joystick control hold the trigger button.\n    Press 'w' to switch to wander mode.\n    Press escape to exit.\n");
}

//��������ģʽ������ģʽȫ��ֹͣ
void wanderMode(void)
{
  wander->activateExclusive();
  printf("\n== Wander Mode ==\n");
  printf("    The robot will now just wander around avoiding things.\n    Press 't' to switch to  teleop mode.\n    Press escape to exit.\n");
}


int main(int argc, char** argv)
{
  Aria::init();		//��ʼ��
  ArArgumentParser argParser(&argc, argv);		//������
  ArSimpleConnector con(&argParser);			//��ͳ�������뼤�������࣬���Ƽ�ʹ��
  ArRobot robot;
  ArSonarDevice sonar;

  argParser.loadDefaultArguments();				//����Ĭ�ϻ�������
  //�ж��Ƿ�ɹ������������ �� �������ַ����;���δ��������
  if(!Aria::parseArgs() || !argParser.checkHelpAndWarnUnparsed())
  {
    Aria::logOptions();			//��¼��������ѡ��
    return 1;
  }

  /* ң�ض����� */
  teleop = new ArActionGroup(&robot);

  // �������ӹ��� (�����������IR���Ӵ�����)
  teleop->addAction(new ArActionLimiterTableSensor, 100);

  // ����ײ��ǰ���ڽ����ϰ���
  teleop->addAction(new ArActionLimiterForwards("speed limiter near", 
                        300, 600, 250), 95);

  // ����ײ��ǰ��Զ�����ϰ���
  teleop->addAction(new ArActionLimiterForwards("speed limiter far", 
                           300, 1100, 400), 90);

  // ����ײ���󷽵��ϰ���
  teleop->addAction(new ArActionLimiterBackwards, 85);

  // ҡ�˶���
  ArActionJoydrive joydriveAct("joydrive", 400, 15);
  teleop->addAction(&joydriveAct, 50);

  // ���̶���
  teleop->addAction(new ArActionKeydrive, 45);
  


  /* ����ģʽ */
  wander = new ArActionGroup(&robot);

  // ʧ�ٻָ�����
  wander->addAction(new ArActionStallRecover, 100);

  // ��ײ������
  wander->addAction(new ArActionBumpers, 75);

  // ����ϰ��﹦�ܣ�������������ϰ������Ϊ225��ֹͣ�˶�
  wander->addAction(new ArActionAvoidFront("Avoid Front Near", 225, 0), 50);

  // ���ٶ���ϰ��﹦��
  wander->addAction(new ArActionAvoidFront, 45);

  // �Ժ㶨�ٶ�����
  wander->addAction(new ArActionConstantVelocity("Constant Velocity", 400), 25);

  

  /* ʹ�ü��������л�ģʽ */

  // ���Ariaû��Ԥ���������д������̰��������������˰�
  ArKeyHandler *keyHandler = Aria::getKeyHandler();
  if (keyHandler == NULL)
  {
    keyHandler = new ArKeyHandler;
    Aria::setKeyHandler(keyHandler);
    robot.attachKeyHandler(keyHandler);
  }

  // ���ûص�����
  ArGlobalFunctor teleopCB(&teleopMode);
  ArGlobalFunctor wanderCB(&wanderMode);
  keyHandler->addKeyHandler('w', &wanderCB);
  keyHandler->addKeyHandler('W', &wanderCB);
  keyHandler->addKeyHandler('t', &teleopCB);
  keyHandler->addKeyHandler('T', &teleopCB);

  // ���ҡ���Ƿ����
  if (!joydriveAct.joystickInited())
    printf("Note: Do not have a joystick, only the arrow keys on the keyboard will work.\n");
  
  // ����ҡ�˲�ʹ��ʱ��ͣ����
  joydriveAct.setStopIfNoButtonPressed(false);


  /* ���ӻ����ˣ���ʼ��ң��ģʽ����  */

  robot.addRangeDevice(&sonar);
  if(!con.connectRobot(&robot))
  { 
    ArLog::log(ArLog::Terse, "actionGroupExample: Could not connect to the robot.");
    Aria::exit(1);
  }

  robot.enableMotors();
  teleopMode();
  robot.run(true);

  Aria::exit(0);
}