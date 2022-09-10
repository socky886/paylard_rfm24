#include "Si24R1.h"
// #include "spi.h"
// #include "main.h"

extern uchar spi_rw_byte(uchar byte);

//----------------------------   ȫ�ֱ���   ----------------------------------
uchar  TX_ADDRESS[TX_ADR_WIDTH] = {0xE7,0xE7,0xE7,0xE7,0xE7};  // ����һ����̬���͵�ַ
uchar  RX_ADDRESS[TX_ADR_WIDTH] = {0xE7,0xE7,0xE7,0xE7,0xE7};  // ����һ����̬���յ�ַ

uchar TX_BUF[TX_PLOAD_WIDTH] = {0,7,7,5,6,5,2,1};
uchar RX_BUF[TX_PLOAD_WIDTH] = {0};

//=====================================================================================
//������spi_rw_byte()
//���ܣ�����SPIЭ�飬дһ�ֽ����ݵ�Si24R1��ͬʱ��Si24R1����һ�ֽ�
//=====================================================================================
// uchar spi_rw_byte(uchar byte)
// {
// 	uchar status;
	
// 	SPDAT = byte;
	
// 	while( (SPSTAT & 0x80) == 0);	//�ȴ��������
// 	SPSTAT =  0xC0;  				//SPI״̬�Ĵ�������

// 	status = SPDAT;
// 	return status;

// }

//=====================================================================================
//������spi_rw_reg()#
//���ܣ�д����value��reg�Ĵ�����ͬʱ����״̬�Ĵ���ֵ
//=====================================================================================
uchar spi_rw_reg(uchar reg, uchar value)
{
	uchar status;

  	Reset_CSN();// CSN 	= 0;                   	// CSN�õͣ���ʼ��������
  	status = spi_rw_byte(reg);   	// ѡ��Ĵ�����ͬʱ����״̬��
  	spi_rw_byte(value);             // Ȼ��д���ݵ��üĴ���
  	Set_CSN();// CSN 	= 1;                   	// CSN���ߣ��������ݴ���
  	return(status);            		// ����״̬�Ĵ���
}

//=====================================================================================
//������spi_rd_reg()
//���ܣ���reg�Ĵ�����һ���ֽڵ�����
//=====================================================================================
uchar spi_rd_reg(uchar reg)
{
	uchar value;

  	Reset_CSN();// CSN 	= 0;                	// CSN�õͣ���ʼ��������
  	spi_rw_byte(reg);              	// ѡ��Ĵ���
  	value 	= spi_rw_byte(0);      	// Ȼ��ӸüĴ��������� 
  	Set_CSN();// CSN 	= 1;                	// CSN���ߣ��������ݴ���
  	return(value);            		// ���ؼĴ�������
}

//=====================================================================================
//������spi_read_buf()
//���ܣ���reg�Ĵ�������bytes���ֽڣ�ͨ��������ȡ����ͨ������ �� ����/���͵�ַ
//=====================================================================================
uchar spi_read_buf(uchar reg, uchar *pBuf, uchar bytes)
{

	uchar status;
	uchar i; 	

  	Reset_CSN();// CSN 	= 0;                   	 // CSN�õͣ���ʼ��������
  	status  = spi_rw_byte(reg);      // ѡ��Ĵ�����ͬʱ����״̬��
  	for(i = 0; i < bytes; i++)
    {
   		pBuf[i] = spi_rw_byte(0);    // ����ֽڴ�Si24R1����
	}
  	Set_CSN();// CSN 	= 1;                     // CSN���ߣ��������ݴ���
  	return(status);             	 // ����״̬�Ĵ���
}

//=====================================================================================
//������spi_write_buf()
//���ܣ���pBuf�����е�����д�뵽Si24R1��ͨ������д�뷢��ͨ������ �� ����/���͵�ַ
//=====================================================================================
uchar spi_write_buf(uchar reg, uchar *pBuf, uchar bytes)
{
	uchar status, i;
  	Reset_CSN();// CSN 	= 0;                     // CSN�õͣ���ʼ��������
  	status 	= spi_rw_byte(reg);      // ѡ��Ĵ�����ͬʱ����״̬��
  	for(i = 0; i < bytes; i++)
    {
   		spi_rw_byte(pBuf[i]);        // ����ֽ�д��Si24R1
	}
  	Set_CSN();// CSN 	= 1;                     // CSN���ߣ��������ݴ���
  	return(status);             	 // ����״̬�Ĵ���
}

uchar Si24R1_Check(void)
{
	uchar buf[5]={0XFA,0XFA,0XFA,0XFA,0XFA};
	uchar i;
	spi_write_buf(W_REGISTER+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	spi_read_buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)
		if(buf[i]!=0XFA) break;	 							   
	if(i!=5)
		return 1;//���Si24R1����	
	return 0;	 //��⵽Si24R1
}
/**
 * @brief TX No ACK
 * 
 */
