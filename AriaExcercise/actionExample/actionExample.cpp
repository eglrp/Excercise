#include "Aria.h"
/* ��ʵ����ʾ�˶����Ĵ�����ʹ�� */

/* ��������ǰֱ��ֱ������̽�⵽���ϰ���Ϊֹ */
class ActionGo : public ArAction
{
public:
	// ���캯�������� myMaxSpeed �� myStopDistance
	ActionGo(double maxSpeed, double stopDistance);
	// �����������޲���
	virtual ~ActionGo(void) {};
	// �������������øú�������ȡ�ö���Ҫ�����Ϊ
	virtual ArActionDesired *fire(ArActionDesired currentDesired);
	// �洢������ָ�������ArSonarDevice�������û�����ɣ�ֹͣ����
	virtual void setRobot(ArRobot *robot);
protected:
	// ʹ��setRobot()�ӻ������л�ȡ�������ɶ���
	ArRangeDevice *mySonar;


	/* ������������fire()�����޸Ĳ����ö����ָ�봫�ݸ�����������
	�������Ӧ�����г�Ա��������ʹfire()��������Ҳ����
	����fire()ÿ�ε���ʱ���ᴴ��һ���ö��󣬵�ʼ�ղ���ɾ�������Ķ���
	*/
	ArActionDesired myDesired;

	double myMaxSpeed;			//����ٶ�
	double myStopDistance;		//ֹͣ����
};


/* ����������̽�⵽�ϰ���ʱת�� */
class ActionTurn : public ArAction
{
public:
	// ���캯��������turnThreshold, turnAmount
	ActionTurn(double turnThreshold, double turnAmount);
	// �����������޲���
	virtual ~ActionTurn(void) {};
	// �������������øú�������ȡ�ö���Ҫ�����Ϊ
	virtual ArActionDesired *fire(ArActionDesired currentDesired);
	// �洢������ָ�������ArSonarDevice�������û�����ɣ�ֹͣ����
	virtual void setRobot(ArRobot *robot);
protected:
	// ʹ��setRobot()�ӻ������л�ȡ�������ɶ���
	ArRangeDevice *mySonar;
	// ������������������������fire()��ʹ��
	ArActionDesired myDesired;
	// �����Զ��ʼת��
	double myTurnThreshold;
	// ת�Ǵ�С
	double myTurnAmount;
	// ת������
	int myTurning; // -1 == left, 1 == right, 0 == none
};

/*
���캯��
*/
ActionGo::ActionGo(double maxSpeed, double stopDistance) :
ArAction("Go")		//"Go"Ϊ�Ըö��������
{
	mySonar = NULL;
	myMaxSpeed = maxSpeed;
	myStopDistance = stopDistance;
	setNextArgument(ArArg("maximum speed", &myMaxSpeed, "Maximum speed to go."));		//���ò������ͣ�����Ϊ������������ָ�롢�������������ڳ����е����������ҵ��ö����Ĳ��������޸�
	setNextArgument(ArArg("stop distance", &myStopDistance, "Distance at which to stop."));
}

/*
��дArAction::setRobot()���ӻ������л�ȡ����װ�û�û������ʱֹͣ����
�������ArAction::setRobot()������ȷ�洢ArAction�����е�ArRobotָ�롣
*/
void ActionGo::setRobot(ArRobot *robot)
{
	ArAction::setRobot(robot);		//����������������˰�
	mySonar = robot->findRangeDevice("sonar");	//������Ϊ��sonar���ķ�Χװ��
	if (robot == NULL)
	{
		/*�����־��Ϣ
		��һ����������־�ȼ����м�ࡢ��������ϸ����
		�ڶ�����������־�������ݣ��ò��������ж��*/
		ArLog::log(ArLog::Terse, "actionExample: ActionGo: Warning: I found no sonar, deactivating.");
		deactivate();		//ͣ�ö���
	}
}

/*
fire�Ƕ����ĺ��ĵ�
currentDesired�Ƕ����������Ѿ������˵Ķ�����ϵ�����������������ע��˴���
��myDesired������������ǰ�ٶȲ����ظö���
myDesired����Ϊ���г�Ա����Ϊ�������ص���ָ��myDesired��ָ��
*/
ArActionDesired *ActionGo::fire(ArActionDesired currentDesired)
{
	double range;
	double speed;

	// ����myDesired�����֮ǰ������
	myDesired.reset();
	// ���û�����ɣ�ֹͣ����
	if (mySonar == NULL)
	{
		deactivate();
		return NULL;
	}
	// ��ȡ���ɵķ�Χ
	/*ʹ��currentReadingPolar��ȡ�Ƕȷ�Χ�����ɶ���
	myRobot��Araction���г�Ա�������ڵ��ù�setRobot()�����ǰ�����Ļ����ˡ�
	getRobotRadius()��ȡ�����˰뾶*/
	range = mySonar->currentReadingPolar(-70, 70) - myRobot->getRobotRadius();	
	// �����Χ�ڱ�ֹͣ����󣬼���ǰ��
	if (range > myStopDistance)
	{
		// ����һ�����ڷ�Χ�������ٶ�
		speed = range * .3;
		// ȷ���ٶȲ�����Ԥ���ٶ�
		if (speed > myMaxSpeed)
			speed = myMaxSpeed;
		// ���������ٶ�
		myDesired.setVel(speed);
	}
	// �����Χ�ڱ�ֹͣ����С��ֹͣǰ��
	else
	{
		myDesired.setVel(0);
	}
	// ����ָ��myDesired��ָ��
	return &myDesired;
}


