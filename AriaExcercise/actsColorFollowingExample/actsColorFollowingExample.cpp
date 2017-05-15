#include "Aria.h"



// Chase�ǽ��������ƶ�����ǰ��Ұ��������Ķ���
class Chase : public ArAction
{
  
public:
  
  // chase������״̬
  enum State {
    NO_TARGET,      // ��Ұ����Ŀ��
    TARGET,         // ��Ұ����Ŀ��
  };

  // ���캯��
  Chase(ArACTS_1_2 *acts, ArVCC4 *camera);
  
  // ��������
  ~Chase(void);
  
  // ����
  ArActionDesired *fire(ArActionDesired currentDesired);

  // �����뵽��ȡ�Ķ����ACTSͨ��
  bool setChannel(int channel);

  // ���ض�����ǰ״̬
  State getState(void) { return myState; }

  // ץ֡��ץȡͼ����ܿ��
  enum {
    WIDTH = 160,
    HEIGHT = 120
  };

protected:
  ArActionDesired myDesired;
  ArACTS_1_2 *myActs;		//��ACTSͨ����
  ArVCC4 *myCamera;			//���Ƽ���VC-C4��VC-C50i�����ƽ�ƣ���б�����Ż���
  ArTime myLastSeen;		//����ʱ������Ͳ�������ʱ�����
  State myState;
  int myChannel;
  int myMaxTime;
};


  // ���캯��
Chase::Chase(ArACTS_1_2 *acts, ArVCC4 *camera) :
    ArAction("Chase", "Chases the largest blob.")
{
  myActs = acts;
  myCamera = camera;
  myChannel = 0;
  myState = NO_TARGET;
  setChannel(1);
  myLastSeen.setToNow();		//����ʱ��
  myMaxTime = 1000;
}

  // ��������
Chase::~Chase(void) {}


// chase����
ArActionDesired *Chase::fire(ArActionDesired currentDesired)
{
  ArACTSBlob blob;				//ACTS������
  ArACTSBlob largestBlob;

  bool flag = false;			//�ж��Ƿ���������

  int numberOfBlobs;
  int blobArea = 10;			//�Ա��������������ҳ����Ķ���

  double xRel, yRel;

  // ���ö�������
  myDesired.reset();
  
  numberOfBlobs = myActs->getNumBlobs(myChannel);		//�Ӹ���ͨ����ȡ��������

  // ���̽�⵽�˶��󣬼�¼��ǰʱ��
  if(numberOfBlobs != 0)
  {
    for(int i = 0; i < numberOfBlobs; i++)
    {
      myActs->getBlob(myChannel, i + 1, &blob);			//��ȡ̽�⵽�Ķ������Ϣ
	  //�жϸö�����������Ƿ���ڶԱ���
      if(blob.getArea() > blobArea)
      {
		  //���¶Ա���
		  flag = true;
		  blobArea = blob.getArea();
		  largestBlob = blob;
      }
    }
	//����ʱ��
    myLastSeen.setToNow();
  }

  // �ж��Ƿ����޶�ʱ���ڷ����˶���
  if (myLastSeen.mSecSince() > myMaxTime)
  {
    if(myState != NO_TARGET) ArLog::log(ArLog::Normal, "Target Lost");
    myState = NO_TARGET;
  }
  else
  {
    // ����ǵ�һ�η��ֶ��������־
    if(myState != TARGET) {
      ArLog::log(ArLog::Normal, "Target Aquired");
      ArLog::log(ArLog::Normal, "(Using channel %d with %d blobs)", myChannel, numberOfBlobs);
    }
	//����״̬����ʾ�Ѿ��ҵ�������
    myState = TARGET;
  }

  if(myState == TARGET && flag == true)
  { 
    // ȷ�����ߵ����������������ĵ�λ��
    xRel = (double)(largestBlob.getXCG() - WIDTH/2.0)  / (double)WIDTH;
    yRel = (double)(largestBlob.getYCG() - HEIGHT/2.0) / (double)HEIGHT;
      
    // ��������ͷָ�����
    if(!(ArMath::fabs(yRel) < .20))
    {
      if (-yRel > 0)
        myCamera->tiltRel(1);
      else
        myCamera->tiltRel(-1);
    }

    // ���û����˵ĺ����ٶ�
    if (ArMath::fabs(xRel) < .10)
    {
      myDesired.setDeltaHeading(0);
    }
    else
    {
      if (ArMath::fabs(-xRel * 10) <= 10)
        myDesired.setDeltaHeading(-xRel * 10);
      else if (-xRel > 0)
        myDesired.setDeltaHeading(10);
      else
        myDesired.setDeltaHeading(-10);
     
    }

    myDesired.setVel(200);
    return &myDesired;    
  }

  // ���û��Ŀ���򲻽��ж���
  return &myDesired;
}

