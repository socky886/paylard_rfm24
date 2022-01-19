#include <stdio.h>
#include <intrins.h>

#include "sys.h"
#include "delay.h"
#include "Si24R1.h"
#include "spi.h"
#include "led.h"
#include "uart.h"
#include "key.h"

//***************2017/4/26venus 改格式***************

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
uchar bdata status_reg;
sbit  RX_DR	 = status_reg^6;   //这是什么操作,不属于标准c，不能extern，只需要把需要用的地方再定义
sbit  TX_DS	 = status_reg^5;
sbit  MAX_RT = status_reg^4;
sbit  TX_FULL = status_reg^0;

int g_nCnt = 0;

//系统初始化
//相关功能，外设初始化
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
		while(IRQ == 1);					  		 //等待发送完成

		status_reg = spi_rw_reg(STATUS,0x00);        // 返回状态寄存器

		if(RX_DR)
		{
			g_nCnt++;	//计数自增
			spi_rw_reg(W_REGISTER + STATUS,status_reg);  // 清除RX_DR中断标志
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

