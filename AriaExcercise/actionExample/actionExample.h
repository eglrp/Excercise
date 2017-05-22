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
