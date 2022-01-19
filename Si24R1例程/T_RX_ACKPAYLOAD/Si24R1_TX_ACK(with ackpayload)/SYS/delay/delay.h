#ifndef _DELAY_H
#define _DELAY_H

#include "sys.h"
//延迟时间		@33.8688MHz
#define TIME_10uS        2
#define TIME_100uS       17
#define TIME_150uS       27

#define TIME_1MS				195
#define TIME_2MS				391         
#define TIME_5MS				985
#define TIME_10MS				1972
#define TIME_15MS				2958
#define TIME_20MS				3944
#define TIME_50MS				9860
#define TIME_80MS				15776
#define TIME_100MS			19720
#define TIME_150MS			19580
#define TIME_200MS			39440
#define TIME_300MS			59160
#define TIME_400MS			78880
#define TIME_500MS			98600
//#define TIME_1S					19720

#define DELAY_TIMES 	5000

//函数声明
void delay_us(uchar );
void delay_900us(void);
void delay(unsigned long time);  		//延迟程序


#endif

