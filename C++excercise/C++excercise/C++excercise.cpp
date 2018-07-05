#include "stdafx.h"
#include<iostream>
#include"Fuzzy_controller.h"
#define NB -3
#define NM -2
#define NS -1
#define ZO 0
#define PS 1
#define PM 2
#define PB 3


int main()
{
	float target = 600;
	float actual = 0;
	float u = 0;
	int ruleMatrix[7][7] = { { NB, NB, NM, NM, NS, ZO, ZO },
	{ NB, NB, NM, NS, NS, ZO, PS },
	{ NM, NM, NM, NS, ZO, PS, PS },
	{ NM, NM, NS, ZO, PS, PM, PM },
	{ NS, NS, ZO, PS, PS, PM, PM },
	{ NS, ZO, PS, PM, PM, PM, PB },
	{ ZO, ZO, PM, PM, PM, PB, PB } };//ģ�������
	float e_mf_paras[21] = { -3, -3, -2, -3, -2, -1, -2, -1, 0, -1, 0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 3 };//���e�������Ⱥ������������������Ⱥ���Ϊ�����ͣ�����3������Ϊһ��
	float de_mf_paras[21] = { -3, -3, -2, -3, -2, -1, -2, -1, 0, -1, 0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 3 };//���仯��de��ģ�������Ⱥ�������
	float u_mf_paras[21] = { -3, -3, -2, -3, -2, -1, -2, -1, 0, -1, 0, 1, 0, 1, 2, 1, 2, 3, 2, 3, 3 };//�����u�������Ⱥ�������
	Fuzzy_controller fuzzy(1000, 650, 500);//��������ʼ�����趨�����仯�ʣ���������ֵ
	fuzzy.setMf("trimf", e_mf_paras, "trimf", de_mf_paras, "trimf", u_mf_paras);//�趨ģ�������Ⱥ���
	fuzzy.setRule(ruleMatrix);//�趨ģ������
	cout << "num   target    actual" << endl;
	for (int i = 0; i<100; i++)
	{
		u = fuzzy.realize(target, actual);
		actual += u;
		cout << i << "      " << target << "      " << actual << endl;
	}
	fuzzy.showInfo();
	system("pause");
	return 0;
}