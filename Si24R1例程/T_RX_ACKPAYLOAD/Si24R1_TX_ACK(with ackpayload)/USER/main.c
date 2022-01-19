#include <stdio.h>
#include <intrins.h>

#include "sys.h"
#include "delay.h"
#include "Si24R1.h"
#include "spi.h"
#include "led.h"
#include "uart.h"
#include "key.h"

//***************2017/6/27venus 调通 NoACKY模式和ACK模式**************

idata char    outbuf[OLEN];                   //发送缓冲区的存储
idata uchar   ostart           = 0;           //发送缓冲区起始索引
idata uchar   oend             = 0;           //发送缓冲区起始索引
bit           sendfull         = 0;           //标志-：标识发送缓冲区满
bit           sendactive       = 0;           //标志：标识发送器激活

idata char    inbuf[ILEN];                    //发送缓冲区的存储
idata uchar   istart           = 0;           //发送缓冲区起始索引
idata uchar   iend             = 0;           //发送缓冲区起始索引
bit inempty=0;

//----------------------------    标志位	----------------------------------
/*uchar bdata status_reg;


sbit  RX_DR	 = status_reg^6;   //这是什么操作,不属于标准c，不能extern，只需要把需要用的地方再定义
sbit  TX_DS	 = status_reg^5;
sbit  MAX_RT = status_reg^4;
sbit  TX_FULL = status_reg^0;
*/

//系统初始化
//相关功能，外设初始化
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

	Si24R1_Tx_ACK_Mode();	//进入发送模式
	txLen = TX_PLOAD_WIDTH;
	rxLen=0;
	
	while(1)
	{	
//		for(i=1; i<32; i++)	// 准备PAYLOAD
//			TX_BUF[i] = rand()%(255 + 1);	
		
		TX_BUF[0] = counter++;	
		if((sta = Si24R1_TxPacket(TX_BUF, txLen, RX_BUF, &rxLen)) == TRX_OK)
		{
			printf("\nSend Data:");               //打印发送的数据
			printf("%4d~ ",(uint)TX_BUF[0]);
			for(i=1;i<txLen;i++)
			{
				printf("%3d ",(uint)TX_BUF[i]);
			}
			printf("\n");
			printf(" Received ACK Payload:");          //打印接收到的ACKPAYLOAD
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

