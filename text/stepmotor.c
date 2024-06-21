#include "stepmotor.h"

void step_motor_send_pulse(u8 step,u8 dir)
{
	u8 temp=step;
	if(dir==0) //���Ϊ��ʱ����ת
		temp=7-step;//���������ź�
	switch(temp)//8 �����Ŀ��ƣ�(A+)->(A+B+)->(B+)->(B+A-)->(A-)->(A-B-)->(B-)>(B-A+)
	{

		case 0: IN_A=1;IN_B=0;IN_C=0;IN_D=0;break;
		case 1: IN_A=1;IN_B=0;IN_C=1;IN_D=0;break;
		case 2: IN_A=0;IN_B=0;IN_C=1;IN_D=0;break;
		case 3: IN_A=0;IN_B=1;IN_C=1;IN_D=0;break;
		case 4: IN_A=0;IN_B=1;IN_C=0;IN_D=0;break;
		case 5: IN_A=0;IN_B=1;IN_C=0;IN_D=1;break;
		case 6: IN_A=0;IN_B=0;IN_C=0;IN_D=1;break;
		case 7: IN_A=1;IN_B=0;IN_C=0;IN_D=1;break;
		default: IN_A=0;IN_B=0;IN_C=0;IN_D=0;break;//ֹͣ����

	}
}
