#ifndef _SI24R1_H
#define _SI24R1_H

// #include "sys.h"
// typedef unsigned char uchar;
#include "main.h"


#define TX_WIDTH        32

#define TX_ADR_WIDTH   	5  		// 5字节宽度的发送/接收地址
#define RX_ADR_WIDTH   	5  		// 5字节宽度的发送/接收地址

#define TX_PLOAD_WIDTH	8		//用户数据宽度
#define RX_PLOAD_WIDTH	14		//用户数据宽度


//全局变量声明
extern uchar TX_ADDRESS[TX_ADR_WIDTH];  // 定义一个静态发送地址
extern uchar TX_BUF[TX_PLOAD_WIDTH];
extern uchar RX_BUF[TX_PLOAD_WIDTH];
//Define interface to Si24R1


// SPI(Si24R1) commands
#define R_REGISTER          0x00  // Define read command to register
#define W_REGISTER          0x20  // Define write command to register
#define R_RX_PAYLOAD        0x61  // Define RX payload register address
#define W_TX_PAYLOAD        0xA0  // Define TX payload register address
#define FLUSH_TX            0xE1  // Define flush TX register command
#define FLUSH_RX            0xE2  // Define flush RX register command
#define REUSE_TX_PL         0xE3  // Define reuse TX payload register command
#define R_RX_PL_WID         0x60  // Define Read RX payload width for the R_RX_PAYLOAD in the RX FIFO
#define W_ACK_PAYLOAD       0xA8  // Define Write Payload to be transmitted together with ACK packet on PIPExxx 
                                  // xxx valid in the range from 000 to 101)
#define W_TX_PAYLOAD_NOACK  0xB0  // Define Write TX Disables AUTOACK on this specific packet


#define NOP                 0xFF  // Define No Operation, might be used to read status register
 


// SPI(Si24R1) registers(addresses)
#define CONFIG      0x00  // 'Config' register address
#define EN_AA       0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02  // 'Enabled RX addresses' register address
#define SETUP_AW    0x03  // 'Setup address width' register address
#define SETUP_RETR  0x04  // 'Setup Auto. Retrans' register address
#define RF_CH       0x05  // 'RF channel' register address
#define RF_SETUP    0x06  // 'RF setup' register address
#define STATUS      0x07  // 'Status' register address

#define MAX_RT		0x10  //最大重发次数中断位
#define TX_OK   	0x20  //TX发送完成中断
#define RX_OK   	0x40  //接收到数据中断

#define OBSERVE_TX  0x08  // 'Observe TX' register address
#define CD          0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0  0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F  // 'RX address pipe5' register address
#define TX_ADDR     0x10  // 'TX address' register address
#define RX_PW_P0    0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17  // 'FIFO Status Register' register address
#define DYNPD       0x1C  // 'Enable dynamic paload length' register address
#define FEATURE     0x1D  //  Feature Register address

//函数声明
// uchar spi_rw_byte(uchar byte);
uchar spi_rw_reg(uchar reg, uchar value);
uchar spi_rd_reg(uchar reg);
uchar spi_read_buf(uchar reg, uchar *pBuf, uchar bytes);
uchar spi_write_buf(uchar reg, uchar *pBuf, uchar bytes);

uchar Si24R1_Check(void);

void Si24R1_Tx_NoACK_Mode(void);
void Si24R1_Rx_NoACK_Mode(void);
void Si24R1_Tx_ACK_Mode(void);
void Si24R1_Rx_ACK_Mode(void);

uchar Si24R1_TxPacket(uchar *txbuf);
uchar Si24R1_RxPacket(uchar *rxbuf);

void send_data(uchar *buf,uchar payloadwidth);

void Si24R1_Rx_Direct(void);

#endif

