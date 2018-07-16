#include "stdafx.h"
#include "ObstacleAvoid.h"

#define VN -4
#define N -2
#define M 0
#define F 2
#define VF 4
#define LL -6
#define LM -4
#define LS -2
#define ZO 0
#define RS 2
#define RM 4
#define RL 6

#define NL -6
#define NM -4
#define NS -2
#define PS 2
#define PM 4
#define PL 6

ObstacleAvoid::ObstacleAvoid() 
{
	
}

ObstacleAvoid::~ObstacleAvoid()
{
	delete[] dis_mf_paras;
	delete[] angle_mf_paras;
	delete[] rot_mf_paras;
}
//���������Ⱥ���
float ObstacleAvoid::trimf(float x, float a, float b, float c)
{
	float u;
	if (x >= a&&x <= b)
		u = (x - a) / (b - a);
	else if (x > b&&x <= c)
		u = (c - x) / (c - b);
	else
		u = 0.0;
	return u;

}
//����ģ�����
float ObstacleAvoid::ComputeAvoidHeading()
{
	float u_dis[NDIS], u_angle[NANGLE], u_rot[NANGLE];
	int u_dis_index[3], u_angle_index[3];//����һ��������༤��3��ģ���Ӽ�
	float rot;
	int INTER = 3;	//���Ǻ���������

	//ģ����������������
	int j = 0;
	for (int i = 0; i < NDIS; i++)
	{
		u_dis[i] = trimf(dis, dis_mf_paras[i*INTER], dis_mf_paras[i*INTER + 1], dis_mf_paras[i*INTER + 2]);//eģ��������������������
		if (u_dis[i] != 0)
			u_dis_index[j++] = i;                                              //�洢�������ģ���Ӽ����±꣬���Լ�С������
	}
	for (; j < 3; j++)u_dis_index[j] = 0;

	j = 0;
	for (int i = 0; i < NANGLE; i++)
	{
		u_angle[i] = trimf(angle, angle_mf_paras[i*INTER], angle_mf_paras[i*INTER + 1], angle_mf_paras[i*INTER + 2]);//deģ��������������������
		if (u_angle[i] != 0)
			u_angle_index[j++] = i;                                                    //�洢�������ģ���Ӽ����±꣬���Լ�С������
	}
	for (; j < 3; j++)u_angle_index[j] = 0;

	float den = 0, num = 0;
	for (int m = 0; m < 3; m++)
	for (int n = 0; n < 3; n++)
	{
		num += u_dis[u_dis_index[m]] * u_angle[u_angle_index[n]] * rule[u_dis_index[m]][u_angle_index[n]];
		den += u_dis[u_dis_index[m]] * u_angle[u_angle_index[n]];
	}
	rot = num / den;
	rot = Krot*rot;
	if (rot >= rotmax)   rot = rotmax;
	else if (rot <= -rotmax)  rot = -rotmax;
	return rot;
}

// ��ʼ��
void ObstacleAvoid::Initialize(float dis_max, float angle_max, float rot_max)
{
	dismax = dis_max;
	anglemax = angle_max;
	rotmax = rot_max;
	dis = dis_max;
	angle = angle_max;
	Kdis = (NDIS - 1) / dismax;
	Kangle = (NANGLE - 1) / anglemax;
	Krot = rotmax / (NANGLE - 1);

	int ruleMatrix[NANGLE][NDIS] = {
		{ NS, ZO, ZO, ZO, ZO },
		{ NS, NS, ZO, ZO, ZO },
		{ NM, NM, NS, ZO, ZO },
		{ NL, NL, NM, NS, ZO },
		{ PM, PM, PS, ZO, ZO },
		{ PS, PS, ZO, ZO, ZO },
		{ PS, ZO, ZO, ZO, ZO },
	};//ģ�������
	float dis_tp_paras[NDIS * 3] = {
		VN, VN, N,
		VN, N, M,
		N, M, F,
		M, F, VF,
		F, VF, VF
	};//����dis�������Ⱥ������������������Ⱥ���Ϊ�����ͣ�����3������Ϊһ��
	float angle_tp_paras[NANGLE * 3] = {
		LL, LL, LM,
		LL, LM, LS,
		LM, LS, ZO,
		LS, ZO, RS,
		ZO, RS, RM,
		RS, RM, RL,
		RM, RL, RL
	};//�Ƕ�angle�������Ⱥ�������
	float rot_tp_paras[NANGLE * 3] = {
		NL, NL, NM,
		NL, NM, NS,
		NM, NS, ZO,
		NS, ZO, PS,
		ZO, PS, PM,
		PS, PM, PL,
		PM, PL, PL
	};//�����ת��rot�������Ⱥ�������

	//�趨ģ�������Ⱥ���
	for (int i = 0; i < NDIS * 3; i++)
		dis_mf_paras[i] = dis_tp_paras[i];
	for (int i = 0; i < NANGLE * 3; i++)
		angle_mf_paras[i] = angle_tp_paras[i];
	for (int i = 0; i < NANGLE * 3; i++)
		rot_mf_paras[i] = rot_tp_paras[i];

	//�趨ģ������
	for (int i = 0; i < NANGLE; i++)
	for (int j = 0; j < NDIS; j++)
		rule[i][j] = ruleMatrix[i][j];
}

/*
�����������ϰ���λ��
������
@laserData���������ݼ��ָ��
@len�����ݳ���
����ֵ��
boolֵ����Ϊ���ϰ����Ϊ���ϰ���
*/
bool ObstacleAvoid::SetObstaclePosture(int* laserData, int len)
{
	bool result;	//�Ƿ����ϰ���
	//����ɨ�����ݣ�ɸѡ��10m��������ϰ���
	int status = 0;	//��ǰ�ϰ���״̬λ��0Ϊ���ϰ��1Ϊ�������ϰ�����ˣ�2Ϊ�������ϰ����Ҷ�
	obstacle[0] = -91;	//�����Ե�Ƕȳ�ʼ��Ϊ-91�����ڶԱ��ϰ���
	for (int i = 0; i < len; i++){
		if (laserData[i] <= 10 * 100)	//Ĭ����cmΪ��λ��i��Ϊ�Ƕȣ�0-180����ʱ�����
		{
			//������µ��ϰ���Ա��Ƿ���ӽ�����
			if (status == 0)
			{
				if (abs(i - 90)>abs(obstacle[0]))
				{
					//����ʱ���¼�¼
					obstacle[0] = i - 90;
					status = 1;
				}
			}
			else
			{
				obstacle[1] = i - 90;
			}
		}
		//��һ���ϰ�������������״̬
		else if (status > 0)
		{
			status = 0;
		}
	}

	if (obstacle[0]==-91)
	{
		result = false;
	}
	else
	{
		//�ж��ϰ���λ����໹���Ҳ�
		//���
		if (obstacle[0] >= 0)
		{
			angle = Kangle*obstacle[0];
			dis = Kdis*laserData[obstacle[0] + 90];
		}
		//�Ҳ�
		else if (obstacle[1] <= 0)
		{
			angle = Kangle*obstacle[1];
			dis = Kdis*laserData[obstacle[1] + 90];
		}
		//��ǰ��
		else
		{
			int tp_angle = -obstacle[0] < obstacle[1] ? obstacle[0] : obstacle[1];
			angle = Kangle*tp_angle;
			dis = Kdis*laserData[tp_angle + 90];
		}
		result = true;
	}
	return result;
}
