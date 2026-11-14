#ifndef __UART1_H__
#define __UART1_H__

#include "board.h"

//UARTx
#define  UART1	                  CW_UART1
#define  UART1_CLK                SYSCTRL_APB1_PERIPH_UART1
#define  UART1_APBClkENx          SYSCTRL_APBPeriphClk_Enable1
#define  UART1_BaudRate           115200
#define  UART1_UclkFreq           8000000

//UARTx GPIO
#define  UART1_GPIO_CLK           (SYSCTRL_AHB_PERIPH_GPIOA)
#define  UART1_TX_GPIO_PORT       CW_GPIOA
#define  UART1_TX_GPIO_PIN        GPIO_PIN_2
#define  UART1_RX_GPIO_PORT       CW_GPIOA
#define  UART1_RX_GPIO_PIN        GPIO_PIN_3

//GPIO AF
#define  UART1_AFTX               PA02_AFx_UART1TXD()
#define  UART1_AFRX               PA03_AFx_UART1RXD()

//中断
#define  UART1_IRQ                UART1_IRQn

void uart1_init();	// 串口1初始化
void uart1_sendstring(char *string); // 串口1发送字符串

#endif