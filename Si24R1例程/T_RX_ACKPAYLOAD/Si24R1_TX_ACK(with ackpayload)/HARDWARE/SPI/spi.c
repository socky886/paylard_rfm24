#include <spi.h>


//=====================================================================================
//函数：spi_init()
//功能: SPI引脚初始化     
//=====================================================================================
void SPI_Init(void)
{

	P1M0 = 0x40;	//0 1 0 0 0 0 0 0  
	P1M1 = 0xB8;	//1 0 1 1 1 0 0 0  p1.7 p1.3-p1.5 推挽输出 / p1.6 高阻输入 / 其余准双向
	
/*	
	//设置CE CSN MOSI SCK 推挽输出 MISO 高阻输入
	P1M1 &= 0x07;
	P1M1 |= 0x40;
	P1M0 &= 0x07;
	P1M0 |= 0xB8;	
*/	
	
	//???
	P3M0 = 0x05;  	//0 0 0 0 0 1 0 1
	P3M1 = 0x02;	//0 0 0 0 0 0 1 0  p3.0 p3.2 高阻输入 / p3.1推挽输出
	
	//  SPCTL
	//  SSIG   SPEN  DORD  MSTR  |  CPOL  CPHA    SPR1 SPR0
	//   1       1    0      1   |   0     0       0    0
  	//  SSIG:  1: MSTR位用于确定器件为主/从    0:  SS脚用于确定器件为主/从
	//  SPEN:  1: SPI使能					   0:  SPI禁止
	//  DORD:  1: LSB先发送					   0:  MSB先发送
	//  MSTR:  1: 主机                         0:  从机
	//  CPOL:  1: SCLK高电平空闲，前沿为下降沿 0： SCLK低电平空闲，前沿为上升沿 
	//  CPHA:  1：SCLK前沿驱动，后时钟沿采样   0:  SS=0时驱动，前沿采样，后沿驱动
	//  SPR1、SPR0:  00: CPU_CLK/4    01  CPU_CLK/16   10  CPU_CLK/64   11  CPU_CLK/128

	SPDAT  = 0;
	SPSTAT = 0xC0;  //SPI状态寄存器清零

	SPCTL  = 0xD0+SPI_CLK;  //主机、MSB先发、SCLK低电平空闲,SPI_CLK

	CE  = 0;        // 待机
	CSN = 1;        // SPI禁止
	SCK = 0;        // SPI时钟置低

}