void Si24R1_Tx_NoACK_Mode(void)
{
	Reset_CE(); // CE = 0;

	// spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);						  // 5 byte Address width(Ĭ��ֵ)
	// spi_write_buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	  // д�뷢�͵�ַ
	// spi_write_buf(W_REGISTER + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK

	// //	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//ʹ��ͨ��0���Զ�Ӧ��
	// //	spi_rw_reg(W_REGISTER+EN_RXADDR,0x01); 							//ʹ��ͨ��0�Ľ��յ�ַ
	// // spi_rw_reg(W_REGISTER+SETUP_RETR,0x1a);							//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	// // spi_rw_reg(W_REGISTER + FEATURE, 0x04);      					// ʹ�ܶ�̬���س���,Disable command W_TX_PAYLOAD_NOACK
	// spi_rw_reg(W_REGISTER + FEATURE, 0x01);	 // ʹ�ܶ�̬���س���,Enable command W_TX_PAYLOAD_NOACK
	// spi_rw_reg(W_REGISTER + DYNPD, 0x01);	 // ʹ�ܽ��չܵ�0~5��̬���س���(���Ͷ˲���ʡ��)
	// spi_rw_reg(W_REGISTER + RF_CH, 40);		 // 2400MHz - 2525MHz,choose CH 04,��2404MHz
	// spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F); // ���ݴ�����2Mbps(**��24L01��ͬ**),-18dbm TX power
	// spi_rw_reg(W_REGISTER + CONFIG, 0x0e);	 // TX_DS IRQ enable CRCʹ�ܣ�16λCRCУ�飬�ϵ�, PTX mode


	// weijunfeng added:2022-01-14
	spi_write_buf(W_REGISTER+TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);					// д�뷢�͵�ַ
	spi_rw_reg(W_REGISTER+FEATURE, 0x01);								// ʹ��W_TX_PAYLOAD_NOACK ����
	// spi_write_buf(W_TX_PAYLOAD_NOACK, buf, TX_PLOAD_WIDTH); // дFIFO
	spi_rw_reg(W_REGISTER+SETUP_AW, 0x03);								// 5 byte Address width
	spi_rw_reg(W_REGISTER+RF_CH, 0x40);								// ѡ����Ƶͨ��0x40
	spi_rw_reg(W_REGISTER+RF_SETUP, 0x08);								// ���ݴ�����2Mbps
	spi_rw_reg(W_REGISTER+CONFIG, 0x0e);								//����Ϊ����ģʽ��CRC Ϊ2Bytes

	Set_CE(); // CE = 1;

}
/**
 * @brief RX NO ACK
 * 
 */
