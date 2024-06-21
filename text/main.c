#include "reg51.h"
#include "public.h"
#include "stepmotor.h"
#include "key.h"
#include "ired.h"
 
 
#define SPEEDMAX 1//max speed
#define SPEEDMIN 5//min speed


#define SMG_A_DP_PORT P0 //ʹ�ú궨������ܶ����
//�������������ʾ 0~F �Ķ�������
u8 gsmg_code[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
 
 
//**********************def of sign
int stepmove=0;//0 for stop,1 for begin
void main()
{	
	u8 key=0;//����ֵ
	u8 dir=0;//����ת��־λ
	u8 step=0;//�����������
	u8 show_num=0;//��̬�������ʾ����
	u8 Remote_control;//ң��������ʹ��
	u8 last_speed = 0;//��¼��Ȼ��ǰһ�̵�״̬
	u8 speed=SPEEDMAX;
	u8 Mode;//Mode=1:ң����ģʽ Mode=0:����ģʽ
	u8 i = 0;//��Ȼ��ѭ����־λ
	u8 j = 0;

	ired_init();//ң������ʼ��
		
 
	while(1)
	{
			Mode = 1;		
			Remote_control = 1;
			key=key_scan();	//get key_state
			//Ϊ��ֹ�ظ���ȡ������Remote_control��־λ����Remote_control=1�ǰ���������Ч��һ����Ч��Remote_control��Ϊ0
			if(Mode == 1)
			{
				if(gired_data[2] == 0x45 && Remote_control == 1)//�Ե��������־λstepmove��ֵ������һ���ź�״̬����һ��
				{
					Remote_control = 0;
					stepmove=!stepmove;
				}				
				if(Remote_control == 0 && gired_data[2] != 0x40 && gired_data[2] != 0x43 && gired_data[2] != 0x44	&& gired_data[2] != 0x45)
					Remote_control = 1;
				if(stepmove==1)	//start
				{
					step_motor_send_pulse(step++,dir);
					if(step==8)	step=0;
					delay_ms(speed);
				
					if(gired_data[2] == 0x44 && Remote_control == 1)//turn + to -
					{
						Remote_control = 0;
						dir=!dir;		
					}	
					else if (gired_data[2] == 0x43 && Remote_control == 1)//speed up
					{

						Remote_control = 0;
						if(speed>SPEEDMAX)
							speed-=1;
						
					}
					else if (gired_data[2] == 0x40 && Remote_control == 1)//speed down
					{
						Remote_control = 0;
						if(speed<SPEEDMIN) 
							speed+=1;	
					}
					delay_ms(SPEEDMAX);
					show_num = speed; //�������ʾ���ȵ�λ��1���5��С
				}
				else show_num = 0;
			}
			else
			{
				if(key==KEY1_PRESS)//start or stop
				{
					stepmove=!stepmove;
				}				
			
				if(stepmove==1)	//start
				{
					step_motor_send_pulse(step++,dir);
					if(step==8)	step=0;
					delay_ms(speed);
				
					if(key==KEY4_PRESS)//turn + to -
					{
						dir=!dir;			
					}	
					else if (key==KEY2_PRESS)//speed up
					{
						if(speed>SPEEDMAX)
							speed-=1;
						
					}
					else if (key==KEY3_PRESS)//speed down
					{
						if(speed<SPEEDMIN) 
							speed+=1;	
					}
					else if (key==KEY5_PRESS)//nature wind
					{
						last_speed = speed;//������Ȼ��ģʽǰ��speedֵ
						SMG_A_DP_PORT = ~gsmg_code[6];//��Ȼ���Ӧ�������ʾΪ6
						speed = 1;					  //��Ȼ��ģʽΪ���ȴ�1����5������5����1��ѭ������
						for(j=1;j<=5;j++)
						{
							for(i=0;i<=100;i++)
							{
								step_motor_send_pulse(step++,dir);
								if(step==8)	step=0;
								delay_ms(j);
							}
						}
						for(j=5;j>=1;j--)
						{
							for(i=0;i<=100;i++)
							{
								step_motor_send_pulse(step++,dir);
								if(step==8)	step=0;
								delay_ms(j);
							}
						}
						for(j=1;j<=5;j++)
						{
							for(i=0;i<=100;i++)
							{
								step_motor_send_pulse(step++,dir);
								if(step==8)	step=0;
								delay_ms(j);
							}
						}
						for(j=5;j>=1;j--)
						{
							for(i=0;i<=100;i++)
							{
								step_motor_send_pulse(step++,dir);
								if(step==8)	step=0;
								delay_ms(j);
							}
						}
						speed = last_speed;	 //speedֵ�ָ�Ϊԭʼֵ
					}
					delay_ms(SPEEDMAX);
					show_num = speed;//�������ʾ���ȵ�λ��1���5��С

				}
				else show_num = 0;
				
			}
			SMG_A_DP_PORT = ~gsmg_code[show_num];//����ܸ�ֵ

				
	}	//while
}//main
