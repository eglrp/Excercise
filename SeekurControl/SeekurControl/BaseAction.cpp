/******************************************************************* 
*  �ļ�����:����������ʵ���ļ�
*  ��Ҫ����: Seekur��������ػ������������ʵ���ļ�
*   
*  ��������: 2017/09/6
*  ����: ���
*  ˵��: ��
*   
*  �޸�����: 
*  ����: 
*  ˵��: 
******************************************************************/ 

#include "BaseAction.h"
#include <iostream>
using namespace std;

#pragma region ������
ConnHandler::ConnHandler(ArRobot *robot) :
myConnectedCB(this, &ConnHandler::connected),  
	myConnFailCB(this, &ConnHandler::connFail),
	myDisconnectedCB(this, &ConnHandler::disconnected)

{
	myRobot = robot;
	myRobot->addConnectCB(&myConnectedCB, ArListPos::FIRST);
	myRobot->addFailedConnectCB(&myConnFailCB, ArListPos::FIRST);
	myRobot->addDisconnectNormallyCB(&myDisconnectedCB, ArListPos::FIRST);
	myRobot->addDisconnectOnErrorCB(&myDisconnectedCB, ArListPos::FIRST);
}

// ����ʧ��ʱ����
void ConnHandler::connFail(void)
{
	printf("����ʧ�ܣ�\n");
	myRobot->stopRunning();
	Aria::exit(1);
	return;
}

// �������������ر����ɺ�amigobot
void ConnHandler::connected(void)
{
	printf("���ӳɹ���\n");
	myRobot->comInt(ArCommands::SONAR, 0);
	myRobot->comInt(ArCommands::ENABLE, 1);
	myRobot->comInt(ArCommands::SOUNDTOG, 0);
}

// ���ӶϿ�ʱ��������
void ConnHandler::disconnected(void)
{
	printf("�Ͽ����ӣ���������\n");
	Aria::exit(0);
}
#pragma endregion

#pragma region ����������
BaseAction::BaseAction(ArRobot *robot){
	myRobot=robot;
}
/*
	��������������
	��������
	����ֵ����
	*/
	BaseAction::~BaseAction(void){

	}
/*
������ˮƽ�ƶ�ĳһ����
������
distacne���ƶ����룬��λΪmm��������ǰ�˶�����������˶�
����ֵ����
*/
void BaseAction::Move(double distacne){
	myRobot->lock();
	printf("�ƶ� %.2f mm\n",distacne);
	myRobot->move(distacne);
	myRobot->unlock();
}

/*
�����������ٶ�
������
velocity���ٶȣ���λΪmm/s
����ֵ����
*/
void BaseAction::SetVelocity(double velocity){
	myRobot->lock();
	printf("�ٶ�����Ϊ %.2f mm/s\n",velocity);
	myRobot->setVel(velocity);
	myRobot->unlock();
}
/*
���������������ٶ�
������
leftVelocity�����ٶȣ���λΪmm/s
rightVelocity�����ٶȣ���λΪmm/s
����ֵ����
*/
void BaseAction::SetVelocity(double leftVelocity,double rightVelocity){
	myRobot->lock();
	printf("���ٶ�����Ϊ %.2f mm/s\t���ٶ�����Ϊ %.2f mm/s",leftVelocity,rightVelocity);
	myRobot->setVel2(leftVelocity,rightVelocity);
	myRobot->unlock();
}
/*
�������������ֹͣ�˶�
��������
����ֵ����
*/
void BaseAction::Stop(){
	myRobot->lock();
	printf("������ֹͣ�˶�\n");
	myRobot->stop();
	myRobot->unlock();
}
/*
�����������˶��Ƿ��Ѿ�����
��������
����ֵ��true��ʾ�Ѿ�������false��ʾ��δ����
*/
bool BaseAction::IsMoveDone(){
	bool status;
	myRobot->lock();
	if(myRobot->isMoveDone()){
		status= true;
	}
	else{
		status= false;
	}
	myRobot->unlock();
	return status;
}
/*
���������ú���
������
heading����ת�Ƕȣ���λΪdeg������Ϊ��ʱ����ת
����ֵ����
*/
void BaseAction::SetHeading(double heading){
	myRobot->lock();
	printf("��������Ϊ %.2f deg/s\n",heading);
	myRobot->setHeading(heading);
	myRobot->unlock();
}
/*
������������ת�ٶ�
������
velocity����ת�ٶȣ���λΪdeg/sec��������ǰ�˶�����������˶�
����ֵ����
*/
void BaseAction::SetRotVel(double velocity){
	myRobot->lock();
	printf("��ת�ٶ�����Ϊ %.2f mm/s\n",velocity);
	myRobot->setRotVel(velocity);
	myRobot->unlock();
}
/*
������������Ժ���
������
heading����ת�Ƕȣ���λΪdeg������Ϊ��ʱ����ת
����ֵ����
*/
void BaseAction::SetDeltaHeading(double deltaHeading){
	myRobot->lock();
	printf("Delta��������Ϊ %.2f deg/s\n",deltaHeading);
	myRobot->setDeltaHeading(deltaHeading);
	myRobot->unlock();
}
/*
�������ж��Ƿ�����ת��Ԥ������
��������
����ֵ��true��ʾ�Ѿ�������false��ʾ��δ����
*/
bool BaseAction::IsHeadingDone(){
	bool status;
	myRobot->lock();
	if(myRobot->isHeadingDone()){
		status= true;
	}
	else{
		status= false;
	}
	myRobot->unlock();
	return status;
}
/*
���������ú����ٶ�
������
velocity���ٶȣ���λΪdeg/sec��
����ֵ����
*/
void BaseAction::SetLatVel(double latVelocity){
	myRobot->lock();
	printf("�����ٶ�����Ϊ %.2f mm/s\n",latVelocity);
	myRobot->setLatVel(latVelocity);
	myRobot->unlock();
}
/*
����������һ��ʱ��
������
ms��ʱ�䣬��λΪ����
����ֵ����
*/
void BaseAction::Sleep(unsigned int ms){
	myRobot->lock();
	printf("����˯�� %.2f mm\n",ms);
	ArUtil::sleep(ms);
	myRobot->unlock();
}
/*
����������ˮƽ���ٶ�
������
acc�����ٶȣ���λΪmm/s2
����ֵ����
*/
void BaseAction::SetTransAccel(double acc){
	myRobot->lock();
	printf("ˮƽ���ٶ�����Ϊ %.2f mm/s\n",acc);
	myRobot->setTransAccel(acc);
	myRobot->unlock();
}
/*
����������ˮƽ���ٶ�
������
decel�����ٶȣ���λΪmm/s2
����ֵ����
*/
void BaseAction::SetTransDecel(double decel){
	myRobot->lock();
	printf("ˮƽ���ٶ�����Ϊ %.2f mm/s\n",decel);
	myRobot->setTransDecel(decel);
	myRobot->unlock();
}
/*
������������ת���ٶ�
������
acc�����ٶȣ���λΪmm/s2
����ֵ����
*/
void BaseAction::SetRotAccel(double acc){
	myRobot->lock();
	printf("��ת���ٶ�����Ϊ %.2f mm/s\n",acc);
	myRobot->setRotAccel(acc);
	myRobot->unlock();
}
/*
������������ת���ٶ�
������
decel�����ٶȣ���λΪmm/s2
����ֵ����
*/
void BaseAction::SetRotDecel(double decel){
	myRobot->lock();
	printf("��ת���ٶ�����Ϊ %.2f mm/s\n",decel);
	myRobot->setRotDecel(decel);
	myRobot->unlock();
}
#pragma endregion



