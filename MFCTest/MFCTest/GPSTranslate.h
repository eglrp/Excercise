//////////////////////////////////////////////////////////////////////////
///   
/// @file    GPSTanslate.h    
/// @brief   GPS文件解析 
///  
/// 本文件完成串口通信类的声明  
///  
/// @version 1.0     
/// @author  吴厚波   
/// @E-mail：kdi1994@163.com 
/// @date    2017/11/14
///  
///  修订说明：  
//////////////////////////////////////////////////////////////////////////


#ifndef GPSTRANSLATE_H
#define GPSTRANSLATE_H

#include <time.h>
#include <string>
#include <vector>

using namespace std;

/*
GPS语句类型
*/
enum GPSType
{
	Zero,//无结果
	GGA,
	RMC,
	VTG,
	PSAT_HPR,
};

/*
解析后的GPS语句，基类
*/
class GPSInfo
{
public:
	GPSType InfoType;		//语句类型
private:

};

/*
解析后的GGA语句
*/
class GGAInfo :public GPSInfo
{
public:
	tm UTCHms;				//UTC时间，仅有时分秒
	double Latitude;		//纬度
	char NSIndicator;		//纬度半球指示符
	double Longitude;		//经度
	char EWIndicator;		//经度半球指示符
	int GPSStatus;			//GPS状态：0=未定位，1=非差分定位，2=差分定位，6=正在估算
	int UseSatelliteNum;	//正在使用解算位置的卫星数量（00~12）（前面的0也将被传输）
	double HDOP;			//HDOP水平精度因子（0.5~99.9）
	double Altitude;		//海拔高度（-9999.9~99999.9）
	double Height;			//地球椭球面相对大地水准面的高度
	double DiffSecs;		//差分时间（从最近一次接收到差分信号开始的秒数，如果不是差分定位将为空）
	int DiffId;				//差分站ID号0000~1023（前面的0也将被传输，如果不是差分定位将为空）
};

/*
解析后的RMC语句
*/
class RMCInfo :public GPSInfo
{
public:
	tm UTCHms;				//UTC时间，仅有时分秒
	char PosStatus;			//定位状态，A=有效定位，V=无效定位
	double Latitude;		//纬度
	char NSIndicator;		//纬度半球指示符
	double Longitude;		//经度
	char EWIndicator;		//经度半球指示符
	double SpeedKnot;		//地面速率（000.0~999.9节，前面的0也将被传输）
	double Heading;			//地面航向（000.0~359.9度，以真北为参考基准，前面的0也将被传输）
	tm UTCDate;				//UTC日期，ddmmyy（日月年）格式
	double MagnDecl;		//磁偏角（000.0~180.0度，前面的0也将被传输）
	char MagnDeclDir;		//磁偏角方向，E（东）或W（西）
	char ModeIndication;	//模式指示（仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效）
};

/*
解析后的VTG语句
*/
class VTGInfo :public GPSInfo
{
public:
	double TrueHeading;		//以真北为参考基准的地面航向（000~359度，前面的0也将被传输）
	double MagnHeading;		//以磁北为参考基准的地面航向（000~359度，前面的0也将被传输）
	double SpeedKnot;		//地面速率（000.0~999.9节，前面的0也将被传输）
	double SpeedKm;			//地面速率（0000.0~1851.8公里/小时，前面的0也将被传输）
	char ModeIndication;	//模式指示（仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效）
};

//$PSAT,HPR,102143.00,356.881,-0.387,-0.4,N*1E
/*
解析后的PSAT,HPR语句
*/
class PSAT_HPRInfo :public GPSInfo
{
public:
	tm UTCHms;				//UTC时间，仅有时分秒
	double Heading;			//航向
	double member1;
	double member2;
	string member3;
};

/*
我使用的GPS信息
*/
class MyGPSInfo
{
public:
	double Latitude;		//纬度
	double Longitude;		//经度
	double Heading;			//航向
	double SpeedKm;			//地面速率（0000.0~1851.8公里/小时，前面的0也将被传输）
	//char NSIndicator;		//纬度半球指示符
	//char EWIndicator;		//经度半球指示符
	//int GPSStatus;			//GPS状态：0=未定位，1=非差分定位，2=差分定位，6=正在估算

	double BJ54_X;		//BJ54 X坐标
	double BJ54_Y;		//BJ54_Y坐标
};

/*
GPS解析类
*/
class GPSTranslate
{
public:
	/*
	描述：构造函数
	参数：
		@line：GPS单行语句
	返回值：GPS解析语句指针
	*/
	GPSInfo* Tanslate(std::string str);
};



#endif // !GPSTRANSLATE_H
