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
	GGAInfo();
	~GGAInfo();
	tm UTCTime;
	double Latitude;
	char NSIndicator;
	double Longitude;
	char EWIndicator;
	int GPSStatus;
	int UseSatelliteNum;
	double HDOP;
	double Altitude;


private:

};

GGAInfo::GGAInfo()
{
}

GGAInfo::~GGAInfo()
{
}

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
