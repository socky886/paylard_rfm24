#include <stdio.h>
#include <intrins.h>

#include "sys.h"
#include "delay.h"
#include "Si24R1.h"
#include "spi.h"
#include "led.h"
#include "uart.h"
#include "key.h"

//***************2017/6/27venus ��ͨ NOACK��ACKģʽ**************

idata char    outbuf[OLEN];                   //���ͻ������Ĵ洢
idata uchar   ostart           = 0;           //���ͻ�������ʼ����
idata uchar   oend             = 0;           //���ͻ�������ʼ����
bit           sendfull         = 0;           //��־-����ʶ���ͻ�������
bit           sendactive       = 0;           //��־����ʶ����������

idata char    inbuf[ILEN];                    //���ͻ������Ĵ洢
idata uchar   istart           = 0;           //���ͻ�������ʼ����
idata uchar   iend             = 0;           //���ͻ�������ʼ����
bit inempty=0;

//----------------------------    ��־λ	----------------------------------
/*uchar bdata status_reg;
sbit  RX_DR	 = status_reg^6;   //����ʲô����,�����ڱ�׼c������extern��ֻ��Ҫ����Ҫ�õĵط��ٶ���
sbit  TX_DS	 = status_reg^5;
sbit  MAX_RT = status_reg^4;
sbit  TX_FULL = status_reg^0;
*/
//ϵͳ��ʼ��
//��ع��ܣ������ʼ��
void System_Init()
{
	EA = 1;
	KEY_Init();
	uart_init();
	LED_Init();
	SPI_Init();
}


void main(void)
{
	uchar counter = 0;
	uchar i;
	uchar len;
	uchar tmp;
	
	delay_900us();
	delay_900us();
	delay_900us();

	System_Init();
	printf("\n================Rx ACK test=================\n");

//	Si24R1_Rx_NoACK_Mode();
	Si24R1_Rx_ACK_Mode();

	while(1)
	{		
		if((tmp = Si24R1_RxPacket(RX_BUF)) == 0)
		{
			printf("\nReceived data: ");
			len=spi_rd_reg(R_RX_PL_WID);
			for(i = 0; i < len; i++)
			{			
				printf("%d ",(uint)RX_BUF[i]);		
			}	
			printf("\n");
			LED1 = !LED1;
		}

	}
}

