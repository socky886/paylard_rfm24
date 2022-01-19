#include "key.h"
#include "delay.h"

void KEY_Init(void)
{
	//配置KEY引脚为输入 P2.5
	P2M0 &= 0xDF;
	P2M0 |= 0x20;
	
	P2M1 &= 0xDF;// M0 M1
				//	1  0
}

//mode: 0,不支持连按; 1,支持连按
uchar KEY_Scan(uchar mode)
{
	static uchar key_up = 1;	//按键松开标志
	if(mode)
		key_up = 1;	//支持连按
	if(key_up && KEY == 0)
	{
		delay(TIME_10MS);	//去抖
		key_up = 0;
		if(KEY == 0)
			return 1;
	}
	else if(KEY == 1) key_up = 1;
	return 0;	//无按键按下
}