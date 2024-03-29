#include "Si24R1.h"
// #include "spi.h"
// #include "main.h"

extern uchar spi_rw_byte(uchar byte);

//----------------------------   全局变量   ----------------------------------
uchar  TX_ADDRESS[TX_ADR_WIDTH] = {0xE7,0xE7,0xE7,0xE7,0xE7};  // 定义一个静态发送地址
uchar  RX_ADDRESS[TX_ADR_WIDTH] = {0xE7,0xE7,0xE7,0xE7,0xE7};  // 定义一个静态接收地址

uchar TX_BUF[TX_PLOAD_WIDTH] = {0,7,7,5,6,5,2,1};
uchar RX_BUF[TX_PLOAD_WIDTH] = {0};

//=====================================================================================
//函数：spi_rw_byte()
//功能：根据SPI协议，写一字节数据到Si24R1，同时从Si24R1读出一字节
//=====================================================================================
// uchar spi_rw_byte(uchar byte)
// {
// 	uchar status;
	
// 	SPDAT = byte;
	
// 	while( (SPSTAT & 0x80) == 0);	//等待发送完成
// 	SPSTAT =  0xC0;  				//SPI状态寄存器清零

// 	status = SPDAT;
// 	return status;

// }

//=====================================================================================
//函数：spi_rw_reg()#
//功能：写数据value到reg寄存器，同时返回状态寄存器值
//=====================================================================================
uchar spi_rw_reg(uchar reg, uchar value)
{
	uchar status;

  	Reset_CSN();// CSN 	= 0;                   	// CSN置低，开始传输数据
  	status = spi_rw_byte(reg);   	// 选择寄存器，同时返回状态字
  	spi_rw_byte(value);             // 然后写数据到该寄存器
  	Set_CSN();// CSN 	= 1;                   	// CSN拉高，结束数据传输
  	return(status);            		// 返回状态寄存器
}

//=====================================================================================
//函数：spi_rd_reg()
//功能：从reg寄存器读一个字节的数据
//=====================================================================================
uchar spi_rd_reg(uchar reg)
{
	uchar value;

  	Reset_CSN();// CSN 	= 0;                	// CSN置低，开始传输数据
  	spi_rw_byte(reg);              	// 选择寄存器
  	value 	= spi_rw_byte(0);      	// 然后从该寄存器读数据 
  	Set_CSN();// CSN 	= 1;                	// CSN拉高，结束数据传输
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

  	Reset_CSN();// CSN 	= 0;                   	 // CSN置低，开始传输数据
  	status  = spi_rw_byte(reg);      // 选择寄存器，同时返回状态字
  	for(i = 0; i < bytes; i++)
    {
   		pBuf[i] = spi_rw_byte(0);    // 逐个字节从Si24R1读出
	}
  	Set_CSN();// CSN 	= 1;                     // CSN拉高，结束数据传输
  	return(status);             	 // 返回状态寄存器
}

