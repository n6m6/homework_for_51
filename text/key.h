#ifndef __key_H__
#define __key_H__
 
#include "reg51.h"
 

sbit KEY1=P3^1;
sbit KEY2=P3^0;
sbit KEY3=P3^4;
sbit KEY4=P3^3;
sbit KEY5=P3^5;
 

#define KEY1_PRESS	1
#define KEY2_PRESS	2
#define KEY3_PRESS	3
#define KEY4_PRESS	4
#define KEY5_PRESS	5
#define KEY_UNPRESS	0
 
unsigned char key_scan();
 
 
#endif