void Si24R1_Rx_NoACK_Mode(void)
{
	Reset_CE(); // CE = 0;

	// spi_write_buf(W_REGISTER + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //д��Rx�ڵ��ַ
	// spi_rw_reg(RX_PW_P0, TX_PLOAD_WIDTH);							  //���ø��س���

	// //	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//ʹ��ͨ��0���Զ�Ӧ��
	// spi_rw_reg(W_REGISTER + EN_RXADDR, 0x01); //ʹ��ͨ��0�Ľ��յ�ַ
	// spi_rw_reg(W_REGISTER + FEATURE, 0x01);	  // ʹ�ܶ�̬���س���,Disable command W_TX_PAYLOAD_NOACK
	// spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  // 5 byte Address width(Ĭ��ֵ)
	// spi_rw_reg(W_REGISTER + DYNPD, 0x01);	  // ʹ�ܽ��չܵ�0~5��̬���س���(���Ͷ˲���ʡ��)
	// spi_rw_reg(W_REGISTER + RF_CH, 40);		  // 2400MHz - 2525MHz,choose CH 04,��2404MHz
	// spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);  // ���ݴ�����2Mbps(**��24L01��ͬ**),-18dbm TX power
	// spi_rw_reg(W_REGISTER + CONFIG, 0x0f);	  // TX_DS IRQ enable CRCʹ�ܣ�16λCRCУ�飬�ϵ�, PTX mode

	spi_write_buf(W_REGISTER+RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // ���յ�ַ
	spi_rw_reg(W_REGISTER+EN_RXADDR, 0x01);			  // ʹ�ܽ���ͨ��0
	spi_rw_reg(W_REGISTER+RF_CH, 0x40);				  // ѡ����Ƶ�ŵ�
	spi_rw_reg(W_REGISTER+RX_PW_P0, TX_PLOAD_WIDTH);	  //���ý���ͨ��0 �������ݿ��
	spi_rw_reg(W_REGISTER+RF_SETUP, 0x08);				  // ���ݴ�����2Mbps,-18dbm TX power
	spi_rw_reg(W_REGISTER+CONFIG, 0x0f);				  // ����Ϊ���շ���CRC Ϊ2Bytes

	Set_CE(); // CE = 1;
}
//Si24R1 ACK ����ģʽ
// void Si24R1_Tx_ACK_Mode(void)
// {
// 	Reset_CE();// CE = 0;

// 	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						// 5 byte Address width(Ĭ��ֵ)
// 	spi_write_buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);  // д�뷢�͵�ַ
// 	spi_write_buf(W_REGISTER+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); 	//����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	
	
// //	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//ʹ��ͨ��0���Զ�Ӧ��    
// //	spi_rw_reg(W_REGISTER+EN_RXADDR,0x01); 							//ʹ��ͨ��0�Ľ��յ�ַ  
// 	// spi_rw_reg(W_REGISTER+SETUP_RETR,0x1a);							//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
// 	// spi_rw_reg(W_REGISTER + FEATURE, 0x04);      					// ʹ�ܶ�̬���س���,Disable command W_TX_PAYLOAD_NOACK
// 	spi_rw_reg(W_REGISTER + FEATURE, 0x01);      					// ʹ�ܶ�̬���س���,Disable command W_TX_PAYLOAD_NOACK
// 	spi_rw_reg(W_REGISTER + DYNPD, 0x01);      					    // ʹ�ܽ��չܵ�0~5��̬���س���(���Ͷ˲���ʡ��)
// 	spi_rw_reg(W_REGISTER + RF_CH, 40);         					// 2400MHz - 2525MHz,choose CH 04,��2404MHz
// 	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						// ���ݴ�����2Mbps(**��24L01��ͬ**),-18dbm TX power
// 	spi_rw_reg(W_REGISTER + CONFIG, 0x0e);      					// TX_DS IRQ enable CRCʹ�ܣ�16λCRCУ�飬�ϵ�, PTX mode	
  
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
	spi_write_buf(W_REGISTER + TX_ADDR, tx_address, 5);  // д�뷢�͵�ַ,5 bytes
	spi_write_buf(W_REGISTER + RX_ADDR_P0, tx_address, 5);  // д�뷢�͵�ַ,5 bytes
	spi_rw_reg(W_REGISTER+DYNPD,0x01);// enable rx pipe0 dynamic payload
	spi_rw_reg(W_REGISTER+ EN_AA,0x01);// enable pipe0 auto acknowledge
	spi_rw_reg(W_REGISTER+ SETUP_RETR,0x10);// the retry intervil is 500us, the retry time is 5
	spi_rw_reg(W_REGISTER + FEATURE, 0x04); // disable dynamic payload, NOACK=0(enable ACK)
	spi_rw_reg(W_REGISTER + RF_CH, 40); // set channel to 40, the carrier frequency is 2440M
	// spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//ʹ��ͨ��0���Զ�Ӧ�� 
	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0B);   						// ���ݴ�����2Mbps,TX power is 0dbM
	spi_rw_reg(W_REGISTER + CONFIG, 0x0E);   						// enable tx_done,rx_done,max_tx,2bytes CRC, tx mode

}

//Si24R1 ACK ����ģʽ
// void Si24R1_Rx_ACK_Mode(void)
// {
// 	Reset_CE();// CE = 0;

// 	spi_write_buf(W_REGISTER+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); 	//д��Rx�ڵ��ַ	
// 	spi_rw_reg(RX_PW_P0, TX_PLOAD_WIDTH);                           //���ø��س���
	
// //	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//ʹ��ͨ��0���Զ�Ӧ��    
//   	spi_rw_reg(W_REGISTER+EN_RXADDR,0x01); 							//ʹ��ͨ��0�Ľ��յ�ַ  
//    	spi_rw_reg(W_REGISTER + FEATURE, 0x04);      					// ʹ�ܶ�̬���س���,Disable command W_TX_PAYLOAD_NOACK
// 	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						// 5 byte Address width(Ĭ��ֵ)
//   	spi_rw_reg(W_REGISTER + DYNPD, 0x01);      					    // ʹ�ܽ��չܵ�0~5��̬���س���(���Ͷ˲���ʡ��)
//     spi_rw_reg(W_REGISTER + RF_CH, 40);         					// 2400MHz - 2525MHz,choose CH 04,��2404MHz
//   	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						// ���ݴ�����2Mbps(**��24L01��ͬ**),-18dbm TX power
//   	spi_rw_reg(W_REGISTER + CONFIG, 0x0f);      					// TX_DS IRQ enable CRCʹ�ܣ�16λCRCУ�飬�ϵ�, PTX mode	
  
