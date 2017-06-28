#include "Aria.h"

// ʹ���������ʱ�������ó���Ȼ��ʹ�á�telnet���������ӱ���TCP�˿�7171�����ӷ���Ϊ���telnet localhost 7171
// ��������ʾ��Ҫ����ʱ���������룬�����س�����
// ʹ��help���������ʾ�������
// ʹ�á�test����test2��ѡ���Ƿ���Ӷ������
// �ڲ�ʹ��ArNetworking���MobileEyes������£�ArNetServer��һ���ṩͨ����������򵥿��Ƴ����ڳ��ص��������еķ�����
// Ҳ������һ��������ͬʱʹ�����ַ���

// ������ֻ����ͻ��������뷵�ص�����
void test(char **argv, int argc, ArSocket *socket)
{
	int i;
	printf("Client said: ");
	for (i = 0; i < argc; ++i)
		printf("\t%s\n", argv[i]);
	printf("\n");
	socket->writeString("Thank you, command received.");
}


int main(int argc, char **argv)
{
	Aria::init();
	// ����ArNetServer����
	ArNetServer server;
	// �����ص�����
	ArGlobalFunctor3<char **, int, ArSocket *> testCB(&test);

	// �ڲ����ӻ�������������Ӷ˿�7171���������ͻ�ͬʱ����
	if (!server.open(NULL, 8101, "password", true))
	{
		printf("Could not open server.\n");
		Aria::exit(1);
		return 1;
	}

	// ��Ӳ�������
	server.addCommand("test", &testCB, "this simply prints out the command given on the server");
	server.addCommand("test2", &testCB, "this simply prints out the command given on the server");

	//server.setLoggingDataSent(true);
	//server.setLoggingDataReceived(true);
	// run while the server is running
	while (server.isOpen() && Aria::getRunning())
	{
		server.runOnce();
		ArUtil::sleep(1);
	}
	server.close();
	Aria::exit(0);
	return 0;  
}