//=====================================================================================
//函数：spi_write_buf()
//功能：把pBuf缓存中的数据写入到Si24R1，通常用来写入发射通道数据 或 接收/发送地址
//=====================================================================================
uchar spi_write_buf(uchar reg, uchar *pBuf, uchar bytes)
{
	uchar status, i;
  	Reset_CSN();// CSN 	= 0;                     // CSN置低，开始传输数据
  	status 	= spi_rw_byte(reg);      // 选择寄存器，同时返回状态字
  	for(i = 0; i < bytes; i++)
    {
   		spi_rw_byte(pBuf[i]);        // 逐个字节写入Si24R1
	}
  	Set_CSN();// CSN 	= 1;                     // CSN拉高，结束数据传输
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
/**
 * @brief TX No ACK
 * 
 */
void Si24R1_Tx_NoACK_Mode(void)
{
	Reset_CE(); // CE = 0;

	// spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);						  // 5 byte Address width(默认值)
	// spi_write_buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	  // 写入发送地址
	// spi_write_buf(W_REGISTER + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK

	// //	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//使能通道0的自动应答
	// //	spi_rw_reg(W_REGISTER+EN_RXADDR,0x01); 							//使能通道0的接收地址
	// // spi_rw_reg(W_REGISTER+SETUP_RETR,0x1a);							//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	// // spi_rw_reg(W_REGISTER + FEATURE, 0x04);      					// 使能动态负载长度,Disable command W_TX_PAYLOAD_NOACK
	// spi_rw_reg(W_REGISTER + FEATURE, 0x01);	 // 使能动态负载长度,Enable command W_TX_PAYLOAD_NOACK
	// spi_rw_reg(W_REGISTER + DYNPD, 0x01);	 // 使能接收管道0~5动态负载长度(发送端不可省略)
	// spi_rw_reg(W_REGISTER + RF_CH, 40);		 // 2400MHz - 2525MHz,choose CH 04,即2404MHz
	// spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F); // 数据传输率2Mbps(**与24L01不同**),-18dbm TX power
	// spi_rw_reg(W_REGISTER + CONFIG, 0x0e);	 // TX_DS IRQ enable CRC使能，16位CRC校验，上电, PTX mode


	// weijunfeng added:2022-01-14
	spi_write_buf(W_REGISTER+TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);					// 写入发送地址
	spi_rw_reg(W_REGISTER+FEATURE, 0x01);								// 使能W_TX_PAYLOAD_NOACK 命令
	// spi_write_buf(W_TX_PAYLOAD_NOACK, buf, TX_PLOAD_WIDTH); // 写FIFO
	spi_rw_reg(W_REGISTER+SETUP_AW, 0x03);								// 5 byte Address width
	spi_rw_reg(W_REGISTER+RF_CH, 0x40);								// 选择射频通道0x40
	spi_rw_reg(W_REGISTER+RF_SETUP, 0x08);								// 数据传输率2Mbps
	spi_rw_reg(W_REGISTER+CONFIG, 0x0e);								//配置为发射模式、CRC 为2Bytes

	Set_CE(); // CE = 1;

}
/**
 * @brief RX NO ACK
 * 
 */
void Si24R1_Rx_NoACK_Mode(void)
{
	Reset_CE(); // CE = 0;

	// spi_write_buf(W_REGISTER + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //写入Rx节点地址
	// spi_rw_reg(RX_PW_P0, TX_PLOAD_WIDTH);							  //设置负载长度

	// //	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//使能通道0的自动应答
	// spi_rw_reg(W_REGISTER + EN_RXADDR, 0x01); //使能通道0的接收地址
	// spi_rw_reg(W_REGISTER + FEATURE, 0x01);	  // 使能动态负载长度,Disable command W_TX_PAYLOAD_NOACK
	// spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  // 5 byte Address width(默认值)
	// spi_rw_reg(W_REGISTER + DYNPD, 0x01);	  // 使能接收管道0~5动态负载长度(发送端不可省略)
	// spi_rw_reg(W_REGISTER + RF_CH, 40);		  // 2400MHz - 2525MHz,choose CH 04,即2404MHz
	// spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);  // 数据传输率2Mbps(**与24L01不同**),-18dbm TX power
	// spi_rw_reg(W_REGISTER + CONFIG, 0x0f);	  // TX_DS IRQ enable CRC使能，16位CRC校验，上电, PTX mode

	spi_write_buf(W_REGISTER+RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 接收地址
	spi_rw_reg(W_REGISTER+EN_RXADDR, 0x01);			  // 使能接收通道0
	spi_rw_reg(W_REGISTER+RF_CH, 0x40);				  // 选择射频信道
	spi_rw_reg(W_REGISTER+RX_PW_P0, TX_PLOAD_WIDTH);	  //设置接收通道0 负载数据宽度
	spi_rw_reg(W_REGISTER+RF_SETUP, 0x08);				  // 数据传输率2Mbps,-18dbm TX power
	spi_rw_reg(W_REGISTER+CONFIG, 0x0f);				  // 配置为接收方、CRC 为2Bytes

	Set_CE(); // CE = 1;
}
//Si24R1 ACK 发射模式
// void Si24R1_Tx_ACK_Mode(void)
// {
// 	Reset_CE();// CE = 0;

// 	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						// 5 byte Address width(默认值)
// 	spi_write_buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);  // 写入发送地址
// 	spi_write_buf(W_REGISTER+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); 	//设置TX节点地址,主要为了使能ACK	
	
// //	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//使能通道0的自动应答    
// //	spi_rw_reg(W_REGISTER+EN_RXADDR,0x01); 							//使能通道0的接收地址  
// 	// spi_rw_reg(W_REGISTER+SETUP_RETR,0x1a);							//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
// 	// spi_rw_reg(W_REGISTER + FEATURE, 0x04);      					// 使能动态负载长度,Disable command W_TX_PAYLOAD_NOACK
// 	spi_rw_reg(W_REGISTER + FEATURE, 0x01);      					// 使能动态负载长度,Disable command W_TX_PAYLOAD_NOACK
// 	spi_rw_reg(W_REGISTER + DYNPD, 0x01);      					    // 使能接收管道0~5动态负载长度(发送端不可省略)
// 	spi_rw_reg(W_REGISTER + RF_CH, 40);         					// 2400MHz - 2525MHz,choose CH 04,即2404MHz
// 	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						// 数据传输率2Mbps(**与24L01不同**),-18dbm TX power
// 	spi_rw_reg(W_REGISTER + CONFIG, 0x0e);      					// TX_DS IRQ enable CRC使能，16位CRC校验，上电, PTX mode	
  
// 	Set_CE();// CE = 1;
// }

/**
 * @brief set TX mode
 *  1: NOACK=0
 *  2: Disable Dynamic payload
 * 
 */
void Si24R1_Tx_ACK_Mode(void)
{
	unsigned char tx_address[]={0xE7,0xE7,0xE7,0xE7,0xE7};
	Reset_CE();

	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  // setup the address width: 5bytes
	spi_write_buf(W_REGISTER + TX_ADDR, tx_address, 5);  // 写入发送地址,5 bytes
	spi_write_buf(W_REGISTER + RX_ADDR_P0, tx_address, 5);  // 写入发送地址,5 bytes
	spi_rw_reg(W_REGISTER+DYNPD,0x01);// enable rx pipe0 dynamic payload
	spi_rw_reg(W_REGISTER+ EN_AA,0x01);// enable pipe0 auto acknowledge
	spi_rw_reg(W_REGISTER+ SETUP_RETR,0x10);// the retry intervil is 500us, the retry time is 5
	spi_rw_reg(W_REGISTER + FEATURE, 0x04); // disable dynamic payload, NOACK=0(enable ACK)
	spi_rw_reg(W_REGISTER + RF_CH, 40); // set channel to 40, the carrier frequency is 2440M
	// spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//使能通道0的自动应答 
	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0B);   						// 数据传输率2Mbps,TX power is 0dbM
	spi_rw_reg(W_REGISTER + CONFIG, 0x0E);   						// enable tx_done,rx_done,max_tx,2bytes CRC, tx mode

}

//Si24R1 ACK 接收模式
// void Si24R1_Rx_ACK_Mode(void)
// {
// 	Reset_CE();// CE = 0;

// 	spi_write_buf(W_REGISTER+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); 	//写入Rx节点地址	
// 	spi_rw_reg(RX_PW_P0, TX_PLOAD_WIDTH);                           //设置负载长度
	
// //	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//使能通道0的自动应答    
//   	spi_rw_reg(W_REGISTER+EN_RXADDR,0x01); 							//使能通道0的接收地址  
//    	spi_rw_reg(W_REGISTER + FEATURE, 0x04);      					// 使能动态负载长度,Disable command W_TX_PAYLOAD_NOACK
// 	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						// 5 byte Address width(默认值)
//   	spi_rw_reg(W_REGISTER + DYNPD, 0x01);      					    // 使能接收管道0~5动态负载长度(发送端不可省略)
//     spi_rw_reg(W_REGISTER + RF_CH, 40);         					// 2400MHz - 2525MHz,choose CH 04,即2404MHz
//   	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						// 数据传输率2Mbps(**与24L01不同**),-18dbm TX power
//   	spi_rw_reg(W_REGISTER + CONFIG, 0x0f);      					// TX_DS IRQ enable CRC使能，16位CRC校验，上电, PTX mode	
  
// 	Set_CE();// CE = 1;
// }

void Si24R1_Rx_ACK_Mode(void)
{
// 	Reset_CE();// CE = 0;

// 	spi_write_buf(W_REGISTER+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); 	//写入Rx节点地址	
// 	spi_rw_reg(RX_PW_P0, TX_PLOAD_WIDTH);                           //设置负载长度
	
// //	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//使能通道0的自动应答    
//   	spi_rw_reg(W_REGISTER+EN_RXADDR,0x01); 							//使能通道0的接收地址  
//    	spi_rw_reg(W_REGISTER + FEATURE, 0x04);      					// 使能动态负载长度,Disable command W_TX_PAYLOAD_NOACK
// 	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						// 5 byte Address width(默认值)
//   	spi_rw_reg(W_REGISTER + DYNPD, 0x01);      					    // 使能接收管道0~5动态负载长度(发送端不可省略)
//     spi_rw_reg(W_REGISTER + RF_CH, 40);         					// 2400MHz - 2525MHz,choose CH 04,即2404MHz
//   	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						// 数据传输率2Mbps(**与24L01不同**),-18dbm TX power
//   	spi_rw_reg(W_REGISTER + CONFIG, 0x0f);      					// TX_DS IRQ enable CRC使能，16位CRC校验，上电, PTX mode	
  
// 	Set_CE();// CE = 1;

	uint8_t rx_addr[]={0xE7,0xE7,0xE7,0xE7,0xE7};

	Reset_CE();
	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						// 5 byte Address width(默认值)
	spi_write_buf(W_REGISTER+RX_ADDR_P0,rx_addr,5); 	//写入Rx节点地址
	spi_rw_reg(W_REGISTER + RF_CH, 40); // set channel to 40, the carrier frequency is 2440M
	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0B);   						// 数据传输率2Mbps,TX power is 0dbM
	spi_rw_reg(W_REGISTER + CONFIG, 0x0f);      					// TX_DS IRQ enable CRC使能，16位CRC校验，上电, RX mode
	spi_rw_reg(W_REGISTER+ FEATURE,0x04); // enable global dynamic payload
	spi_rw_reg(W_REGISTER+DYNPD,0x01);// enable rx pipe 0 dynamic payload
	spi_rw_reg(W_REGISTER+EN_RXADDR,0x01);// enable rx address of pipe0
	Set_CE();
}

