#ifndef _UART_H
#define _UART_H

#include "sys.h"

#define ILEN               16                 //输出缓冲区的大小
#define OLEN               16         //输出缓冲区的大小

//======================================================
//            中断方式的串口定义
//======================================================
//#define BAUDRATE          4800       //串口波特率
//#define BAUDRATE           9600       //串口波特率
//#define BAUDRATE           9600       //串口波特率
//#define BAUDRATE           57600       //串口波特率
#define BAUDRATE           9600       //串口波特率


//函数声明

void uart_init(void);
//char _getkey (void);
void putbuf(char c);
char putchar(char c);

#endif

