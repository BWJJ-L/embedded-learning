/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#ifndef	__STC8G_PCA_H
#define	__STC8G_PCA_H

#include	"config.h"

//========================================================================
//                              寄存器位定义
//========================================================================

sfr CCON   = 0xD8;	//
sfr CMOD   = 0xD9;	//
sfr CCAPM0 = 0xDA;	//PCA模块0的工作模式寄存器。
sfr CCAPM1 = 0xDB;	//PCA模块1的工作模式寄存器。
sfr CCAPM2 = 0xDC;	//PCA模块2的工作模式寄存器。

sfr CL     = 0xE9;	//
sfr CCAP0L = 0xEA;	//PCA模块0的捕捉/比较寄存器低8位。
sfr CCAP1L = 0xEB;	//PCA模块1的捕捉/比较寄存器低8位。
sfr CCAP2L = 0xEC;	//PCA模块2的捕捉/比较寄存器低8位。

sfr PCA_PWM0 = 0xF2;	//PCA模块0 PWM寄存器。
sfr PCA_PWM1 = 0xF3;	//PCA模块1 PWM寄存器。
sfr PCA_PWM2 = 0xF4;	//PCA模块2 PWM寄存器。

sfr CH     = 0xF9;
sfr CCAP0H = 0xFA;		//PCA模块0的捕捉/比较寄存器高8位。
sfr CCAP1H = 0xFB;		//PCA模块1的捕捉/比较寄存器高8位。
sfr CCAP2H = 0xFC;		//PCA模块2的捕捉/比较寄存器高8位。

//						7     6     5     4     3     2     1     0     Reset Value
//sfr CCON   = 0xD8;	CF    CR    -     -    CCF3  CCF2  CCF1  CCF0   00xx,xx00	//PCA 控制寄存器。
sbit CCF0  = CCON^0;	/* PCA 模块0中断标志，由硬件置位，必须由软件清0。	*/
sbit CCF1  = CCON^1;	/* PCA 模块1中断标志，由硬件置位，必须由软件清0。	*/
sbit CCF2  = CCON^2;	/* PCA 模块2中断标志，由硬件置位，必须由软件清0。	*/
//sbit CCF3  = CCON^3;	/* PCA 模块3中断标志，由硬件置位，必须由软件清0。	*/
sbit CR    = CCON^6;	/* 1: 允许PCA计数器计数，必须由软件清0。*/
sbit CF    = CCON^7;	/* PCA计数器溢出（CH，CL由FFFFH变为0000H）标志。PCA计数器溢出后由硬件置位，必须由软件清0。*/

//========================================================================
//                              定义声明
//========================================================================

#define PCA_VECTOR		7

#define	PCA0			0
#define	PCA1			1
#define	PCA2			2
#define	PCA_Counter		3

#define	PCA_Clock_1T	(4<<1)
#define	PCA_Clock_2T	(1<<1)
#define	PCA_Clock_4T	(5<<1)
#define	PCA_Clock_6T	(6<<1)
#define	PCA_Clock_8T	(7<<1)
#define	PCA_Clock_12T	(0<<1)
#define	PCA_Clock_Timer0_OF	(2<<1)
#define	PCA_Clock_ECI	(3<<1)

#define	PCA_Rise_Active	(1<<5)
#define	PCA_Fall_Active	(1<<4)

#define	PCA_PWM_8bit	(0<<6)
#define	PCA_PWM_7bit	(1<<6)
#define	PCA_PWM_6bit	(2<<6)
#define	PCA_PWM_10bit	(3<<6)

//---------------------------------------------------------

#define	PCA_ECOM			(1<<6)	//允许PCA模块的比较功能
#define	PCA_CCAPP			(1<<5)	//允许PCA模块进行上升沿捕获
#define	PCA_CCAPN			(1<<4)	//允许PCA模块进行下降沿捕获
#define	PCA_MAT				(1<<3)	//允许PCA模块的匹配功能
#define	PCA_TOG				(1<<2)	//允许PCA模块的高速脉冲输出功能
#define	PCA_PWM				(1<<1)	//允许PCA模块的脉宽调制输出功能
#define	PCA_ECCF			(1<<0)	//允许PCA模块的匹配/捕获中断

#define	PCA_Mode_PWM							0x42	//B0100_0010
#define	PCA_Mode_Capture					0x11	//B0001_0001
#define	PCA_Mode_SoftTimer				0x48	//B0100_1000
#define	PCA_Mode_HighPulseOutput	0x4d	//B0100_1101

//---------------------------------------------------------

typedef struct
{
	u8	PCA_Clock;		//PCA_Clock_1T, PCA_Clock_2T, PCA_Clock_4T, PCA_Clock_6T, PCA_Clock_8T, PCA_Clock_12T, PCA_Clock_Timer0_OF, PCA_Clock_ECI
	u8	PCA_PWM_Wide;	//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit, PCA_PWM_10bit
	u16	PCA_Value;
	u8	PCA_RUN;			//ENABLE, DISABLE
} PCA_InitTypeDef;

extern	bit		B_Capture0,B_Capture1,B_Capture2;
extern	u8		PCA0_mode,PCA1_mode,PCA2_mode;
extern	u16		CCAP0_tmp,PCA_Timer0;
extern	u16		CCAP1_tmp,PCA_Timer1;
extern	u16		CCAP2_tmp,PCA_Timer2;

void	PCA_Init(u8 PCA_id, PCA_InitTypeDef *PCAx);
void	UpdatePcaPwm(u8 PCA_id, u16 pwm_value);


#endif

