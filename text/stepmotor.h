#ifndef __stepmotor_H__
#define __stepmotor_H__
 
#include "reg51.h"
#include "public.h"
 

sbit IN_A=P1^0;
sbit IN_B=P1^1;
sbit IN_C=P1^2;
sbit IN_D=P1^3;
 
void step_motor_send_pulse(u8 step,u8 dir);//dir = 1 for +,dir = 0 for -
 
#endif
