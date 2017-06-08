#include "Aria.h"
#include "ArGPS.h"
#include "ArGPSConnector.h"
#include "ArTrimbleGPS.h"
#include "ArTCMCompassDirect.h"
#include <iostream>

int main(int argc, char** argv)
{
  Aria::init();
  ArLog::init(ArLog::StdErr, ArLog::Normal);

  ArArgumentParser argParser(&argc, argv);
  argParser.loadDefaultArguments();
  ArSimpleConnector connector(&argParser);
  ArGPSConnector gpsConnector(&argParser);

  if(!Aria::parseArgs() || !argParser.checkHelpAndWarnUnparsed())
  {
    Aria::logOptions();
    ArLog::log(ArLog::Terse, "gpsExample options:\n  -printTable   Print data to standard output in regular columns rather than a refreshing terminal display, and print more digits of precision");
    return 1;
  }

  // ���ӻ�����
  ArRobot robot;
  if(!connector.connectRobot(&robot))
  {
    ArLog::log(ArLog::Terse, "gpsExample: Warning: Could not connect to robot.  Will not be able to switch GPS power on, or load GPS options from this robot's parameter file.");
  }
  else
  {
    ArLog::log(ArLog::Normal, "gpsExample: Connected to robot.");
    robot.runAsync(true);
  }

  // ��������в��� -printTable
  bool printTable = argParser.checkArgument("printTable");

  // ��Seekur�У�GPS�������ĵ�Դ������������(��3������Ϊ0ֹͣ����)
  robot.com2Bytes(116, 6, 1);

  // ����GPS�����ݻ�����ָ������������������������Ի�ȡ�����˵Ĳ����Ͷ�Ӧ��GPSװ��������Ϣ��Ĭ��ֵ�����������͡���Ʒ�ȣ�
  ArLog::log(ArLog::Normal, "gpsExample: Connecting to GPS, it may take a few seconds...");
  ArGPS *gps = gpsConnector.createGPS(&robot);
  if(!gps || !gps->connect())
  {
    ArLog::log(ArLog::Terse, "gpsExample: Error connecting to GPS device.  Try -gpsType, -gpsPort, and/or -gpsBaud command-line arguments. Use -help for help.");
    return -1;
  }

  if(gpsConnector.getGPSType() == ArGPSConnector::Simulator)
  {
    ArLog::log(ArLog::Normal, "gpsExample: GPS is a simulator. Setting dummy position.");
    //(dynamic_cast<ArSimulatedGPS*>(gps))->setSimulatedPosition(42.80709, -71.579047, 100);
  }


  ArLog::log(ArLog::Normal, "gpsExample: Reading data...");
  ArTime lastReadTime;
  if(printTable)
    gps->printDataLabelsHeader();
  while(true)
  {
    int r = gps->read();
    if(r & ArGPS::ReadError)
    {
      ArLog::log(ArLog::Terse, "gpsExample: Warning: error reading GPS data.");
      ArUtil::sleep(1000);
      continue;
    }


    if(r & ArGPS::ReadUpdated)
    {
      if(printTable)
      {
        gps->printData(false);
        printf("\n");
      }
      else
      {
        gps->printData();
        printf("\r");
      }
      fflush(stdout);
      ArUtil::sleep(500);
      lastReadTime.setToNow();
      continue;
    } else {
      if(lastReadTime.secSince() >= 5) {
        ArLog::log(ArLog::Terse, "gpsExample: Warning: haven't recieved any data from GPS for more than 5 seconds!");
      }
      ArUtil::sleep(1000);
      continue;
    }

  }
  return 0;
}