#include "ired.h"



/********************************************************************
***********
* 函 数 名 : ired_init
* 函数功能 : 红外端口初始化函数，外部中断 0 配置
* 输 入 : 无
* 输 出 : 无
*********************************************************************
**********/

u8 gired_data[4];

void ired_init(void)
{
	IT0=1; //下降沿触发
	EX0=1; //打开中断 0 允许
	EA=1; //打开总中断
	IRED=1; //初始化端口
}

void ired() interrupt 0 //外部中断 0 服务函数
{
	u8 ired_high_time=0;
	u16 time_cnt=0;
	u8 i=0,j=0;
	if(IRED==0)
	{
		time_cnt=1000;
		while((!IRED)&&(time_cnt))//等待引导信号 9ms 低电平结束，若超过 10ms强制退出
		{
			delay_10us(1);//延时约 10us
			time_cnt--;
			if(time_cnt==0)return;
		}
		if(IRED)//引导信号 9ms 低电平已过，进入 4.5ms 高电平
		{
			time_cnt=500;
			while(IRED&&time_cnt)//等待引导信号 4.5ms 高电平结束，若超过 5ms 强制退出
			{
				delay_10us(1);
				time_cnt--;
				if(time_cnt==0)return;
			}
			for(i=0;i<4;i++)//循环 4 次，读取 4 个字节数据
			{
				for(j=0;j<8;j++)//循环 8 次读取每位数据即一个字节
				{
					time_cnt=600;
					while((IRED==0)&&time_cnt)//等待数据 1 或 0 前面的 0.56ms结束，若超过 6ms 强制退出
					{
						delay_10us(1);
						time_cnt--;
						if(time_cnt==0)return;
					}
					time_cnt=20;
					while(IRED)//等待数据 1 或 0 后面的高电平结束，若超过 2ms 强制退出
					{
						delay_10us(10);//约 0.1ms
						ired_high_time++;
						if(ired_high_time>20)return;
					}
					gired_data[i]>>=1;//先读取的为低位，然后是高位
					if(ired_high_time>=8)//如果高电平时间大于 0.8ms，数据则为1，否则为 0
						gired_data[i]|=0x80;
					ired_high_time=0;//重新清零，等待下一次计算时间
				}
			}
		}
		if(gired_data[2]!=~gired_data[3])//校验控制码与反码，错误则返回
		{
			for(i=0;i<4;i++)
				gired_data[i]=0;
			return;
		}
	}
}


