/* �������������ݶ�ȡʾ��*/
#include "Aria.h"

int main(int argc, char **argv)
{
	Aria::init();
	ArLog::init(ArLog::StdErr, ArLog::Verbose);		//��һ����־�ڳ�ʼ����ʽ���ڸ÷�ʽ�У�������Ļ����ʾ���е��ù���
	ArRobot robot;
	ArArgumentParser parser(&argc, argv);
	parser.loadDefaultArguments();

	ArRobotConnector robotConnector(&parser, &robot);
	ArLaserConnector laserConnector(&parser, &robot, &robotConnector);

	// ���ӻ����ˣ����������ͺ����ƻ�ȡһЩ��ʼ���ݣ�Ȼ����ظû����˵Ĳ����ļ�
	if(!robotConnector.connectRobot())
	{
		ArLog::log(ArLog::Terse, "lasersExample: Could not connect to the robot.");
		if(parser.checkHelpAndWarnUnparsed())
		{
			// -help not given
			Aria::logOptions();
			Aria::exit(1);
		}
	}


	if (!Aria::parseArgs())
	{
		Aria::logOptions();
		Aria::exit(2);
		return 2;
	}

	ArLog::log(ArLog::Normal, "lasersExample: Connected to robot.");

	robot.runAsync(true);


	// �����ڲ����ļ��ж���ļ���
	// һЩ��ǩ����connectLasers()�Ĳ������Ӷ����ƴ�����Ϊ���ĸ�����ᱻ�ŵ�ArRobot�ļ���洢�б���
	if(!laserConnector.connectLasers())
	{
		ArLog::log(ArLog::Terse, "Could not connect to configured lasers. Exiting.");
		Aria::exit(3);
		return 3;
	}

	// �ȴ�һ��ʱ���Զ�ȡ��������
	ArUtil::sleep(500);

	ArLog::log(ArLog::Normal, "Connected to all lasers.");

	// ��������ӵļ����л�ȡ��������
	while(robot.isConnected())
	{
		int numLasers = 0;

		// ��ȡָ��ArRobot�����Ӽ����б��ָ�룬�����������Է������������޸ġ�
		// ÿ�����ⶼ��һ�����������Դ���ĳ����������������ƣ�laser->getName()����ʹ��������ΪArRobot::findLaser()�Ĳ�������ȡָ���Ӧ�����ָ�롣
		robot.lock();
		std::map<int, ArLaser*> *lasers = robot.getLaserMap();			//��ȡ�����˼���������б�

		for(std::map<int, ArLaser*>::const_iterator i = lasers->begin(); i != lasers->end(); ++i)
		{
			int laserIndex = (*i).first;		//�б��м��������
			ArLaser* laser = (*i).second;		//ָ�򼤹��ָ��
			if(!laser)
				continue;
			++numLasers;
			laser->lockDevice();		//�˴���ע�ͣ����ݺ������Ʋ����Ϊ��������

			// ��ǰ������һϵ���ϰ��������X,Y�����һЩ�������ԣ����������Լ������µ�����
			std::list<ArPoseWithTime*> *currentReadings = laser->getCurrentBuffer();

			// ��һ�������ǻ�ȡ�޶��Ƕȷ�Χ�ڵļ������¶���������ڸ�����Χ��û����Ч�����¶�����dist��laser->getMaxRange()���ã���dist will be greater than laser->getMaxRange()��
			double angle = 0;
			double dist = laser->currentReadingPolar(laser->getStartDegrees(), laser->getEndDegrees(), &angle);		//�����ֱ�Ϊ��ʼ�Ƕȡ������ǶȺ���Ч�Ƕȡ���3������Ĭ��Ϊ�գ��ǿ�ʱ����ȡ�������¶���ʱ����ǰ�Ƕȸ�ֵ����3������

			ArLog::log(ArLog::Normal, "Laser #%d (%s): %s. Have %d 'current' readings. Closest reading is at %3.0f degrees and is %2.4f meters away.", laserIndex, laser->getName(), (laser->isConnected() ? "connected" : "NOT CONNECTED"), currentReadings->size(), angle, dist/1000.0);
			laser->unlockDevice();
		}
		if(numLasers == 0)
			ArLog::log(ArLog::Normal, "No lasers.");
		else
			ArLog::log(ArLog::Normal, "");

		// ���������ˣ���Ϣ5��ȴ���һ��ѭ��
		robot.unlock();
		ArUtil::sleep(5000);
	}

	ArLog::log(ArLog::Normal, "lasersExample: exiting.");
	Aria::exit(0);
	return 0;
}