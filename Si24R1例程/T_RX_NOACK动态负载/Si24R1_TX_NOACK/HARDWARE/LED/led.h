#ifndef _LED_H
#define _LED_H

#include "sys.h"
													
sbit LED1  = P2^0;  
sbit LED2  = P2^1;  

void LED_Init(void);

#endif

