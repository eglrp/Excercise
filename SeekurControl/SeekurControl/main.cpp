#include "BaseAction.h"
#include <process.h>

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

int main(int argc, char **argv){
	Aria::init();
	ArArgumentParser argParser(&argc,argv);
	argParser.loadDefaultArguments();

	ArRobot robot;
	ArRobotConnector con(&argParser,&robot);

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
			Aria::logOptions();
		}
		Aria::exit(1);
		return 1;
	}

	// �첽���л����˴���ѭ��
	robot.enableMotors();
	robot.runAsync(false);

	//�������������
	BaseAction action(&robot);

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
			action.SetHeading(value);
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
	robot.disconnect();
	Aria::exit();

}