void Si24R1_Rx_Direct(void)
{
	uint8_t rx_addr[]={0xE7,0xE7,0xE7,0xE7,0xE7};
	uint8_t sta;
	Reset_CE();
	// sta=spi_rd_reg(STATUS);  //读取状态寄存器的值    	 
	// spi_rw_reg(W_REGISTER+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	// printf("the status is %02x\n");
	spi_rw_reg(FLUSH_RX,0x00);//清除RX FIFO寄存器 
	spi_rw_reg(FLUSH_TX,0x00);//清除TX FIFO寄存器 
	// set the tx and rx address width to 3bytes
	spi_rw_reg(W_REGISTER + SETUP_AW, 0x01); 
	// write the rx address of pipe0
	spi_write_buf(W_REGISTER+RX_ADDR_P0,rx_addr,3); 
	// enable rx pipe0
	spi_rw_reg(W_REGISTER+EN_RXADDR,0x03F); 
	// set channel
	spi_rw_reg(W_REGISTER + RF_CH, 40); 

	// set the payload width of the rx pipe0
	spi_rw_reg(W_REGISTER + RX_PW_P0, 10); 
	
	// set the datarate to 2M
	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0B);//symbol rate and tx power
	// disable CRC, power up mode,enter rx mode
	spi_rw_reg(W_REGISTER + CONFIG, 0x03);
	// enable no ack, disable dynamic
	// spi_rw_reg(W_REGISTER + FEATURE, 0x01);
    
	Set_CE();

	// while(ReadIRQ()==1);// while(IRQ == 1);					  //等待接收完成
	// printf("receive packet\n");
	sta=spi_rd_reg(STATUS);  //读取状态寄存器的值    	 
	spi_rw_reg(W_REGISTER+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	printf("the status is %02x\n",sta);
	if(sta&RX_OK)
	{
		printf("receive packet\n");
		spi_rw_reg(FLUSH_RX,0x00);//清除RX FIFO寄存器 
	}
}


