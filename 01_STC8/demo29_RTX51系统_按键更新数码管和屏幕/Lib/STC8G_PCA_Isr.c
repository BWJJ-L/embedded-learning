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

#include	"STC8G_PCA.h"

//========================================================================
//                               本地变量声明
//========================================================================

u16		CAP0_Old,CAP1_Old,CAP2_Old;       //记录上一次的捕获值
u16		CAP0_New,CAP1_New,CAP2_New;       //记录本次的捕获值

//========================================================================
// 函数: PCA_ISR_Handler
// 描述: PCA中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-23
//========================================================================
void	PCA_ISR_Handler (void) interrupt PCA_VECTOR
{
	if(CCF0)		//PCA模块0中断
	{
		CCF0 = 0;		//清PCA模块0中断标志
		// TODO: 在此处添加用户代码
		if(CCAPM0 >= PCA_Mode_SoftTimer)		//PCA_Mode_SoftTimer and PCA_Mode_HighPulseOutput
		{
			CCAP0_tmp += PCA_Timer0;
			CCAP0L = (u8)CCAP0_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
			CCAP0H = (u8)(CCAP0_tmp >> 8);	//后写CCAP0H
		}
		else if(CCAPM0 & PCA_Mode_Capture)
		{
			CAP0_Old = CAP0_New;
			CAP0_New = CCAP0H;	//读CCAP0H
			CAP0_New = (CAP0_New << 8) + CCAP0L;
			CCAP0_tmp = CAP0_New - CAP0_Old;
			B_Capture0 = 1;
		}
	}

	if(CCF1)	//PCA模块1中断
	{
		CCF1 = 0;		//清PCA模块1中断标志
		// TODO: 在此处添加用户代码
		if(CCAPM1 >= PCA_Mode_SoftTimer)		//PCA_Mode_SoftTimer and PCA_Mode_HighPulseOutput
		{
			CCAP1_tmp += PCA_Timer1;
			CCAP1L = (u8)CCAP1_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
			CCAP1H = (u8)(CCAP1_tmp >> 8);	//后写CCAP0H
		}
		else if(CCAPM1 & PCA_Mode_Capture)
		{
			CAP1_Old = CAP1_New;
			CAP1_New = CCAP1H;	//读CCAP1H
			CAP1_New = (CAP1_New << 8) + CCAP1L;
			CCAP1_tmp = CAP1_New - CAP1_Old;
			B_Capture1 = 1;
		}
	}

	if(CCF2)	//PCA模块2中断
	{
		CCF2 = 0;		//清PCA模块1中断标志
		// TODO: 在此处添加用户代码
		if(CCAPM2 >= PCA_Mode_SoftTimer)		//PCA_Mode_SoftTimer and PCA_Mode_HighPulseOutput
		{
			CCAP2_tmp += PCA_Timer2;
			CCAP2L = (u8)CCAP2_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
			CCAP2H = (u8)(CCAP2_tmp >> 8);	//后写CCAP0H
		}
		else if(CCAPM2 & PCA_Mode_Capture)
		{
			CAP2_Old = CAP2_New;
			CAP2_New = CCAP2H;	//读CCAP2H
			CAP2_New = (CAP2_New << 8) + CCAP2L;
			CCAP2_tmp = CAP2_New - CAP2_Old;
			B_Capture2 = 1;
		}
	}

	if(CF)	//PCA溢出中断
	{
		CF = 0;			//清PCA溢出中断标志
		// TODO: 在此处添加用户代码
	}

}