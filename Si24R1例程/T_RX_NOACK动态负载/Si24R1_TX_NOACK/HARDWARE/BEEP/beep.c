#include "beep.h"

void BEEP_Init(void)
{
	P2M0 &= 0xEF;	//P2.4�������
	P2M1 &= 0xEF;
	P2M1 |= 0x10;
}


