#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "Delay.h"
#include "NVIC.h"
#include "Switch.h"

/*
串口通讯测试 —— 双设备 UART 互联(透传)

硬件连接:
  1. PC串口A 接 设备A 的 UART1 (P3.0/P3.1)
     PC串口B 接 设备B 的 UART1 (P3.0/P3.1)
  2. 设备A 的 UART4 (P5.2/P5.3) 与 设备B 的 UART4 交叉相连:
     A_RXD(P5.2) <-- B_TXD(P5.3)
     A_TXD(P5.3) --> B_RXD(P5.2)
     两板 GND 相连

功能:
  a. 从 UART1 收到 PC 发来的数据, 原样从 UART4 转发出去
  b. 从 UART4 收到另一台设备发来的数据, 原样从 UART1 转发出去

A、B 两台设备烧录同一份程序。
默认波特率: 115200, N, 8, 1
*/

/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

	// 串口1的 P3.0(RXD), P3.1(TXD) 配置为准双向口(上拉)
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;	//指定要初始化的IO
	GPIO_InitStructure.Mode = GPIO_PullUp;				//上拉准双向口
	GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);			//初始化P3口

	// 串口4的 P5.2(RXD), P5.3(TXD) 配置为准双向口(上拉)
	GPIO_InitStructure.Pin  = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);			//初始化P5口
}

/***************  串口初始化函数 *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义

	//---------- 串口1: 连接PC ----------
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//8位数据,可变波特率
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;		//波特率发生器: Timer1
	COMx_InitStructure.UART_BaudRate  = 115200UL;			//波特率
	COMx_InitStructure.UART_RxEnable  = ENABLE;			//允许接收
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率不加倍
	UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1

	NVIC_UART1_Init(ENABLE, Priority_1);	//开启串口1中断, 优先级1
	UART1_SW(UART1_SW_P30_P31);				//串口1引脚切换: P3.0-RXD, P3.1-TXD

	//---------- 串口4: 连接另一台设备 ----------
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer4;		//波特率发生器改用 Timer4
														//(Timer1已被串口1占用, 不能冲突)
	UART_Configuration(UART4, &COMx_InitStructure);		//初始化串口4, 其余参数同上

	NVIC_UART4_Init(ENABLE, Priority_1);	//开启串口4中断, 优先级1
	UART4_SW(UART4_SW_P52_P53);				//串口4引脚切换: P5.2-RXD, P5.3-TXD
}

// C89 语法: 局部变量必须声明在函数/大括号开头
void main()
{
	u8 i;

	// 初始化GPIO
	GPIO_config();
	// 初始化UART1 和 UART4
	UART_config();

	// 开启全局中断 (必须要加)
	EA = 1;
    

	// 上电提示, 会从本板 UART1 发回给PC串口工具
	PrintString1("UART Bridge Ready!\r\n");

	while(1)
	{
		//---------- UART1 收到数据 -> 从 UART4 转发 ----------
		//中断里每收1个字节, RX_TimeOut 被重置为 TimeOutSet1(=5)
		//主循环里超时计数减到0, 说明一帧数据接收完整了
		if(COM1.RX_TimeOut > 0)
		{
			if(--COM1.RX_TimeOut == 0)
			{
				for(i = 0; i < COM1.RX_Cnt; i++)
				{
					TX4_write2buff(RX1_Buffer[i]);	//收到的数据从串口4原样发出
				}
				COM1.RX_Cnt = 0;	//清空接收计数, 准备接收下一帧
			}
		}

		//---------- UART4 收到数据 -> 从 UART1 转发 ----------
		if(COM4.RX_TimeOut > 0)
		{
			if(--COM4.RX_TimeOut == 0)
			{
                if('A' == RX4_Buffer[0]){
                    P53 = 1;
                }else if('B' == RX4_Buffer[0]){
                    P53 = 0;
                }
                
				for(i = 0; i < COM4.RX_Cnt; i++)
				{
					TX1_write2buff(RX4_Buffer[i]);	//收到的数据从串口1原样发出
				}
				COM4.RX_Cnt = 0;
			}
		}
	}
}