// ���ö�����Ϣ��Դ��ͨ��
bool Chase::setChannel(int channel)
{
  if (channel >= 1 && channel <= ArACTS_1_2::NUM_CHANNELS)
  {
    myChannel = channel;
    return true;
  }
  else
    return false;
}




// �ص�����/���ü�����������
void toggleAction(ArAction* action)
{
  if(action->isActive()) {
    action->deactivate();
    ArLog::log(ArLog::Normal, "%s action is now deactivated.", action->getName());
  }
  else {
    action->activate();
    ArLog::log(ArLog::Normal, "%s action is now activated.", action->getName());
  }
}



int main(int argc, char** argv)
{
  Aria::init();

  ArRobot robot;

  // ���ռ��̲�������
  ArKeyHandler keyHandler;
  Aria::setKeyHandler(&keyHandler);		//ע��

  ArSonarDevice sonar;

  // ��� (Cannon VC-C4)
  ArVCC4 vcc4 (&robot);

  // ACTS, ���ڸ��ٰߵ����ɫ
  ArACTS_1_2 acts;

  // �����в���
  ArArgumentParser argParser(&argc, argv);
  argParser.loadDefaultArguments();
  
  // ��ͳ�������뼤��������
  ArSimpleConnector simpleConnector(&argParser);

  // ��������             
  if (!Aria::parseArgs())
  {    
    Aria::logOptions();
    keyHandler.restore();
    Aria::exit(1);
    return 1;
  }

  // ���������ƶ�������������̽�⵽���ϰ��
  ArActionLimiterForwards limiter("speed limiter near", 350, 800, 200);
  ArActionLimiterForwards limiterFar("speed limiter far", 400, 1250, 300);
  ArActionLimiterBackwards backwardsLimiter;
  ArActionConstantVelocity stop("stop", 0);
  //ArActionConstantVelocity backup("backup", -200);
  

  // ��ɫ���涯������
  Chase chase(&acts, &vcc4);

  // ������������
  ArActionKeydrive keydriveAction;

  // ʹ��a�����л���������
  keyHandler.addKeyHandler('a', new ArGlobalFunctor1<ArAction*>(&toggleAction, &keydriveAction));		//����aʱ����keydriveActionΪ��������toggleAction����

  // ע�����
  Aria::setKeyHandler(&keyHandler);

  // ������������˰�
  robot.attachKeyHandler(&keyHandler);

  // Ϊ�������������
  robot.addRangeDevice(&sonar);

  // ���ӻ�����
  if (!simpleConnector.connectRobot(&robot))
  {
    ArLog::log(ArLog::Terse, "Error: Could not connect to robot... exiting\n");
    keyHandler.restore();
    Aria::exit(1);
  }

  // ����ACTS������
  if(!acts.openPort(&robot)) 
  {
    ArLog::log(ArLog::Terse, "Error: Could not connect to ACTS... exiting.");
    keyHandler.restore();
    Aria::exit(2);
  }

  // ��ʼ�����
  vcc4.init();

  // �ȴ�1��
  ArUtil::sleep(1000);

  // ��ֹ�������ٶȹ���
  robot.setAbsoluteMaxTransVel(400);

  // ����������
  robot.comInt(ArCommands::ENABLE, 1);
  
  // �ر�amigobot����
  robot.comInt(ArCommands::SOUNDTOG, 0);

  // Wait....
  ArUtil::sleep(200);

  // �����ȶȽ�����Ӷ���
  robot.addAction(&limiter, 7);
  robot.addAction(&limiterFar, 6);
  robot.addAction(&backwardsLimiter, 5);
  robot.addAction(&keydriveAction, 4);
  robot.addAction(&chase, 3);
  robot.addAction(&stop, 1);

  // ͣ�ü�������
  keydriveAction.deactivate();

  // ����������
  ArLog::log(ArLog::Normal, "Running. Train ACTS to detect a color to drive towards an object, or use 'a' key to switch to keyboard driving mode.");
  robot.run(true);
  
  Aria::exit(0);
  return 0;
}