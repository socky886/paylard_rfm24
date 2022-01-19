#include <stdio.h>
#include <intrins.h>

#include "sys.h"
#include "delay.h"
#include "Si24R1.h"
#include "spi.h"
#include "led.h"
#include "uart.h"
#include "key.h"

//***************2017/6/27venus ��ͨ NoACKYģʽ��ACKģʽ**************

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
	
	uchar sta,txLen,rxLen;
	delay_900us();
	delay_900us();
	delay_900us();

	System_Init();
	
	printf("\n================Tx ACK test=================\n");
/*	
	while(Si24R1_Check())		
	{
		printf("\nSi24R1 Check Error!\n");
	}
	printf("\nSi24R1 Check OK~\n");
	*/

	Si24R1_Tx_ACK_Mode();	//���뷢��ģʽ
	txLen = TX_PLOAD_WIDTH;
	rxLen=0;
	
	while(1)
	{	
//		for(i=1; i<32; i++)	// ׼��PAYLOAD
//			TX_BUF[i] = rand()%(255 + 1);	
		
		TX_BUF[0] = counter++;	
		if((sta = Si24R1_TxPacket(TX_BUF, txLen, RX_BUF, &rxLen)) == TRX_OK)
		{
			printf("\nSend Data:");               //��ӡ���͵�����
			printf("%4d~ ",(uint)TX_BUF[0]);
			for(i=1;i<txLen;i++)
			{
				printf("%3d ",(uint)TX_BUF[i]);
			}
			printf("\n");
			printf(" Received ACK Payload:");          //��ӡ���յ���ACKPAYLOAD
			for(i=0; i<rxLen; i++)
			{
				printf("%2d ", (uint)RX_BUF[i]);
			}
		}
		else
		printf("\nfail! return val: %x\n",(uint)sta);
		LED1 = !LED1;
		delay(TIME_500MS);
		delay(TIME_500MS);
		
	}
}

