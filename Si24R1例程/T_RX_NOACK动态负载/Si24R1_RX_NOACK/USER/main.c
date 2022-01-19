#include <stdio.h>
#include <intrins.h>

#include "sys.h"
#include "delay.h"
#include "Si24R1.h"
#include "spi.h"
#include "led.h"
#include "uart.h"
#include "key.h"

//***************2017/4/26venus �ĸ�ʽ***************

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
uchar bdata status_reg;
sbit  RX_DR	 = status_reg^6;   //����ʲô����,�����ڱ�׼c������extern��ֻ��Ҫ����Ҫ�õĵط��ٶ���
sbit  TX_DS	 = status_reg^5;
sbit  MAX_RT = status_reg^4;
sbit  TX_FULL = status_reg^0;

int g_nCnt = 0;

//ϵͳ��ʼ��
//��ع��ܣ������ʼ��
void System_Init()
{
	EA = 1;
	uart_init();
	LED_Init();
	SPI_Init();
}


void main(void)
{
	uchar counter = 0;
	uchar i;
	
	g_nCnt = 0;
	
	delay_900us();
	delay_900us();
	delay_900us();

	System_Init();
	printf("\n===rx test====\n");

/*	
	for(i = 1; i < TX_PLOAD_WIDTH; i++)
	{
		TX_BUF[i]	= i;
	}	
*/
	rx_mode_setting();
	
	while(1)
	{		
		CE = 1;
		while(IRQ == 1);					  		 //�ȴ��������

		status_reg = spi_rw_reg(STATUS,0x00);        // ����״̬�Ĵ���

		if(RX_DR)
		{
			g_nCnt++;	//��������
			spi_rw_reg(W_REGISTER + STATUS,status_reg);  // ���RX_DR�жϱ�־
		  spi_read_buf(R_RX_PAYLOAD,RX_BUF,TX_PLOAD_WIDTH ) ;
//			printf("\r\n %d:  ",g_nCnt);
			for(i = 0; i < TX_PLOAD_WIDTH; i++)
			{			
				printf("%d ",(uint)RX_BUF[i]);		
			}	
			printf("\r\n");
			LED1 = !LED1;
		}
		
	}
}

