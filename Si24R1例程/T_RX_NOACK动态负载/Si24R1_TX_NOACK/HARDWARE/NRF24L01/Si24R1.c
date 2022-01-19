#include "Si24R1.h"
#include "spi.h"
#include "delay.h"

//----------------------------   全局变量   ----------------------------------
uchar code TX_ADDRESS[TX_ADR_WIDTH] = {0xE7,0xE7,0xE7,0xE7,0xA7};  // 定义一个静态发送地址
uchar TX_BUF[TX_PLOAD_WIDTH] = {0,7,32,5,16,5,2,1};

//=====================================================================================
//函数：spi_rw_byte()
//功能：根据SPI协议，写一字节数据到Si24R1，同时从Si24R1读出一字节
//=====================================================================================
uchar spi_rw_byte(uchar byte)
{
	uchar status;
	
	SPDAT = byte;
	
	while( (SPSTAT & 0x80) == 0);	//等待发送完成
	SPSTAT =  0xC0;  				//SPI状态寄存器清零

	status = SPDAT;
	return status;

}

//=====================================================================================
//函数：spi_rw_reg()
//功能：写数据value到reg寄存器，同时返回状态寄存器值
//=====================================================================================
uchar spi_rw_reg(uchar reg, uchar value)
{
	uchar status;

  	CSN 	= 0;                   	// CSN置低，开始传输数据
  	status = spi_rw_byte(reg);   	// 选择寄存器，同时返回状态字
  	spi_rw_byte(value);             // 然后写数据到该寄存器
  	CSN 	= 1;                   	// CSN拉高，结束数据传输
  	return(status);            		// 返回状态寄存器
}

//=====================================================================================
//函数：spi_rd_reg()
//功能：从reg寄存器读一个字节的数据
//=====================================================================================
uchar spi_rd_reg(uchar reg)
{
	uchar value;

  	CSN 	= 0;                	// CSN置低，开始传输数据
  	spi_rw_byte(reg);              	// 选择寄存器
  	value 	= spi_rw_byte(0);      	// 然后从该寄存器读数据 
  	CSN 	= 1;                	// CSN拉高，结束数据传输
  	return(value);            		// 返回寄存器数据
}

//=====================================================================================
//函数：spi_read_buf()
//功能：从reg寄存器读出bytes个字节，通常用来读取接收通道数据 或 接收/发送地址
//=====================================================================================
uchar spi_read_buf(uchar reg, uchar *pBuf, uchar bytes)
{

	uchar status;
	uchar i; 	

  	CSN 	= 0;                   	 // CSN置低，开始传输数据
  	status  = spi_rw_byte(reg);      // 选择寄存器，同时返回状态字
  	for(i = 0; i < bytes; i++)
    {
   		pBuf[i] = spi_rw_byte(0);    // 逐个字节从Si24R1读出
	}
  	CSN 	= 1;                     // CSN拉高，结束数据传输
  	return(status);             	 // 返回状态寄存器
}

//=====================================================================================
//函数：spi_write_buf()
//功能：把pBuf缓存中的数据写入到Si24R1，通常用来写入发射通道数据 或 接收/发送地址
//=====================================================================================
uchar spi_write_buf(uchar reg, uchar *pBuf, uchar bytes)
{
	uchar status, i;
  	CSN 	= 0;                     // CSN置低，开始传输数据
  	status 	= spi_rw_byte(reg);      // 选择寄存器，同时返回状态字
  	for(i = 0; i < bytes; i++)
    {
   		spi_rw_byte(pBuf[i]);        // 逐个字节写入Si24R1
	}
  	CSN 	= 1;                     // CSN拉高，结束数据传输
  	return(status);             	 // 返回状态寄存器
}


void tx_mode_setting( void)
{
	CE = 0;

	//spi_rw_reg(FLUSH_TX,0x00);

	spi_write_buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     		// 写入发送地址
	spi_rw_reg(W_REGISTER + FEATURE, 0x05);      					     	// 使能NO_ACK_TX
	spi_rw_reg(W_REGISTER + DYNPD, 0x01); 
//	spi_write_buf(W_TX_PAYLOAD, buf, TX_PLOAD_WIDTH);                    // 写数据包到TX FIFO ACK
//	spi_write_buf(W_TX_PAYLOAD_NOACK, buf, TX_PLOAD_WIDTH);                // 写数据包到TX FIFO NO ACK
	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						 		// 5 byte Address width
	spi_rw_reg(W_REGISTER + RF_CH, 40);         					     	// 选择射频通道0x40
	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						 	    // 数据传输率2Mbps(**与24L01不同**),-18dbm TX power	
	spi_rw_reg(W_REGISTER + CONFIG, 0x0e);      					     	// TX_DS IRQ enable CRC使能，16位CRC校验，上电, PTX mode	
  
	//CE = 1;
}

void rx_mode_setting( void)
{

	CE = 0;
	//spi_rw_reg(FLUSH_TX,0x00);
	spi_write_buf(W_REGISTER + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);     	// 写入发送地址
	spi_rw_reg(W_REGISTER + FEATURE, 0x01);      					     	// 使能NO_ACK_TX
	
//	spi_write_buf(W_TX_PAYLOAD, buf, TX_PLOAD_WIDTH);                    // 写数据包到TX FIFO ACK
//	spi_write_buf(W_TX_PAYLOAD_NOACK, buf, TX_PLOAD_WIDTH);                // 写数据包到TX FIFO NO ACK

	spi_rw_reg(W_REGISTER + EN_RXADDR, 0x01);   						 	// 使能接收通道0
	spi_rw_reg(W_REGISTER + RF_CH, 40);         					     	// 选择射频通道0x40	
	spi_rw_reg( RX_PW_P0, TX_PLOAD_WIDTH);									//设置接收通道0负载数据长度
	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						 	    // 数据传输率2Mbps(**与24L01不同**),-18dbm TX power
	spi_rw_reg(W_REGISTER + CONFIG, 0x0f);      					     	// TX_DS IRQ enable CRC使能，16位CRC校验，上电, PTX mode	
  
	//CE = 1;
}

void send_data(uchar *buf,uchar payloadwidth)			   //buf: payload 
{
	spi_write_buf(W_TX_PAYLOAD_NOACK, buf, payloadwidth);                    // 写数据包到TX FIFO ACK
	delay(TIME_500MS);
	CE = 1;
}

