#include "TestFun.h"
#include <iostream>  
#include <fstream>

/*
���������캯��
������
@no���˿ں�
@baud��������
@path������ļ�·��
����ֵ����
*/
void TestFun::ConnectPort(int no, int baud, std::string path){
	CSerialPort mySerialPort;

	if (!mySerialPort.InitPort(no, baud))
	{
		std::cout << "��ʼ��ʧ�ܣ�" << std::endl;
	}
	else
	{
		std::cout << "��ʼ���ɹ���" << std::endl;
	}

	if (!mySerialPort.OpenListenThread())
	{
		std::cout << "�����̴߳�ʧ�ܣ�" << std::endl;
	}
	else
	{
		std::cout << "�����̴߳򿪳ɹ���" << std::endl;
	}

	int temp;
	std::cin >> temp;
	//std::ofstream out(path);
	//if (temp == 1){
		mySerialPort.CloseListenTread();
		for (int i = 0; i < mySerialPort.length; i++){
			for (int j = 0; mySerialPort.dataAarry[i][j] != '\n'; j++){
				std::cout << mySerialPort.dataAarry[i][j];
				//out << mySerialPort.dataAarry[i][j];
			}
			std::cout << std::endl;
			//out << '\n';
		}
	//}
}