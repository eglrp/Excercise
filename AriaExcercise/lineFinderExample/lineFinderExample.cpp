/* Aria���Բ�����ʵ�ó���ļ�ʾ������ʹ�ü������ǵ��������������ĵ㡣 ��ʾ�����򽫲��ϵ������������ǵĴ����������е���״ͼ����
�������f��������Щ�㽫�����ڡ��㡱�͡��ߡ��ļ��С� ʹ�ü�ͷ������ݸ�ң�ػ����ˡ� */
#include "Aria.h"

int main(int argc, char **argv)
{
	Aria::init();

	ArSimpleConnector connector(&argc, argv);
	ArRobot robot;
	ArSick sick;		//����SICK LMS-2XXϵ�еļ���

	if (!Aria::parseArgs() || argc > 1)
	{
		Aria::logOptions();
		Aria::exit(1);
		return 1;
	}

	ArKeyHandler keyHandler;
	Aria::setKeyHandler(&keyHandler);
	robot.attachKeyHandler(&keyHandler);


	robot.addRangeDevice(&sick);

	// ����ArLineFinder�����������������������ϸ��Ϣ
	ArLineFinder lineFinder(&sick);
	lineFinder.setVerbose(true);

	// Ϊ�����󶨻ص�������ArLineFinder::getLinesAndSaveThem()��
	// �����ڵ�ǰ�������Ƕ����е����������ԡ�points���͡�lines��Ϊ�ļ����洢
	ArFunctorC<ArLineFinder> findLineCB(&lineFinder, 
		&ArLineFinder::getLinesAndSaveThem);
	keyHandler.addKeyHandler('f', &findLineCB);
	keyHandler.addKeyHandler('F', &findLineCB);


	ArLog::log(ArLog::Normal, "lineFinderExample: connecting to robot...");
	if (!connector.connectRobot(&robot))
	{
		printf("Could not connect to robot... exiting\n");
		Aria::exit(1);  // exit program with error code 1
		return 1;
	}
	robot.runAsync(true);

	// ƥ�伤��
	ArLog::log(ArLog::Normal, "lineFinderExample: connecting to SICK laser...");
	connector.setupLaser(&sick);
	sick.runAsync();
	if (!sick.blockingConnect())
	{
		printf("Could not connect to SICK laser... exiting\n");
		Aria::exit(1);
		return 1;
	}

	printf("If you press the 'f' key the points and lines found will be saved\n");
	printf("Into the 'points' and 'lines' file in the current working directory\n");

	robot.waitForRunExit();
	Aria::exit(0);
	return 0;
}