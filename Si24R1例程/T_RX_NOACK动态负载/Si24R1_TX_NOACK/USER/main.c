#include <stdio.h>
#include <intrins.h>

#include "sys.h"
#include "delay.h"
#include "Si24R1.h"
#include "spi.h"
#include "led.h"
#include "uart.h"

//***************2017/4/26venus �ĸ�ʽ***************
//���ݰ����ȣ�8 bytes
//��ַ
//�������������������500ms

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
	uchar m;
	
	delay_900us();
	delay_900us();
	delay_900us();

	System_Init();
	printf("\n================project test=================\n");

	tx_mode_setting();
	
//	m=spi_rd_reg(FEATURE);
//	printf("FEATURE=%x",(uint)m);
	
/**/
	while(1)
	{
		
		TX_BUF[0] = counter++;		
		
		send_data(TX_BUF,TX_PLOAD_WIDTH );			 //��������
	
		while(IRQ == 1);					  		 //�ȴ��������
		status_reg = spi_rw_reg(STATUS,0x00);        // ����״̬�Ĵ���
//		status_reg = spi_rw_reg(STATUS,0x00);        // ����״̬�Ĵ���
		
		printf("\n%x",(uint)status_reg);

		if(TX_DS)
		{
			spi_rw_reg(W_REGISTER + STATUS,status_reg);  // ���TX_D�жϱ�־
			
			printf("\n\rTransmited Data:\n\r");
			printf("\n**%d** \n",(uint)TX_BUF[0]);
			for(i = 1; i < TX_PLOAD_WIDTH; i++)
			{			
				printf("%d ",(uint)TX_BUF[i]);		
//				if(i%10==0)	printf("\n");
			}		
			printf("\n");

			}
		LED1 = !LED1;
		delay(TIME_10MS);
//		delay_900us();
	
	}
	
}// end

