#include "Aria.h"
//�������ж˿����ݻ�ȡʵ��

// our robot object
ArRobot *robot;

bool getAuxPrinter(ArRobotPacket *packet)
{
  char c;
  
  // ����ⲻ��һ���������ݰ��򷵻�false������������������������
  // ��������ID����ڻ������ֲ��GETAUX����������
  if (packet->getID() != 0xb0) // 0xB0 is SERAUXpac. SERAUX2pac is 0xB8, SERAUX3pac is 0xC8.
    return false;

  // �Ӱ������л�ȡ���ݲ���ӡ
  while (packet->getReadLength () < packet->getLength() - 2)
  {
    c = packet->bufToUByte();
    if (c == '\r' || c == '\n')
    {
      putchar('\n');

      // �ڻ������ֲ�������η������ݸ�����
      // (TTY2����AUX1�˿ڣ�TTY3����AUX2)
      robot->comStr(ArCommands::TTY2, "Hello, World!\n\r");
    }
    else
      putchar(c);
    fflush(stdout);
  }

  
  // �����������
  robot->comInt(ArCommands::GETAUX, 1);

  // ��һ������12���ֽڣ�ʹ��:
  //robot->comInt(ArCommands::GETAUX, 12);

  // �����ӵڶ����������ڻ�ȡ���ݣ�ʹ��GETAUX2������Ƿ��ص����ݰ�������IDҲ��ͬ
  //robot->comInt(ArCommands::GETAUX2, 1);
  

  // ����true��ʾ�Ѿ������������
  return true;
}
  

int main(int argc, char **argv) 
{
  Aria::init();

  ArArgumentParser argParser(&argc, argv);
  ArSimpleConnector conn(&argParser);
  argParser.loadDefaultArguments();
  if(!Aria::parseArgs() || !argParser.checkHelpAndWarnUnparsed())
  {
    Aria::logOptions();
    return 1;
  }
  
  // ȫ��ָ��
  robot = new ArRobot;

  // ���������
  ArGlobalRetFunctor1<bool, ArRobotPacket *> getAuxCB(&getAuxPrinter);
  // ���������ݰ������ӵ������˵İ�����б��һλ
  robot->addPacketHandler(&getAuxCB, ArListPos::FIRST);

  // ���ӻ�����
  if(!conn.connectRobot(robot))
  {
      ArLog::log(ArLog::Terse, "getAuxExample: Error connecting to the robot.");
      return 2;
  }

  ArLog::log(ArLog::Normal, "getAuxExample: Connected to the robot. Sending command to change AUX1 baud rate to 9600...");
  robot->comInt(ArCommands::AUX1BAUD, 0); // See robot manual

  // �����1������
  robot->comInt(ArCommands::GETAUX, 1);

  robot->run(true);
  Aria::exit(0);
  return 0;  
}