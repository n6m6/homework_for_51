#ifndef __ired_H__
#define __ired_H__
 
#include "reg51.h"
#include "public.h"
 
extern u8 gired_data[4];

sbit IRED = P3^2;



void ired_init(void);
void ired();
 
 
#endif