//启动Si24R1,发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
uchar Si24R1_TxPacket(uchar *txbuf)
{
	uchar sta;
	Reset_CE();// CE = 0;
	//使用NOACK模式时，应使用命令 W_TX_PAYLOAD_NOACK
	spi_write_buf(W_TX_PAYLOAD_NOACK,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  
  	// spi_write_buf(W_TX_PAYLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  
 	Set_CE();// CE = 1;//启动发送	   
	while(ReadIRQ()==1);// while(IRQ == 1);					  //等待发送完成

	// Reset_CE();// CE = 0;
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
//启动Si24R1,接收一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
uchar Si24R1_RxPacket(uchar *rxbuf)
{
	uchar sta;		
	Set_CE();// CE = 1;
	while(ReadIRQ()==1);// while(IRQ == 1);					  //等待接收完成
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

void NRF24L01_init_cw(uint8_t channel) 
{
        Reset_CE();// CE = 0;
        spi_rw_reg(W_REGISTER+CONFIG,0x12);        // on, no crc, int on RX/TX done
        spi_rw_reg(W_REGISTER+EN_AA,0x00);         // no auto-acknowledge
        spi_rw_reg(W_REGISTER+EN_RXADDR,0x00);     // no RX
        spi_rw_reg(W_REGISTER+SETUP_AW,0x02);      // 4-byte address
        spi_rw_reg(W_REGISTER+SETUP_RETR, 0x00);   // no auto-retransmit
        // spi_rw_reg(W_REGISTER+RF_SETUP,0x06|(1<<7)|(1<<4));      // 1MBps at 0dBm, and cw ,pll lock
		spi_rw_reg(W_REGISTER+RF_SETUP,0x98|0x07);      // 2MBps at 7dBm, and cw ,pll lock
        spi_rw_reg(W_REGISTER+STATUS,0x3E);        // clear various flags
        spi_rw_reg(W_REGISTER+DYNPD,0x00);         // no dynamic payloads
        spi_rw_reg(W_REGISTER+FEATURE, 0x00);      // no features
        spi_rw_reg(W_REGISTER+RX_PW_P0,32);        // always RX 32 bytes
        spi_rw_reg(W_REGISTER+EN_RXADDR, 0x01);    // RX on pipe 0

        spi_rw_reg(W_REGISTER+RF_CH,channel);
        spi_rw_reg(W_REGISTER+CONFIG,0x12);// TX on

        Set_CE();// CE = 1;
}
