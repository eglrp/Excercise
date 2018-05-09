#ifndef _PID_H_  
#define _PID_H_  

typedef struct _pid{
	float SetSpeed;		//�����ٶ�
	float ActualSpeed;	//��ǰ�ٶ�
	float err;		//��ǰƫ��ֵ
	float err_last;	//��һ�ε�ƫ��ֵ
	float Kp;	//P���Ʊ���ϵ��
	float Ki;	//I���Ʊ���ϵ��
	float Kd;	//D���Ʊ���ϵ��
	float voltage;
	float integral;	//����
	float umax;		//��ֵ�ϱ߽�
	float umin;		//��ֵ�±߽�
}Pid;


class Pid_control
{
public:

	void PID_init();
	float PID_realize(float speed);

private:
	int index;
	Pid pid;
};
#endif
