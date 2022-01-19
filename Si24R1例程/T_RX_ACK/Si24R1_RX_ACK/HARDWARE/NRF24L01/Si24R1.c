#include "Si24R1.h"
#include "spi.h"


//----------------------------   全局变量   ----------------------------------
uchar code TX_ADDRESS[TX_ADR_WIDTH] = {0xE7,0xE7,0xE7,0xE7,0xE7};  // 定义一个静态发送地址
uchar code RX_ADDRESS[TX_ADR_WIDTH] = {0xE7,0xE7,0xE7,0xE7,0xE7};  // 定义一个静态接收地址

uchar TX_BUF[TX_PLOAD_WIDTH] = {0,7,7,5,8,5,2,1};
uchar RX_BUF[TX_PLOAD_WIDTH] = {0};

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

uchar Si24R1_Check(void)
{
	uchar buf[5]={0XFA,0XFA,0XFA,0XFA,0XFA};
	uchar i;
	spi_write_buf(W_REGISTER+TX_ADDR,buf,5);//写入5个字节的地址.	
	spi_read_buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)
		if(buf[i]!=0XFA) break;	 							   
	if(i!=5)
		return 1;//检测Si24R1错误	
	return 0;	 //检测到Si24R1
}

	
	


//Si24R1 ACK 发射模式
void Si24R1_Tx_ACK_Mode(void)
{
	CE = 0;

	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						// 5 byte Address width(默认值)
	spi_write_buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);  // 写入发送地址
	spi_write_buf(W_REGISTER+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); 	//设置TX节点地址,主要为了使能ACK	
	
//	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//使能通道0的自动应答    
//	spi_rw_reg(W_REGISTER+EN_RXADDR,0x01); 							//使能通道0的接收地址  
	spi_rw_reg(W_REGISTER+SETUP_RETR,0x1a);							//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	spi_rw_reg(W_REGISTER + FEATURE, 0x04);      					// 使能动态负载长度,Disable command W_TX_PAYLOAD_NOACK
	spi_rw_reg(W_REGISTER + DYNPD, 0x01);      					    // 使能接收管道0~5动态负载长度(发送端不可省略)
	spi_rw_reg(W_REGISTER + RF_CH, 40);         					// 2400MHz - 2525MHz,choose CH 04,即2404MHz
	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						// 数据传输率2Mbps(**与24L01不同**),-18dbm TX power
	spi_rw_reg(W_REGISTER + CONFIG, 0x0e);      					// TX_DS IRQ enable CRC使能，16位CRC校验，上电, PTX mode	
  
	CE = 1;
}
//Si24R1 ACK 接收模式
void Si24R1_Rx_ACK_Mode(void)
{
	CE = 0;

	spi_write_buf(W_REGISTER+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); 	//写入Rx节点地址	
	spi_rw_reg(RX_PW_P0, TX_PLOAD_WIDTH);                           //设置负载长度
	
//	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//使能通道0的自动应答    
  	spi_rw_reg(W_REGISTER+EN_RXADDR,0x01); 							//使能通道0的接收地址  
   	spi_rw_reg(W_REGISTER + FEATURE, 0x04);      					// 使能动态负载长度,Disable command W_TX_PAYLOAD_NOACK
	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						// 5 byte Address width(默认值)
  	spi_rw_reg(W_REGISTER + DYNPD, 0x01);      					    // 使能接收管道0~5动态负载长度(发送端不可省略)
    spi_rw_reg(W_REGISTER + RF_CH, 40);         					// 2400MHz - 2525MHz,choose CH 04,即2404MHz
  	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						// 数据传输率2Mbps(**与24L01不同**),-18dbm TX power
  	spi_rw_reg(W_REGISTER + CONFIG, 0x0f);      					// TX_DS IRQ enable CRC使能，16位CRC校验，上电, PTX mode	
  
	CE = 1;
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
uchar Si24R1_TxPacket(uchar *txbuf)
{
	uchar sta;
	CE = 0;
	//使用NOACK模式时，应使用命令 W_TX_PAYLOAD_NOACK
//	spi_write_buf(W_TX_PAYLOAD_NOACK,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  
  	spi_write_buf(W_TX_PAYLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  
 	CE=1;//启动发送	   
	while(IRQ == 1);					  		 //等待发送完成
	sta = spi_rw_reg(STATUS,0x00);        // 返回状态寄存器	   
	spi_rw_reg(W_REGISTER+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_RT)//达到最大重发次数
	{
		spi_rw_reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_RT; 
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
uchar Si24R1_RxPacket(uchar *rxbuf)
{
	uchar sta;
	CE = 1;		//此句不可缺少
	sta=spi_rd_reg(STATUS);  //读取状态寄存器的值    	 
	spi_rw_reg(W_REGISTER+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		spi_read_buf(R_RX_PAYLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		spi_rw_reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//没收到任何数据
}			


/*
void send_data(uchar *buf,uchar payloadwidth)			   //buf: payload 
{
	spi_write_buf(W_TX_PAYLOAD, buf, payloadwidth);                    // 写数据包到TX FIFO ACK
//	spi_write_buf(W_TX_PAYLOAD_NOACK, buf, payloadwidth);                    // 写数据包到TX FIFO ACK
	CE = 1;
	
}
*/