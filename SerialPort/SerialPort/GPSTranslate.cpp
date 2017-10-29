#include "GPSTranslate.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

//��������ת��ģ�庯��   
template <class Type>
Type stringToNum(const string str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

/*
���������캯��
������
@line��GPS�������
����ֵ��GPS�������
*/
GPSInfo* GPSTanslate::Tanslate(string line)
{
	vector<string> arr1;  //����һ���ַ�������    
	int position = 0;
	do
	{
		string tmp_s;
		position = line.find(","); //�ҵ����ŵ�λ��     
		tmp_s = line.substr(0, position); //��ȡ��Ҫ���ַ���      
		line.erase(0, position + 1); //���Ѷ�ȡ������ɾȥ       
		arr1.push_back(tmp_s);   //���ַ���ѹ��������    
	} while (position != -1);
	GPSInfo* info;
	string tag = arr1[0];
	if (strcmp(tag.c_str(), "$GPGGA"))
	{
		info = new GGAInfo();

	} 
	else
	{
	}
}