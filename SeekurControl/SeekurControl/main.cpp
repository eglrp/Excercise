#include "BaseAction.h"
#include <process.h>

#include <iostream>

using namespace std;

void* work_thread(ArRobot* robot)
{
	//�߳�ִ����
	while(1){
		double x=robot->getX();
		double y=robot->getY();
		printf("x:%.2f\ty:%.2f",x,y);
	}
	return 0;
}

class JoydriveAction : public ArAction
{
public:
	// constructor
	JoydriveAction(void);
	// empty destructor
	virtual ~JoydriveAction(void);
	//the fire which will actually tell the resolver what to do
	virtual ArActionDesired *fire(ArActionDesired currentDesired);
	// whether the joystick is initalized or not
	bool joystickInited(void);
protected:
	// action desired
	ArActionDesired myDesired;
	// joystick handler
	ArJoyHandler myJoyHandler;
};
/*
Note the use of constructor chaining with ArAction.
*/
JoydriveAction::JoydriveAction(void) :
ArAction("Joydrive Action", "This action reads the joystick and sets the translational and rotational velocity based on this.")
{
	// initialize the joystick
	myJoyHandler.init();
	// set up the speed parameters on the joystick
	myJoyHandler.setSpeeds(50, 700);
}
JoydriveAction::~JoydriveAction(void)
{
}
// whether the joystick is there or not
bool JoydriveAction::joystickInited(void)
{
	return myJoyHandler.haveJoystick();
}
// the guts of the thing
ArActionDesired *JoydriveAction::fire(ArActionDesired currentDesired)
{
	int rot, trans;
	// print out some info about hte robot
	printf("\rx %6.1f  y %6.1f  tth  %6.1f vel %7.1f mpacs %3d", myRobot->getX(),
		myRobot->getY(), myRobot->getTh(), myRobot->getVel(),
		myRobot->getMotorPacCount());
	fflush(stdout);
	// see if one of the buttons is pushed, if so drive
	if (myJoyHandler.haveJoystick() && (myJoyHandler.getButton(1) ||
		myJoyHandler.getButton(2)))
	{
		// get the readings from the joystick
		myJoyHandler.getAdjusted(&rot, &trans);
		// set what we want to do
		myDesired.setVel(trans);
		myDesired.setDeltaHeading(-rot);
		// return the actionDesired
		return &myDesired;
	}
	else
	{
		// set what we want to do
		myDesired.setVel(0);
		myDesired.setDeltaHeading(0);
		// return the actionDesired
		return &myDesired;
	}
}

int main(int argc, char **argv){
	


	Aria::init();
	ArArgumentParser argParser(&argc,argv);
	argParser.loadDefaultArguments();

	ArRobot* robot = new ArRobot();
	ArRobotConnector con(&argParser,robot);

	//// �����潨�����Ӿ��
	//ConnHandler ch(&robot);

	//�������������еĲ���
	if(!Aria::parseArgs()){
		Aria::logOptions();
		Aria::exit(1);
		return 1;
	}
	//���ӻ�����
	if(!con.connectRobot()){
		ArLog::log(ArLog::Normal,"�޷����ӻ�����");
		if(argParser.checkHelpAndWarnUnparsed()){
			ArLog::log(ArLog::Terse, "�޷����ӻ�����");
		}
		Aria::exit(1);
		return 1;
	}

	robot->com2Bytes(116,6,1);
	robot->com2Bytes(116, 7, 1);
	robot->com2Bytes(116, 28, 1);
	robot->com2Bytes(116, 12, 1);

	// �첽���л����˴���ѭ��
	robot->enableMotors();
	robot->runAsync(true);
	//robot->comInt(ArCommands::ENABLE, 1);

	//�������������
	BaseAction action(robot);

	//�ֱ�������
	//JoydriveAction jdAct;
	//// if the joydrive action couldn't find the joystick, then exit.
	//if (!jdAct.joystickInited())
	//{
	//	printf("Do not have a joystick, set up the joystick then rerun the program\n\n");
	//	Aria::exit(1);
	//	return 1;
	//}

	bool repeat=true;
	while (repeat) 
	{
		printf("��������ָ��:0��ֹͣ�˶�\t1��ֱ��ǰ��һ�ξ���\t2��ֱ��ǰ���ٶ�\n\
			   3�����þ��Ժ���\t4��������Ժ���\t5����ת�ٶ�\n");
		printf("ָ��:");
		int com;
		double value;
		scanf("%d",&com);
		if (com!=0)
		{
			printf("��ֵ:");
			scanf("%lf",&value);
		}
		switch(com){
		case 0:
			action.Stop();
			break;
		case 1:
			action.Move(value);
			break;
		case 2:
			action.SetVelocity(value);
			break;
		case 3:
			//robot->addAction(&jdAct, 100);
			break;
		case 4:
			action.SetDeltaHeading(value);
			break;
		case 5:
			action.SetRotVel(value);
			break;
		default:
			repeat=false;
			break;
		}
	}
	robot->disconnect();
	Aria::exit();

}