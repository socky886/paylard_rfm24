#ifndef _UART_H
#define _UART_H

#include "sys.h"

#define ILEN               16                 //����������Ĵ�С
#define OLEN               16         //����������Ĵ�С

//======================================================
//            �жϷ�ʽ�Ĵ��ڶ���
//======================================================
//#define BAUDRATE          4800       //���ڲ�����
//#define BAUDRATE           9600       //���ڲ�����
//#define BAUDRATE           9600       //���ڲ�����
//#define BAUDRATE           57600       //���ڲ�����
#define BAUDRATE           9600       //���ڲ�����


//��������

void uart_init(void);
//char _getkey (void);
void putbuf(char c);
char putchar(char c);

#endif

