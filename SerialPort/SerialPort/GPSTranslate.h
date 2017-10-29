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
#include <string>

using namespace std;

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

/*
�������GGA���
*/
class GGAInfo :public GPSInfo
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

/*
�������RMC���
*/
class RMCInfo :public GPSInfo
{
public:
	tm UTCTime;				//UTCʱ�䣬����ʱ����
	char PosStatus;			//��λ״̬��A=��Ч��λ��V=��Ч��λ
	double Latitude;		//γ��
	char NSIndicator;		//γ�Ȱ���ָʾ��
	double Longitude;		//����
	char EWIndicator;		//���Ȱ���ָʾ��
	double SpeedKnot;		//�������ʣ�000.0~999.9�ڣ�ǰ���0Ҳ�������䣩
	double Heading;			//���溽��000.0~359.9�ȣ����汱Ϊ�ο���׼��ǰ���0Ҳ�������䣩
	tm UTCDate;				//UTC���ڣ�ddmmyy�������꣩��ʽ
	double MagnDecl;		//��ƫ�ǣ�000.0~180.0�ȣ�ǰ���0Ҳ�������䣩
	char MagnDeclDir;		//��ƫ�Ƿ���E��������W������
	char ModeIndication;	//ģʽָʾ����NMEA0183 3.00�汾�����A=������λ��D=��֣�E=���㣬N=������Ч��
};

/*
�������VTG���
*/
class VTGInfo :public GPSInfo
{
public:
	double TrueHeading;		//���汱Ϊ�ο���׼�ĵ��溽��000~359�ȣ�ǰ���0Ҳ�������䣩
	double MagnHeading;		//�Դű�Ϊ�ο���׼�ĵ��溽��000~359�ȣ�ǰ���0Ҳ�������䣩
	double SpeedKnot;		//�������ʣ�000.0~999.9�ڣ�ǰ���0Ҳ�������䣩
	double SpeedKm;			//�������ʣ�0000.0~1851.8����/Сʱ��ǰ���0Ҳ�������䣩
	char ModeIndication;	//ģʽָʾ����NMEA0183 3.00�汾�����A=������λ��D=��֣�E=���㣬N=������Ч��
};

/*
GPS������
*/
class GPSTanslate
{
public:
	/*
	���������캯��
	������
		@line��GPS�������
	����ֵ��GPS�������
	*/
	GPSInfo* Tanslate(std::string str);
};



#endif // !GPSTRANSLATE_H
