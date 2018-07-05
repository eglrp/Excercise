#include<string>
#include <iostream>
using namespace std;

class Fuzzy_controller
{
public:
	const static int N = 7;//������������ģ���Ӽ��ĸ���
private:
	float target;//ϵͳ�Ŀ���Ŀ��
	float actual;//������õ�ʵ��ֵ
	float e;     //���
	float e_pre; //��һ�ε����
	float de;    //���ı仯��
	float emax;  //��������������
	float demax; //���绯�ʻ������������
	float umax;  //���������
	float Ke;    //Ke=n/emax,��������Ϊ[-3,-2,-1,0,1,2,3]
	float Kde;   //Ke=n/demax,��������Ϊ[-3,-2,-1,0,1,2,3]
	float Ku;    //Ke=umax/n,��������Ϊ[-3,-2,-1,0,1,2,3]
	int rule[N][N];//ģ�������
	string mf_t_e;   //e�������Ⱥ�������
	string mf_t_de;  //de�������Ⱥ�������
	string mf_t_u;   //u�������Ⱥ�������
	float *e_mf_paras; //���������Ⱥ����Ĳ���
	float *de_mf_paras;//����ƫ�������Ⱥ����Ĳ���
	float *u_mf_paras; //����������Ⱥ����Ĳ���

public:
	Fuzzy_controller(float e_max, float de_max, float u_max);
	~Fuzzy_controller();
	float trimf(float x, float a, float b, float c);          //���������Ⱥ���
	float gaussmf(float x, float ave, float sigma);          //��̬�����Ⱥ���
	float trapmf(float x, float a, float b, float c, float d); //���������Ⱥ���
	//����ģ�������Ⱥ����Ĳ���
	void setMf(const string & mf_type_e, float *e_mf, const string & mf_type_de, float *de_mf, const string & mf_type_u, float *u_mf);
	void setRule(int rulelist[N][N]);                          //����ģ������
	float realize(float t, float a);              //ʵ��ģ������
	void showInfo();                                      //��ʾ��ģ������������Ϣ
	void showMf(const string & type, float *mf_paras);      //��ʾ�����Ⱥ�������Ϣ
};