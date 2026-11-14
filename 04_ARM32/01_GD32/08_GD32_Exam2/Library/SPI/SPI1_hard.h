#ifndef __SPI1_HARD_H__
#define __SPI1_HARD_H__

#include "gd32f4xx.h"

//SCL=SCLK 
#define SPI1H_SCL_RCU     RCU_GPIOB
#define SPI1H_SCL_PORT    GPIOB
#define SPI1H_SCL_PIN     GPIO_PIN_13
#define SPI1H_SCL_AF      GPIO_AF_5

//SDA=MOSI
#define SPI1H_MOSI_RCU    RCU_GPIOB
#define SPI1H_MOSI_PORT   GPIOB
#define SPI1H_MOSI_PIN    GPIO_PIN_15
#define SPI1H_MOSI_AF     GPIO_AF_5

//FS0=MISO
#define SPI1H_MISO_RCU    RCU_GPIOB
#define SPI1H_MISO_PORT   GPIOB
#define SPI1H_MISO_PIN    GPIO_PIN_14
#define SPI1H_MISO_AF     GPIO_AF_5

// 预分频：SPI_PSC_x x为(2, 4, 8, 16, 32, 64, 128, 256)
#define SPI1H_PSC		  SPI_PSC_4

// 极性和相位：
//	SPI_CK_PL_LOW_PH_1EDGE		0，0
//	SPI_CK_PL_LOW_PH_2EDGE   	0，1
//	SPI_CK_PL_HIGH_PH_1EDGE		1，0
//	SPI_CK_PL_HIGH_PH_2EDGE		1，1
#define SPI1H_CPOL_CPHA		SPI_CK_PL_HIGH_PH_2EDGE

// 大小端：
//	SPI_ENDIAN_MSB： 大端
//	SPI_ENDIAN_LSB： 小端
#define SPI1H_PARAM_ENDIAN		SPI_ENDIAN_MSB

#define SPI1H_TX_DMA_ENABLE			1  // DMA开关
#if SPI1H_TX_DMA_ENABLE
#define SPI1H_TX_DMA_RCU 			RCU_DMA0
#define SPI1H_TX_DMA_PERIPH_CH		DMA0, DMA_CH4
#define SPI1H_TX_DMA_PERIPH_SUB		DMA_SUBPERI0
#define SPI1H_DATA_ADDR				(uint32_t)(&SPI_DATA(SPI1))

void SPI1_dma_write(uint32_t memery_addr, uint32_t memery_len);

#endif

// 初始化
void SPI1_hard_init();
// 写数据
void SPI1_hard_write(uint8_t dat);
// 读数据
uint8_t SPI1_hard_read();
// 读写数据
uint8_t SPI1_hard_read_write(uint8_t dat);

#endif