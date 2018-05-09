#include "stdafx.h"
#include <iostream>  
#include "pid.h"  

using namespace std;

void  Pid_control::PID_init()
{
	pid.SetSpeed = 0.0;
	pid.ActualSpeed = 0.0;
	pid.err = 0.0;
	pid.err_last = 0.0;
	pid.voltage = 0.0;
	pid.integral = 0.0;
	pid.Kp = 0.2;
	pid.Ki = 0.04;
	pid.Kd = 0.2;
	pid.umax = 400;
	pid.umin = -200;
}

float Pid_control::PID_realize(float speed){
	int index;	//����״̬��0ʱΪPD���ƣ�1ʱΪPID����
	pid.SetSpeed = speed;
	pid.err = pid.SetSpeed - pid.ActualSpeed;

	//�жϵ�ǰ�ٶ��Ƿ�����ֵ��Χ��
	if (pid.ActualSpeed > pid.umax)
	{

		if (abs(pid.err) > 200)
		{
			index = 0;
		}
		else
		{
			//��ƫ��С����ֵʱ������I���ֿ���
			index = 1;
			pid.integral += pid.err;
		}
		pid.voltage = pid.Kp*pid.err + index*pid.Ki*pid.integral + pid.Kd*(pid.err - pid.err_last);
	}
	else if (pid.ActualSpeed<pid.umin){
		if (abs(pid.err)>200)
		{
			index = 0;
		}
		else{
			index = 1;
			if (pid.err > 0)
			{
				pid.integral += pid.err;
			}
		}
	}
	else{
		if (abs(pid.err) > 200)
		{
			index = 0;
		}
		else{
			index = 1;
			pid.integral += pid.err;
		}
	}

	pid.voltage = pid.Kp*pid.err + index*pid.Ki*pid.integral + pid.Kd*(pid.err - pid.err_last);

	pid.err_last = pid.err;
	pid.ActualSpeed = pid.voltage*1.0;
	return pid.ActualSpeed;
}
