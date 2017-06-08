/* ʹ��ArActionGoto�࣬ͨ������һ��2.5*2.5m�������εĶ���Ϊһ��ArActionGoto������Ŀ���������������ڷ����������˶�
   ��Ҳ���������ٶ���������ײ��һ��ʱ�������ȡ��Ŀ�꣨���һ���������ֹͣ������ֹͣ�����˳��� */
#include "Aria.h"

int main(int argc, char **argv)
{
  Aria::init();
  ArArgumentParser parser(&argc, argv);
  parser.loadDefaultArguments();
  ArSimpleConnector simpleConnector(&parser);
  ArRobot robot;
  ArSonarDevice sonar;
  ArAnalogGyro gyro(&robot);	//���������ǣ����ڸ��ƻ�������̬
  robot.addRangeDevice(&sonar);

  // ������ť�����ʹ��ESC����Ѹ�ٹرճ���
  ArKeyHandler keyHandler;
  Aria::setKeyHandler(&keyHandler);
  robot.attachKeyHandler(&keyHandler);
  printf("You may press escape to exit\n");

  // ����ײ
  ArActionLimiterForwards limiterAction("speed limiter near", 300, 600, 250);
  ArActionLimiterForwards limiterFarAction("speed limiter far", 300, 1100, 400);
  ArActionLimiterTableSensor tableLimiterAction;
  robot.addAction(&tableLimiterAction, 100);
  robot.addAction(&limiterAction, 95);
  robot.addAction(&limiterFarAction, 90);

  // Goto������Ϊ�����ȼ�
  ArActionGoto gotoPoseAction("goto");
  robot.addAction(&gotoPoseAction, 50);
  
  // ֹͣ������Ϊ�����ȼ�����û��Ŀ��ʱ���ϻ�����
  ArActionStop stopAction("stop");
  robot.addAction(&stopAction, 40);

  // �������������в���
  if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
  {    
    Aria::logOptions();
    Aria::exit(1);
    return 1;
  }
  
  // ���ӻ�����
  if (!simpleConnector.connectRobot(&robot))
  {
    printf("Could not connect to robot... exiting\n");
    Aria::exit(1);
    return 1;
  }
  robot.runAsync(true);

  // ���������˷��������ر�amigobot����
  robot.enableMotors();
  robot.comInt(ArCommands::SOUNDTOG, 0);

  const int duration = 30000; //�����ƶ���ʱ
  ArLog::log(ArLog::Normal, "Going to four goals in turn for %d seconds, then cancelling goal and exiting.", duration/1000);

  bool first = true;
  int goalNum = 0;
  ArTime start;
  start.setToNow();
  while (Aria::getRunning()) 
  {
    robot.lock();

    // ����ǵ�һ��ѭ����ʼ��ѡ��һ����Ŀ�꣬�������������Ŀ��
    if (first || gotoPoseAction.haveAchievedGoal())
    {
      first = false;
      goalNum++;
      if (goalNum > 4)
        goalNum = 1; // ��ֹĿ����� #1

      // Ϊ��ͬĿ������λ��
      if (goalNum == 1)
        gotoPoseAction.setGoal(ArPose(2500, 0));
      else if (goalNum == 2)
        gotoPoseAction.setGoal(ArPose(2500, 2500));
      else if (goalNum == 3)
        gotoPoseAction.setGoal(ArPose(0, 2500));
      else if (goalNum == 4)
        gotoPoseAction.setGoal(ArPose(0, 0));

      ArLog::log(ArLog::Normal, "Going to next goal at %.0f %.0f", 
            gotoPoseAction.getGoal().getX(), gotoPoseAction.getGoal().getY());
    }

    if(start.mSecSince() >= duration) {
      ArLog::log(ArLog::Normal, "%d seconds have elapsed. Cancelling current goal, waiting 3 seconds, and exiting.", duration/1000);
      gotoPoseAction.cancelGoal();
      robot.unlock();
      ArUtil::sleep(3000);
      break;
    }

    robot.unlock();
    ArUtil::sleep(10000);
  }
  
  Aria::exit(0);
  return 0;
}