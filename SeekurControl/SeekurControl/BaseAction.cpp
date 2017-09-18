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

#pragma region ���Ӳ�����
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
	printf("directMotionDemo connection handler: Failed to connect.\n");
	myRobot->stopRunning();
	Aria::exit(1);
	return;
}

// �������������ر����ɺ�amigobot
void ConnHandler::connected(void)
{
	printf("directMotionDemo connection handler: Connected\n");
	myRobot->comInt(ArCommands::SONAR, 0);
	myRobot->comInt(ArCommands::ENABLE, 1);
	myRobot->comInt(ArCommands::SOUNDTOG, 0);
}

// ���ӶϿ�ʱ��������
void ConnHandler::disconnected(void)
{
	printf("directMotionDemo connection handler: Lost connection, exiting program.\n");
	Aria::exit(0);
}
#pragma endregion

