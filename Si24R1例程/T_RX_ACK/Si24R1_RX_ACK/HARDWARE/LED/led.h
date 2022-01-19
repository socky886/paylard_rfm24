#ifndef _LED_H
#define _LED_H

#include "sys.h"
													
sbit LED1  = P2^0;  
sbit LED2  = P2^1;  
sbit LED3  = P2^2;  
sbit LED4  = P2^3; 

void LED_Init(void);
void HorseRaceLED(void);

#endif

