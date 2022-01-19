#include "led.h"

void LED_Init(void)
{
//	P2M0 = 0x20;	//0 0 1 0 0 0 0 0  venus
//	P2M1 = 0x1f;  //0 0 0 1 1 1 1 1  p2.0~p2.4 推挽输出 / p2.5 高阻输入 / 余 准双向
	P2M0 &= 0xFC;
	P2M0 |= 0x00;
	
	P2M1 &= 0xFC;
	P2M1 |= 0x03;
}



