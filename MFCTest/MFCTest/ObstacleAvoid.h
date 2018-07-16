//#include "stdafx.h"
#include<string>
#include <iostream>
//#include "MFCTestView.h"

#ifndef _OBSTACLEAVOID_H_
#define _OBSTACLEAVOID_H_

class ObstacleAvoid
{
public:
	const static int NANGLE = 7;//�Ƕ���������ģ���Ӽ���
	const static int NDIS = 5;//������������ģ���Ӽ���
private:
	float dis;     //������ľ���
	float angle;    //������ĽǶ�
	float dismax;  //���������������
	float anglemax; //�ǶȻ������������
	float rotmax;  //���������
	float Kdis;    //Kdis=n/dismax,��������Ϊ[-4,4],ÿ2��Ϊһ�α仯
	float Kangle;   //Kangle=n/anglemax,��������Ϊ[-6,6],ÿ2��Ϊһ�α仯
	float Krot;    //Krot=rotmax/n,��������Ϊ[-6,6],ÿ2��Ϊһ�α仯
	int rule[NDIS][NDIS];//ģ�������
	float dis_mf_paras[NDIS*3]; //����������Ⱥ����Ĳ���
	float angle_mf_paras[NANGLE * 3];//�Ƕȵ�ƫ�������Ⱥ����Ĳ���
	float rot_mf_paras[NANGLE * 3]; //ת��ǵ������Ⱥ����Ĳ���
	int obstacle[3];	//�����������ϰ�����Ϣ�����ݷֱ�Ϊ�ϰ������Ե�Ƕȣ���ǰ��Ϊ0����ʱ��Ϊ�������ұ�Ե�Ƕȣ������ĵ���롣
	//CMFCTestView *m_CView;	//���ƶ���ָ�룬���Կ��ƻ������˶�
public:
	ObstacleAvoid();
	~ObstacleAvoid();
	float trimf(float x, float a, float b, float c);          //���������Ⱥ���
	float ComputeAvoidHeading();              //ʵ��ģ�����ƣ�����ת���
	/*
	��������ʼ��
	������
	@dis_max��������
	@angle_max�����Ƕ�
	@rot_max�����ת���
	����ֵ��
	*/
	void Initialize(float dis_max, float angle_max, float rot_max);
	/*
	�����������ϰ���λ��
	������
	@laserData���������ݼ��ָ��
	@len�����ݳ���
	����ֵ��
	boolֵ����Ϊ���ϰ����Ϊ���ϰ���
	*/
	bool SetObstaclePosture(int* laserData, int len);
};

#endif