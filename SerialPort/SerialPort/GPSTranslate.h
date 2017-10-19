//////////////////////////////////////////////////////////////////////////
///   
/// @file    GPSTanslate.h    
/// @brief   GPS�ļ����� 
///  
/// ���ļ���ɴ���ͨ���������  
///  
/// @version 1.0     
/// @author  ¬��   
/// @E-mail��lujun.hust@gmail.com  
/// @date    2010/03/19  
///  
///  �޶�˵����  
//////////////////////////////////////////////////////////////////////////


#ifndef GPSTRANSLATE_H
#define GPSTRANSLATE_H

#include <time.h>

/*
GPS�������
*/
enum GPSType
{
	GGA,
	RMC,
	VTG
};

/*
�������GPS��䣬����
*/
class GPSInfo
{
public:
	GPSType InfoType;		//�������
private:

};

class GGAInfo:GPSInfo
{
public:
	tm UTCTime;				//UTCʱ�䣬����ʱ����
	double Latitude;		//γ��
	char NSIndicator;		//γ�Ȱ���ָʾ��
	double Longitude;		//����
	char EWIndicator;		//���Ȱ���ָʾ��
	int GPSStatus;			// GPS״̬��0=δ��λ��1=�ǲ�ֶ�λ��2=��ֶ�λ��6=���ڹ���
	int UseSatelliteNum;	//����ʹ�ý���λ�õ�����������00~12����ǰ���0Ҳ�������䣩
	double HDOP;			//HDOPˮƽ�������ӣ�0.5~99.9��
	double Altitude;		//���θ߶ȣ�-9999.9~99999.9��
	double Height;			//������������Դ��ˮ׼��ĸ߶�
	double DiffTime;		//���ʱ�䣨�����һ�ν��յ�����źſ�ʼ��������������ǲ�ֶ�λ��Ϊ�գ�
	int DiffId;				//���վID��0000~1023��ǰ���0Ҳ�������䣬������ǲ�ֶ�λ��Ϊ�գ�
};

class RMCInfo :GPSInfo
{
public:
	tm UTCTime;				//UTCʱ�䣬����ʱ����
	double Latitude;		//γ��
	char NSIndicator;		//γ�Ȱ���ָʾ��
	double Longitude;		//����
	char EWIndicator;		//���Ȱ���ָʾ��
	int GPSStatus;			// GPS״̬��0=δ��λ��1=�ǲ�ֶ�λ��2=��ֶ�λ��6=���ڹ���
	int UseSatelliteNum;	//����ʹ�ý���λ�õ�����������00~12����ǰ���0Ҳ�������䣩
	double HDOP;			//HDOPˮƽ�������ӣ�0.5~99.9��
	double Altitude;		//���θ߶ȣ�-9999.9~99999.9��
	double Height;			//������������Դ��ˮ׼��ĸ߶�
	double DiffTime;		//���ʱ�䣨�����һ�ν��յ�����źſ�ʼ��������������ǲ�ֶ�λ��Ϊ�գ�
	int DiffId;				//���վID��0000~1023��ǰ���0Ҳ�������䣬������ǲ�ֶ�λ��Ϊ�գ�
};

class GPSTanslate
{
public:
	GPSTanslate();
	~GPSTanslate();

private:

};

GPSTanslate::GPSTanslate()
{
}

GPSTanslate::~GPSTanslate()
{
}
#endif // !GPSTRANSLATE_H
