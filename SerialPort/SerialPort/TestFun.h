#ifndef TESTFUN_H
#define TESTFUN_H

#include "SerialPort.h"  
#include <string>

/*
�����ú�����
*/
class TestFun
{
public:
	/*
	���������캯��
	������
		@no���˿ں�
		@baud��������
		@path������ļ�·��
	����ֵ����
	*/
	void ConnectPort(int no = 3, int baud = 115200, std::string path = "data.txt");
};

#endif // !TESTFUN_H
