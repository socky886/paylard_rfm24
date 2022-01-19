#ifndef _SYS_H
#define _SYS_H

#include <STC12LE5628AD.h>

//单片机的相关配置，宏定义
//变量简写定义

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long


//
//单片机时钟定义
//
#define T				12


//晶振频率 单位HZ
//#define XTAL               33868800      
#define XTAL               33177600      
//#define XTAL               22118400      
//#define XTAL               16000000      
//#define XTAL                 12000000     //串口波特率 支持4800bps，不支持9600bps，误差太大 
//#define XTAL               11059200      


#endif