/* ���캯�� */
ActionTurn::ActionTurn(double turnThreshold, double turnAmount) :
ArAction("Turn")
{
	myTurnThreshold = turnThreshold;
	myTurnAmount = turnAmount;
	setNextArgument(ArArg("turn threshold (mm)", &myTurnThreshold, "The number of mm away from obstacle to begin turnning."));
	setNextArgument(ArArg("turn amount (deg)", &myTurnAmount, "The number of degress to turn if turning."));	//���ò������ͣ�����Ϊ������������ָ�롢�������������ڳ����е����������ҵ��ö����Ĳ��������޸�
	myTurning = 0;
}

/*
��дArAction::setRobot()���ӻ������л�ȡ����װ�û�û������ʱֹͣ����
�������ArAction::setRobot()������ȷ�洢ArAction�����е�ArRobotָ�롣
*/
void ActionTurn::setRobot(ArRobot *robot)
{
	ArAction::setRobot(robot);
	mySonar = robot->findRangeDevice("sonar");
	if (mySonar == NULL)
	{
		/*�����־��Ϣ
		��һ����������־�ȼ����м�ࡢ��������ϸ����
		�ڶ�����������־�������ݣ��ò��������ж��*/
		ArLog::log(ArLog::Terse, "actionExample: ActionTurn: Warning: I found no sonar, deactivating.");
		deactivate(); 
	}
}

/*
fire�Ƕ����ĺ��ĵ�
currentDesired�Ƕ����������Ѿ������˵Ķ�����ϵ�����������������ע��˴���
��myDesired������������ǰ�ٶȲ����ظö���
myDesired����Ϊ���г�Ա����Ϊ�������ص���ָ��myDesired��ָ��
*/
ArActionDesired *ActionTurn::fire(ArActionDesired currentDesired)
{
	double leftRange, rightRange;
	// ����myDesired�����֮ǰ������
	myDesired.reset();
	// ���û�����ɣ�ֹͣ����
	if (mySonar == NULL)
	{
		deactivate();
		return NULL;
	}
	// ��ȡ������������̽�ⷶΧ
	leftRange = (mySonar->currentReadingPolar(0, 100) - 
		myRobot->getRobotRadius());
	rightRange = (mySonar->currentReadingPolar(-100, 0) - 
		myRobot->getRobotRadius());
	// ������ҷ�Χ��С����ֵ������ת�������ֹͣת��
	if (leftRange > myTurnThreshold && rightRange > myTurnThreshold)
	{
		myTurning = 0;
		myDesired.setDeltaHeading(0);		//����ת������Ϊ0
	}
	// ����Ѿ���ת���У��������÷���ת��
	else if (myTurning)
	{
		myDesired.setDeltaHeading(myTurnAmount * myTurning);
	}
	// �����δת�򣬵���Ҫת��������������ת���Ҳ�
	// �޸�ת�����myTurning�Ա�֤�������÷���ת��
	else if (leftRange < rightRange)
	{
		myTurning = -1;
		myDesired.setDeltaHeading(myTurnAmount * myTurning);
	}
	// �����δת�򣬵���Ҫת�����Ҳ��������ת���Ҳ�
	// �޸�ת�����myTurning�Ա�֤�������÷���ת��
	else 
	{
		myTurning = 1;
		myDesired.setDeltaHeading(myTurnAmount * myTurning);
	}
	// ����ָ��myDesired��ָ��
	return &myDesired;
}



int main(int argc, char** argv)
{
	Aria::init();		//ȫ�ֳ�ʼ��

	ArSimpleConnector conn(&argc, argv);		//��ͳ�����˺ͼ��������������Ƽ�ʹ��
	ArRobot robot;
	ArSonarDevice sonar;				//���Ի����˵�������ɶ���

	ActionGo go(500, 350);
	ActionTurn turn(400, 10);
	ArActionStallRecover recover;		//ʧ�ٻָ�����


	// �������������в���
	if(!Aria::parseArgs())
	{
		//�����������������������־����¼��ǰ�������в���
		Aria::logOptions();
		return 1;
	}

	// ���ӻ�����
	if(!conn.connectRobot(&robot))
	{
		ArLog::log(ArLog::Terse, "actionExample: Could not connect to robot! Exiting.");
		return 2;
	}

	// Ϊ��������ӷ�Χװ�ã���һ����������Ӷ���֮ǰ
	robot.addRangeDevice(&sonar);


	// ������Ӷ���
	robot.addAction(&recover, 100);	//����������Ӷ�����ǰ��Ϊ�����࣬����Ϊ�������ȼ�
	robot.addAction(&go, 50);
	robot.addAction(&turn, 49);

	//����������
	robot.enableMotors();

	// ��ʼ�����˽���ѭ��
	// 'true' ��Ϊ���Ͽ�����ʱֹͣ����
	robot.run(true);

	Aria::exit(0);
}