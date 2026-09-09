#include "reg51.h"
#include "intrins.h"


sfr     T2L     =   0xd7;
sfr     T2H     =   0xd6;
sfr     AUXR    =   0x8e;

sfr     P0M1    =   0x93;
sfr     P0M0    =   0x94;
sfr     P1M1    =   0x91;
sfr     P1M0    =   0x92;
sfr     P2M1    =   0x95;
sfr     P2M0    =   0x96;
sfr     P3M1    =   0xb1;
sfr     P3M0    =   0xb2;
sfr     P4M1    =   0xb3;
sfr     P4M0    =   0xb4;
sfr     P5M1    =   0xc9;
sfr     P5M0    =   0xca;

sbit    P10     =   P1^0;
sbit    P11     =   P1^1;

sfr     P5      =   0xc8;
sbit    P53     =   P5^3;

void UART1_Isr() interrupt 4
{
    if (TI)
    {
        TI = 0;                                 //清中断标志
        P53 = 1;
    }
    
    if (RI)
    {
        RI = 0;                                 //清中断标志
        P53 = 0;
    }
}

void Delay1000ms(void)	//@11.0592MHz
{
	unsigned char data i, j, k;

	i = 57;
	j = 27;
	k = 112;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main()
{
    P5M1 &= ~(1 << 3);
    P5M0 |= (1 << 3);
   
    
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    

    SCON = 0x50;
    T2L = 0xe8;                                 //65536-11059200/115200/4=0FFE8H
    T2H = 0xff;
    AUXR = 0x15;                                //启动定时器
    ES = 1;                                     //使能串口中断
    EA = 1;
    SBUF = 0x5a;                                //发送测试数据

    while (1){
        
        SBUF = 0x26;
        Delay1000ms();
        Delay1000ms();
    };
}

