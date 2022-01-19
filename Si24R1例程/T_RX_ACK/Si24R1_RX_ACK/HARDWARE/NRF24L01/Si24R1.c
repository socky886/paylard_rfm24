#include "Si24R1.h"
#include "spi.h"


//----------------------------   ȫ�ֱ���   ----------------------------------
uchar code TX_ADDRESS[TX_ADR_WIDTH] = {0xE7,0xE7,0xE7,0xE7,0xE7};  // ����һ����̬���͵�ַ
uchar code RX_ADDRESS[TX_ADR_WIDTH] = {0xE7,0xE7,0xE7,0xE7,0xE7};  // ����һ����̬���յ�ַ

uchar TX_BUF[TX_PLOAD_WIDTH] = {0,7,7,5,8,5,2,1};
uchar RX_BUF[TX_PLOAD_WIDTH] = {0};

//=====================================================================================
//������spi_rw_byte()
//���ܣ�����SPIЭ�飬дһ�ֽ����ݵ�Si24R1��ͬʱ��Si24R1����һ�ֽ�
//=====================================================================================
uchar spi_rw_byte(uchar byte)
{
	uchar status;
	
	SPDAT = byte;
	
	while( (SPSTAT & 0x80) == 0);	//�ȴ��������
	SPSTAT =  0xC0;  				//SPI״̬�Ĵ�������

	status = SPDAT;
	return status;

}

//=====================================================================================
//������spi_rw_reg()
//���ܣ�д����value��reg�Ĵ�����ͬʱ����״̬�Ĵ���ֵ
//=====================================================================================
uchar spi_rw_reg(uchar reg, uchar value)
{
	uchar status;

  	CSN 	= 0;                   	// CSN�õͣ���ʼ��������
  	status = spi_rw_byte(reg);   	// ѡ��Ĵ�����ͬʱ����״̬��
  	spi_rw_byte(value);             // Ȼ��д���ݵ��üĴ���
  	CSN 	= 1;                   	// CSN���ߣ��������ݴ���
  	return(status);            		// ����״̬�Ĵ���
}

//=====================================================================================
//������spi_rd_reg()
//���ܣ���reg�Ĵ�����һ���ֽڵ�����
//=====================================================================================
uchar spi_rd_reg(uchar reg)
{
	uchar value;

  	CSN 	= 0;                	// CSN�õͣ���ʼ��������
  	spi_rw_byte(reg);              	// ѡ��Ĵ���
  	value 	= spi_rw_byte(0);      	// Ȼ��ӸüĴ��������� 
  	CSN 	= 1;                	// CSN���ߣ��������ݴ���
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

  	CSN 	= 0;                   	 // CSN�õͣ���ʼ��������
  	status  = spi_rw_byte(reg);      // ѡ��Ĵ�����ͬʱ����״̬��
  	for(i = 0; i < bytes; i++)
    {
   		pBuf[i] = spi_rw_byte(0);    // ����ֽڴ�Si24R1����
	}
  	CSN 	= 1;                     // CSN���ߣ��������ݴ���
  	return(status);             	 // ����״̬�Ĵ���
}

//=====================================================================================
//������spi_write_buf()
//���ܣ���pBuf�����е�����д�뵽Si24R1��ͨ������д�뷢��ͨ������ �� ����/���͵�ַ
//=====================================================================================
uchar spi_write_buf(uchar reg, uchar *pBuf, uchar bytes)
{
	uchar status, i;
  	CSN 	= 0;                     // CSN�õͣ���ʼ��������
  	status 	= spi_rw_byte(reg);      // ѡ��Ĵ�����ͬʱ����״̬��
  	for(i = 0; i < bytes; i++)
    {
   		spi_rw_byte(pBuf[i]);        // ����ֽ�д��Si24R1
	}
  	CSN 	= 1;                     // CSN���ߣ��������ݴ���
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

	
	


//Si24R1 ACK ����ģʽ
void Si24R1_Tx_ACK_Mode(void)
{
	CE = 0;

	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						// 5 byte Address width(Ĭ��ֵ)
	spi_write_buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);  // д�뷢�͵�ַ
	spi_write_buf(W_REGISTER+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); 	//����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	
	
//	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//ʹ��ͨ��0���Զ�Ӧ��    
//	spi_rw_reg(W_REGISTER+EN_RXADDR,0x01); 							//ʹ��ͨ��0�Ľ��յ�ַ  
	spi_rw_reg(W_REGISTER+SETUP_RETR,0x1a);							//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	spi_rw_reg(W_REGISTER + FEATURE, 0x04);      					// ʹ�ܶ�̬���س���,Disable command W_TX_PAYLOAD_NOACK
	spi_rw_reg(W_REGISTER + DYNPD, 0x01);      					    // ʹ�ܽ��չܵ�0~5��̬���س���(���Ͷ˲���ʡ��)
	spi_rw_reg(W_REGISTER + RF_CH, 40);         					// 2400MHz - 2525MHz,choose CH 04,��2404MHz
	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						// ���ݴ�����2Mbps(**��24L01��ͬ**),-18dbm TX power
	spi_rw_reg(W_REGISTER + CONFIG, 0x0e);      					// TX_DS IRQ enable CRCʹ�ܣ�16λCRCУ�飬�ϵ�, PTX mode	
  
	CE = 1;
}
//Si24R1 ACK ����ģʽ
void Si24R1_Rx_ACK_Mode(void)
{
	CE = 0;

	spi_write_buf(W_REGISTER+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); 	//д��Rx�ڵ��ַ	
	spi_rw_reg(RX_PW_P0, TX_PLOAD_WIDTH);                           //���ø��س���
	
//	spi_rw_reg(W_REGISTER+EN_AA,0x01);     							//ʹ��ͨ��0���Զ�Ӧ��    
  	spi_rw_reg(W_REGISTER+EN_RXADDR,0x01); 							//ʹ��ͨ��0�Ľ��յ�ַ  
   	spi_rw_reg(W_REGISTER + FEATURE, 0x04);      					// ʹ�ܶ�̬���س���,Disable command W_TX_PAYLOAD_NOACK
	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						// 5 byte Address width(Ĭ��ֵ)
  	spi_rw_reg(W_REGISTER + DYNPD, 0x01);      					    // ʹ�ܽ��չܵ�0~5��̬���س���(���Ͷ˲���ʡ��)
    spi_rw_reg(W_REGISTER + RF_CH, 40);         					// 2400MHz - 2525MHz,choose CH 04,��2404MHz
  	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						// ���ݴ�����2Mbps(**��24L01��ͬ**),-18dbm TX power
  	spi_rw_reg(W_REGISTER + CONFIG, 0x0f);      					// TX_DS IRQ enable CRCʹ�ܣ�16λCRCУ�飬�ϵ�, PTX mode	
  
	CE = 1;
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
uchar Si24R1_TxPacket(uchar *txbuf)
{
	uchar sta;
	CE = 0;
	//ʹ��NOACKģʽʱ��Ӧʹ������ W_TX_PAYLOAD_NOACK
//	spi_write_buf(W_TX_PAYLOAD_NOACK,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  
  	spi_write_buf(W_TX_PAYLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  
 	CE=1;//��������	   
	while(IRQ == 1);					  		 //�ȴ��������
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
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
uchar Si24R1_RxPacket(uchar *rxbuf)
{
	uchar sta;
	CE = 1;		//�˾䲻��ȱ��
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