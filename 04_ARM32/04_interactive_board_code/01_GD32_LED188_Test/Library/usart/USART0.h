#ifndef __USART0_H__
#define __USART0_H__

#include "gpio_cfg.h"

// =============================== DMA ============================
// DMA发送功能开关 
#define USART0_TX_DMA_ENABLE      1
// DMA接收功能开关 
#define USART0_RX_DMA_ENABLE      1

// 外设的数据地址
#define USART0_DATA_ADDR          (uint32_t)&USART_DATA(USART0)

#define USART0_TX_DMA_RCU         RCU_DMA1
#define USART0_TX_DMA_PERIPH_CH   DMA1, DMA_CH7
#define USART0_TX_DMA_PERIPH_SUB  DMA_SUBPERI4

#define USART0_RX_DMA_RCU         RCU_DMA1
#define USART0_RX_DMA_PERIPH_CH   DMA1, DMA_CH5
#define USART0_RX_DMA_PERIPH_SUB  DMA_SUBPERI4

// 功能开关
#define USART0_PRINTF			1
// 开关打开为1，同时，在合适位置定义函数void USART0_on_recv(uint8_t* data, uint32_t len)
#define USART0_RECV_CALLBACK	1	

#if USART0_RECV_CALLBACK
// 收到串口0数据，回调函数
void USART0_on_recv(uint8_t* data, uint32_t len);
#endif

// PA9 	USART0_TX	AF7
#define USART0_TX_RCU 	RCU_GPIOA
#define USART0_TX_PORT 	GPIOA
#define USART0_TX_PIN 	GPIO_PIN_9
#define USART0_TX_AF 	GPIO_AF_7

// PA10 	USART0_RX	AF7
#define USART0_RX_RCU 	RCU_GPIOA
#define USART0_RX_PORT 	GPIOA
#define USART0_RX_PIN 	GPIO_PIN_10
#define USART0_RX_AF 	GPIO_AF_7

// 波特率
#define USART0_BAUDRATE 115200UL

// 优先级
#define USART0_PRIORITY	2,2

void USART0_init(); // 串口初始化
// 串口发送
// 发送1个byte数据
void USART0_send_byte(uint8_t byte);
// 发送多个byte数据
void USART0_send_data(uint8_t* data, uint32_t len);
// 发送字符串 (结尾标记\0)
void USART0_send_string(char *data);

#endif