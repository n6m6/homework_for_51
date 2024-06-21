#include "reg51.h"
#include "public.h"
#include "stepmotor.h"
#include "key.h"
#include "ired.h"
 
 
#define SPEEDMAX 1//max speed
#define SPEEDMIN 5//min speed


#define SMG_A_DP_PORT P0 //使用宏定义数码管段码口
//共阴极数码管显示 0~F 的段码数据
u8 gsmg_code[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
 
 
//**********************def of sign
int stepmove=0;//0 for stop,1 for begin
void main()
{	
	u8 key=0;//按键值
	u8 dir=0;//正反转标志位
	u8 step=0;//步进电机步数
	u8 show_num=0;//静态数码管显示数字
	u8 Remote_control;//遥控器控制使能
	u8 last_speed = 0;//记录自然风前一刻的状态
	u8 speed=SPEEDMAX;
	u8 Mode;//Mode=1:遥控器模式 Mode=0:按键模式
	u8 i = 0;//自然风循环标志位
	u8 j = 0;

	ired_init();//遥控器初始化
		
 
	while(1)
	{
			Mode = 1;		
			Remote_control = 1;
			key=key_scan();	//get key_state
			//为防止重复读取，设置Remote_control标志位，当Remote_control=1是按键按下有效，一次有效后Remote_control变为0
			if(Mode == 1)
			{
				if(gired_data[2] == 0x45 && Remote_control == 1)//对电机启动标志位stepmove赋值，接受一次信号状态反向一次
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
					show_num = speed; //数码管显示当先档位，1最大，5最小
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
						last_speed = speed;//保存自然风模式前的speed值
						SMG_A_DP_PORT = ~gsmg_code[6];//自然风对应数码管显示为6
						speed = 1;					  //自然风模式为风扇从1档到5档再由5档到1档循环两次
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
						speed = last_speed;	 //speed值恢复为原始值
					}
					delay_ms(SPEEDMAX);
					show_num = speed;//数码管显示当先档位，1最大，5最小

				}
				else show_num = 0;
				
			}
			SMG_A_DP_PORT = ~gsmg_code[show_num];//数码管赋值

				
	}	//while
}//main
