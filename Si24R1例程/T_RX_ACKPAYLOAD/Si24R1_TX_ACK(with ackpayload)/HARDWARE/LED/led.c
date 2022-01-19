#include "led.h"
#include "delay.h"

void LED_Init(void)
{
//	P2M0 = 0x20;	//0 0 1 0 0 0 0 0  venus
//	P2M1 = 0x1f;  	//0 0 0 1 1 1 1 1  p2.0~p2.4 ������� / p2.5 �������� / �� ׼˫��
	P2M0 &= 0xF0;	//����λ����
	
	P2M1 &= 0xF0;
	P2M1 |= 0x0F;
	LED1 = LED2 = LED3 = LED4 = 1;
}


void HorseRaceLED(void)
{
	LED1 = 0;
	delay(TIME_500MS);
	LED1 = 1;
	delay(TIME_500MS);
	LED2 = 0;
	delay(TIME_500MS);
	LED2 = 1;
	delay(TIME_500MS);
	LED3 = 0;
	delay(TIME_500MS);
	LED3 = 1;
	delay(TIME_500MS);
	LED4 = 0;
	delay(TIME_500MS);
	LED4 = 1;
	delay(TIME_500MS);
}
