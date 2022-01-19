#include "key.h"
#include "delay.h"

void KEY_Init(void)
{
	//����KEY����Ϊ���� P2.5
	P2M0 &= 0xDF;
	P2M0 |= 0x20;
	
	P2M1 &= 0xDF;// M0 M1
				//	1  0
}

//mode: 0,��֧������; 1,֧������
uchar KEY_Scan(uchar mode)
{
	static uchar key_up = 1;	//�����ɿ���־
	if(mode)
		key_up = 1;	//֧������
	if(key_up && KEY == 0)
	{
		delay(TIME_10MS);	//ȥ��
		key_up = 0;
		if(KEY == 0)
			return 1;
	}
	else if(KEY == 1) key_up = 1;
	return 0;	//�ް�������
}