// 	Set_CE();// CE = 1;
// }

void Si24R1_Rx_ACK_Mode(void)
{
// 	Reset_CE();// CE = 0;

// 	spi_write_buf(W_REGISTER+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); 	//д��Rx�ڵ��ַ	
// 	spi_rw_reg(RX_PW_P0, TX_PLOAD_WIDTH);                           //���ø��س���
	
// //	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//ʹ��ͨ��0���Զ�Ӧ��    
//   	spi_rw_reg(W_REGISTER+EN_RXADDR,0x01); 							//ʹ��ͨ��0�Ľ��յ�ַ  
//    	spi_rw_reg(W_REGISTER + FEATURE, 0x04);      					// ʹ�ܶ�̬���س���,Disable command W_TX_PAYLOAD_NOACK
// 	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						// 5 byte Address width(Ĭ��ֵ)
//   	spi_rw_reg(W_REGISTER + DYNPD, 0x01);      					    // ʹ�ܽ��չܵ�0~5��̬���س���(���Ͷ˲���ʡ��)
//     spi_rw_reg(W_REGISTER + RF_CH, 40);         					// 2400MHz - 2525MHz,choose CH 04,��2404MHz
//   	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						// ���ݴ�����2Mbps(**��24L01��ͬ**),-18dbm TX power
//   	spi_rw_reg(W_REGISTER + CONFIG, 0x0f);      					// TX_DS IRQ enable CRCʹ�ܣ�16λCRCУ�飬�ϵ�, PTX mode	
  
// 	Set_CE();// CE = 1;

	uint8_t rx_addr[]={0xE7,0xE7,0xE7,0xE7,0xE7};

	Reset_CE();
	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						// 5 byte Address width(Ĭ��ֵ)
	spi_write_buf(W_REGISTER+RX_ADDR_P0,rx_addr,5); 	//д��Rx�ڵ��ַ
	spi_rw_reg(W_REGISTER + RF_CH, 40); // set channel to 40, the carrier frequency is 2440M
	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0B);   						// ���ݴ�����2Mbps,TX power is 0dbM
	spi_rw_reg(W_REGISTER + CONFIG, 0x0f);      					// TX_DS IRQ enable CRCʹ�ܣ�16λCRCУ�飬�ϵ�, RX mode
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
	// sta=spi_rd_reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	// spi_rw_reg(W_REGISTER+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	// printf("the status is %02x\n");
	spi_rw_reg(FLUSH_RX,0x00);//���RX FIFO�Ĵ��� 
	spi_rw_reg(FLUSH_TX,0x00);//���TX FIFO�Ĵ��� 
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

	// while(ReadIRQ()==1);// while(IRQ == 1);					  //�ȴ��������
	// printf("receive packet\n");
	sta=spi_rd_reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	spi_rw_reg(W_REGISTER+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	printf("the status is %02x\n",sta);
	if(sta&RX_OK)
	{
		printf("receive packet\n");
		spi_rw_reg(FLUSH_RX,0x00);//���RX FIFO�Ĵ��� 
	}
}


//����Si24R1,����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
uchar Si24R1_TxPacket(uchar *txbuf)
{
	uchar sta;
	Reset_CE();// CE = 0;
	//ʹ��NOACKģʽʱ��Ӧʹ������ W_TX_PAYLOAD_NOACK
	spi_write_buf(W_TX_PAYLOAD_NOACK,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  
  	// spi_write_buf(W_TX_PAYLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  
 	Set_CE();// CE = 1;//��������	   
	while(ReadIRQ()==1);// while(IRQ == 1);					  //�ȴ��������

	// Reset_CE();// CE = 0;
	sta = spi_rw_reg(STATUS,0x00);        // ����״̬�Ĵ���	   
	spi_rw_reg(W_REGISTER+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_RT)//�ﵽ����ط�����
	{
		spi_rw_reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_RT; 
	}
	
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}
//����Si24R1,����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
uchar Si24R1_RxPacket(uchar *rxbuf)
{
	uchar sta;		
	Set_CE();// CE = 1;
	while(ReadIRQ()==1);// while(IRQ == 1);					  //�ȴ��������
	sta=spi_rd_reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	spi_rw_reg(W_REGISTER+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		spi_read_buf(R_RX_PAYLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		spi_rw_reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}			


/*
void send_data(uchar *buf,uchar payloadwidth)			   //buf: payload 
{
	spi_write_buf(W_TX_PAYLOAD, buf, payloadwidth);                    // д���ݰ���TX FIFO ACK
//	spi_write_buf(W_TX_PAYLOAD_NOACK, buf, payloadwidth);                    // д���ݰ���TX FIFO ACK
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
