#include "Si24R1.h"
#include "spi.h"
#include "delay.h"

//----------------------------   ȫ�ֱ���   ----------------------------------
uchar code TX_ADDRESS[TX_ADR_WIDTH] = {0xE7,0xE7,0xE7,0xE7,0xA7};  // ����һ����̬���͵�ַ
uchar TX_BUF[TX_PLOAD_WIDTH] = {0,7,32,5,16,5,2,1};

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


void tx_mode_setting( void)
{
	CE = 0;

	//spi_rw_reg(FLUSH_TX,0x00);

	spi_write_buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     		// д�뷢�͵�ַ
	spi_rw_reg(W_REGISTER + FEATURE, 0x05);      					     	// ʹ��NO_ACK_TX
	spi_rw_reg(W_REGISTER + DYNPD, 0x01); 
//	spi_write_buf(W_TX_PAYLOAD, buf, TX_PLOAD_WIDTH);                    // д���ݰ���TX FIFO ACK
//	spi_write_buf(W_TX_PAYLOAD_NOACK, buf, TX_PLOAD_WIDTH);                // д���ݰ���TX FIFO NO ACK
	spi_rw_reg(W_REGISTER + SETUP_AW, 0x03);  						 		// 5 byte Address width
	spi_rw_reg(W_REGISTER + RF_CH, 40);         					     	// ѡ����Ƶͨ��0x40
	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						 	    // ���ݴ�����2Mbps(**��24L01��ͬ**),-18dbm TX power	
	spi_rw_reg(W_REGISTER + CONFIG, 0x0e);      					     	// TX_DS IRQ enable CRCʹ�ܣ�16λCRCУ�飬�ϵ�, PTX mode	
  
	//CE = 1;
}

void rx_mode_setting( void)
{

	CE = 0;
	//spi_rw_reg(FLUSH_TX,0x00);
	spi_write_buf(W_REGISTER + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);     	// д�뷢�͵�ַ
	spi_rw_reg(W_REGISTER + FEATURE, 0x01);      					     	// ʹ��NO_ACK_TX
	
//	spi_write_buf(W_TX_PAYLOAD, buf, TX_PLOAD_WIDTH);                    // д���ݰ���TX FIFO ACK
//	spi_write_buf(W_TX_PAYLOAD_NOACK, buf, TX_PLOAD_WIDTH);                // д���ݰ���TX FIFO NO ACK

	spi_rw_reg(W_REGISTER + EN_RXADDR, 0x01);   						 	// ʹ�ܽ���ͨ��0
	spi_rw_reg(W_REGISTER + RF_CH, 40);         					     	// ѡ����Ƶͨ��0x40	
	spi_rw_reg( RX_PW_P0, TX_PLOAD_WIDTH);									//���ý���ͨ��0�������ݳ���
	spi_rw_reg(W_REGISTER + RF_SETUP, 0x0F);   						 	    // ���ݴ�����2Mbps(**��24L01��ͬ**),-18dbm TX power
	spi_rw_reg(W_REGISTER + CONFIG, 0x0f);      					     	// TX_DS IRQ enable CRCʹ�ܣ�16λCRCУ�飬�ϵ�, PTX mode	
  
	//CE = 1;
}

void send_data(uchar *buf,uchar payloadwidth)			   //buf: payload 
{
	spi_write_buf(W_TX_PAYLOAD_NOACK, buf, payloadwidth);                    // д���ݰ���TX FIFO ACK
	delay(TIME_500MS);
	CE = 1;
}

