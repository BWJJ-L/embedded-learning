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

#include "SPI.h"

u8 	SPI_RxTimerOut;
u8 	SPI_BUF_type SPI_RxBuffer[SPI_BUF_LENTH];
bit B_SPI_Busy; //发送忙标志


//========================================================================
// 函数: void	SPI_Init(SPI_InitTypeDef *SPIx)
// 描述: SPI初始化程序.
// 参数: SPIx: 结构参数,请参考spi.h里的定义.
// 返回: none.
// 版本: V1.0, 2012-11-22
//========================================================================
void	SPI_Init(SPI_InitTypeDef *SPIx)
{
	if(SPIx->SPI_SSIG == ENABLE)			SPCTL |=  (1<<7);	//conform Master or Slave by SPI_Mode(ignore SS)
	else									SPCTL &= ~(1<<7);	//conform Master or Slave by SS pin.
	SPI_Start(SPIx->SPI_Enable);
	SPI_FirstBit_Set(SPIx->SPI_FirstBit);
	SPI_Mode_Set(SPIx->SPI_Mode);
	SPI_CPOL_Set(SPIx->SPI_CPOL);
	SPI_CPHA_Set(SPIx->SPI_CPHA);
	SPI_Clock_Select(SPIx->SPI_Speed);
	
	SPI_RxTimerOut = 0;
	B_SPI_Busy = 0;
}

//========================================================================
// 函数: void	SPI_SetMode(u8 mode)
// 描述: SPI设置主从模式函数.
// 参数: mode: 指定模式, 取值 SPI_Mode_Master 或 SPI_Mode_Slave.
// 返回: none.
// 版本: V1.0, 2012-11-22
//========================================================================
void	SPI_SetMode(u8 mode)
{
	if(mode == SPI_Mode_Slave)
	{
		SPCTL &= ~(1<<4);	//重新设置为从机待机
		SPCTL &= ~(1<<7);	//SS引脚确定主从
	}
	else
	{
		SPCTL |= (1<<4);	//使能 SPI 主机模式
		SPCTL |= (1<<7);	//忽略SS引脚功能
	}
}

//========================================================================
// 函数: void SPI_WriteByte(u8 dat)
// 描述: SPI发送一个字节数据.
// 参数: dat: 要发送的数据.
// 返回: none.
// 版本: V1.0, 2020-09-14
//========================================================================
void SPI_WriteByte(u8 dat)		//SPI发送一个字节数据
{
	SPDAT = dat;
	B_SPI_Busy = 1;
	while(B_SPI_Busy) ;
}

//========================================================================
// 函数: void SPI_ReadByte(u8 dat)
// 描述: SPI查询模式读取一个字节数据.
// 参数: none.
// 返回: 读取的数据.
// 版本: V1.0, 2020-09-14
//========================================================================
u8 SPI_ReadByte(void)
{
    SPDAT = 0xff;
	B_SPI_Busy = 1;
	while(B_SPI_Busy) ;
    return (SPDAT);
}
