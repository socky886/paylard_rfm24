#include "led.h"

void LED_Init(void)
{
//	P2M0 = 0x20;	//0 0 1 0 0 0 0 0  venus
//	P2M1 = 0x1f;  //0 0 0 1 1 1 1 1  p2.0~p2.4 ������� / p2.5 �������� / �� ׼˫��
	P2M0 &= 0xFC;
	P2M0 |= 0x00;
	
	P2M1 &= 0xFC;
	P2M1 |= 0x03;
}



