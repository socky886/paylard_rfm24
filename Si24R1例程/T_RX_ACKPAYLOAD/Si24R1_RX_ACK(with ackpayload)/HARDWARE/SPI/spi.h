#ifndef _SPI_H
#define _SPI_H

#include "sys.h"

//SPI CLK     00: CPU_CLK/4    01  CPU_CLK/16   10  CPU_CLK/64  11  CPU_CLK/128
//             收到1次       |         隔一次收一次             |  全部收到
// 33.1776MHz  8.2944MHz     |    2.0736MHz       0.5184MHz     |   0.2592MHz

// 22.1184MHz  5.5296MHz     |    1.3824MHz       0.3456MHz     |   0.1728MHz

// 11.0596MHz  |2.7649MHz         0.6913MHz       0.1728MHz     |   0.0864MHz


#define SPI_CLK  0
//==============================================================0=======
//                      Define SPI pins(Si24R1)
//======================================================================

sbit SCK  = P1^7;  // Master Out, Slave In pin (output to Si24R1)
sbit MISO = P1^6;  // Master In, Slave Out pin (input from Si24R1)
sbit MOSI = P1^5;  // Serial Clock pin, (output to Si24R1)
sbit CSN  = P1^4;  // Slave Select pin, (output to  Si24R1)
 
sbit CE   = P1^3;  // Chip Enable pin signal (output to Si24R1)
sbit IRQ  = P3^2;  // Interrupt signal,(input  from Si24R1)	
sbit VSS20 = P3^7;	  //TEST PIN, SI24R2 VSS20 

void SPI_Init(void);

#endif

