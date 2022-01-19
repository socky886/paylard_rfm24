#include "uart.h"



#define ILEN               16               //输出缓冲区的大小
#define OLEN               16               //输出缓冲区的大小

extern idata char    outbuf[OLEN];          //发送缓冲区的存储
extern idata uchar   ostart       ;         //发送缓冲区起始索引
extern idata uchar   oend            ;      //发送缓冲区起始索引
extern bit           sendfull       ;       //标志-：标识发送缓冲区满
extern bit           sendactive     ;       //标志：标识发送器激活

extern idata char    inbuf[ILEN];           //发送缓冲区的存储
extern idata uchar   istart         ;       //发送缓冲区起始索引
extern idata uchar   iend            ;      //发送缓冲区起始索引
extern bit inempty;


//==================================================================
//================          串口初始化        ======================
//==================================================================

void uart_init(void)
{
//	ulong temp1,temp2;

	P3M1 &= 0xFC;
	P3M1 |= 0x02;	//0000 0010  p3.0 RX 高阻输入 / p3.1推挽输出
	
	P3M0 &= 0xFC;
    P3M0 |= 0x05;   //0000 0101
	
	
	PCON = 0x80;   //SMOD  -  -  - GF1 GF0 PD IDL
                   //SMOD串口波特率加倍位，1--加倍

    SCON = 0x50;   //SM0  SM1  SM2 REN  TB8 RB8  TI RI
                   //SM0,SM1 模式选择：00-8位移位寄存器，      01-10位异步(定时器1波特率可调)
                   //                  10-11位异步(f/32或f/64) 11-11位异步(定时器1波特率可调)
                   //SM2 ：            1-只有RB8为1，RI才置位     0-接收到字符RI就置位
                   //REN：             1-允许串行口接收  0-禁止串行口接收
                   //TB8,RB9：         发送/接收的第9位数据
                   //TI：              发送完成中断标志位
                   //RI:               接收完成中断标志位
                   //方式1,禁止接收

    TMOD = 0x20;   //       T1/C1        |         T0/C0
                   //GATE  C/T反  M1  M0   GATE  C/T反   M1  M0
                   //GATE:   1-双重启动控制，TR0/TR1+INT0/INT1   0-TR0/TR1控制
                   //C/T反： 1-Couter   0-Timer
                   //M1,M0:  00-13位定时/计数器  TL低5位，TH高8位
                   //        01-16位定时器
                   //        10-常数自动装入的8位定时器/计数器
                   //        11-仅属于T/C0的两个8为定时器，计数器
                   //定时器1 定时方式2，作为串口波特率时钟

    TH1 = (uchar)( 256 - (XTAL/(16L*T*BAUDRATE)));  //T=6 双倍速 T=12 单倍速
		 //方式1 Baudrate formula:(2^SMOD/32)*(T1溢出率)
		 //T1溢出率 formula： SYSclk/12/(256 - TH1)->SYSclk/(12*(256 - TH1))
	//TH0 = 0;
	/*temp1 = 16L*T*BAUDRATE;	//除数
	temp2 = XTAL%temp1;		//余数
	temp1 = temp1 /2;			//除数的1/2
	

	if(temp2 > temp1 )			//补偿TH1少减的数
	{
		TH1--;
	}
			   */
    TR1 = 1;                                       //启动定时器T1
	ES = 1;
}

//==================================================================
//================        串口中断程序        ======================
//==================================================================

static void com_isr(void) interrupt 4 using 1
{
    char ch0;
    if(TI != 0)
    {
        TI = 0;    
       
        if(ostart != oend)                              //若字符在缓冲区
        {
               
                SBUF     = outbuf[ostart++ & (OLEN-1)]; //发送字符
                sendfull = 0;                           //清零sendfull标志

        }
        else                                            //若所有字符发送完成
        {
                sendactive = 0;                         //清零sendactive
        }
    }
    if(RI)
    {   
        ch0 = SBUF;
        RI = 0;
        if (istart + ILEN != iend) 
        {
          inbuf[iend++ & (ILEN-1)] = ch0;
          inempty = 0;
        }        
    }
}

char _getkey (void)  {
  if (iend == istart)
  {
      inempty = 1;
     
    //itask = os_running_task_id ();    /* set input task number                */
    //os_wait (K_SIG, 0, 0);            /* RTX-51 call: wait for signal         */
    //itask = 0xff;                     /* clear input task number              */
   //   return 0;
  }
  return (inbuf[istart++ & (ILEN-1)]);
}




//=================================================================
//写字符到SBUF或发送缓冲区
//=================================================================
void putbuf(char c)
{
  
	if(!sendfull)                                         //若缓冲区未满则发送
    {
        if(!sendactive)                                   //若发送器未激活
        {
            sendactive = 1;                               //则直接送第一个字符到SBUF，激活发送器
            SBUF       = c;                               //启动发送
         }
        else
        {
          //  ES = 0;                                             //关串口中断
            outbuf[ oend++  & (OLEN-1) ] = c;                   //放字符到发送缓冲区
            if (  ( (oend ^ ostart)  & (OLEN-1))  == 0 )
            {
                sendfull = 1;                                   //缓冲区满，置位sendfull标志
            }                                                   //开串口中断
           // ES = 1;
        }
    }
}

//=========================================
//    printf()使用putchar()输出一个字符
//=========================================
char putchar(char c)
{
       /* if(c == '\n')
        {
              putbuf(0x0D);                                 //在LF前发送CR
        }*/
		
		while(sendfull)
		{
		;
		}
       	if(c == '\n')
        {
           putbuf(0x0D);                                 //在LF前发送CR
        }	   
        putbuf(c);                                          //把字符放入缓冲区
        return(c);

}