#include "uart.h"



#define ILEN               16               //����������Ĵ�С
#define OLEN               16               //����������Ĵ�С

extern idata char    outbuf[OLEN];          //���ͻ������Ĵ洢
extern idata uchar   ostart       ;         //���ͻ�������ʼ����
extern idata uchar   oend            ;      //���ͻ�������ʼ����
extern bit           sendfull       ;       //��־-����ʶ���ͻ�������
extern bit           sendactive     ;       //��־����ʶ����������

extern idata char    inbuf[ILEN];           //���ͻ������Ĵ洢
extern idata uchar   istart         ;       //���ͻ�������ʼ����
extern idata uchar   iend            ;      //���ͻ�������ʼ����
extern bit inempty;


//==================================================================
//================          ���ڳ�ʼ��        ======================
//==================================================================

void uart_init(void)
{
//	ulong temp1,temp2;

	P3M1 &= 0xFC;
	P3M1 |= 0x02;	//0000 0010  p3.0 RX �������� / p3.1�������
	
	P3M0 &= 0xFC;
    P3M0 |= 0x05;   //0000 0101
	
	
	PCON = 0x80;   //SMOD  -  -  - GF1 GF0 PD IDL
                   //SMOD���ڲ����ʼӱ�λ��1--�ӱ�

    SCON = 0x50;   //SM0  SM1  SM2 REN  TB8 RB8  TI RI
                   //SM0,SM1 ģʽѡ��00-8λ��λ�Ĵ�����      01-10λ�첽(��ʱ��1�����ʿɵ�)
                   //                  10-11λ�첽(f/32��f/64) 11-11λ�첽(��ʱ��1�����ʿɵ�)
                   //SM2 ��            1-ֻ��RB8Ϊ1��RI����λ     0-���յ��ַ�RI����λ
                   //REN��             1-�����пڽ���  0-��ֹ���пڽ���
                   //TB8,RB9��         ����/���յĵ�9λ����
                   //TI��              ��������жϱ�־λ
                   //RI:               ��������жϱ�־λ
                   //��ʽ1,��ֹ����

    TMOD = 0x20;   //       T1/C1        |         T0/C0
                   //GATE  C/T��  M1  M0   GATE  C/T��   M1  M0
                   //GATE:   1-˫���������ƣ�TR0/TR1+INT0/INT1   0-TR0/TR1����
                   //C/T���� 1-Couter   0-Timer
                   //M1,M0:  00-13λ��ʱ/������  TL��5λ��TH��8λ
                   //        01-16λ��ʱ��
                   //        10-�����Զ�װ���8λ��ʱ��/������
                   //        11-������T/C0������8Ϊ��ʱ����������
                   //��ʱ��1 ��ʱ��ʽ2����Ϊ���ڲ�����ʱ��

    TH1 = (uchar)( 256 - (XTAL/(16L*T*BAUDRATE)));  //T=6 ˫���� T=12 ������
		 //��ʽ1 Baudrate formula:(2^SMOD/32)*(T1�����)
		 //T1����� formula�� SYSclk/12/(256 - TH1)->SYSclk/(12*(256 - TH1))
	//TH0 = 0;
	/*temp1 = 16L*T*BAUDRATE;	//����
	temp2 = XTAL%temp1;		//����
	temp1 = temp1 /2;			//������1/2
	

	if(temp2 > temp1 )			//����TH1�ټ�����
	{
		TH1--;
	}
			   */
    TR1 = 1;                                       //������ʱ��T1
	ES = 1;
}

//==================================================================
//================        �����жϳ���        ======================
//==================================================================

static void com_isr(void) interrupt 4 using 1
{
    char ch0;
    if(TI != 0)
    {
        TI = 0;    
       
        if(ostart != oend)                              //���ַ��ڻ�����
        {
               
                SBUF     = outbuf[ostart++ & (OLEN-1)]; //�����ַ�
                sendfull = 0;                           //����sendfull��־

        }
        else                                            //�������ַ��������
        {
                sendactive = 0;                         //����sendactive
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
//д�ַ���SBUF���ͻ�����
//=================================================================
void putbuf(char c)
{
  
	if(!sendfull)                                         //��������δ������
    {
        if(!sendactive)                                   //��������δ����
        {
            sendactive = 1;                               //��ֱ���͵�һ���ַ���SBUF���������
            SBUF       = c;                               //��������
         }
        else
        {
          //  ES = 0;                                             //�ش����ж�
            outbuf[ oend++  & (OLEN-1) ] = c;                   //���ַ������ͻ�����
            if (  ( (oend ^ ostart)  & (OLEN-1))  == 0 )
            {
                sendfull = 1;                                   //������������λsendfull��־
            }                                                   //�������ж�
           // ES = 1;
        }
    }
}

//=========================================
//    printf()ʹ��putchar()���һ���ַ�
//=========================================
char putchar(char c)
{
       /* if(c == '\n')
        {
              putbuf(0x0D);                                 //��LFǰ����CR
        }*/
		
		while(sendfull)
		{
		;
		}
       	if(c == '\n')
        {
           putbuf(0x0D);                                 //��LFǰ����CR
        }	   
        putbuf(c);                                          //���ַ����뻺����
        return(c);

}