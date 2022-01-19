#include <stdio.h>
#include <intrins.h>

#include "sys.h"
#include "delay.h"
#include "Si24R1.h"
#include "spi.h"
#include "led.h"
#include "uart.h"

//***************2017/4/26venus 改格式***************
//数据包长度：8 bytes
//地址
//连续发数，发数间隔：500ms

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
		
		send_data(TX_BUF,TX_PLOAD_WIDTH );			 //发送数据
	
		while(IRQ == 1);					  		 //等待发送完成
		status_reg = spi_rw_reg(STATUS,0x00);        // 返回状态寄存器
//		status_reg = spi_rw_reg(STATUS,0x00);        // 返回状态寄存器
		
		printf("\n%x",(uint)status_reg);

		if(TX_DS)
		{
			spi_rw_reg(W_REGISTER + STATUS,status_reg);  // 清除TX_D中断标志
			
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

