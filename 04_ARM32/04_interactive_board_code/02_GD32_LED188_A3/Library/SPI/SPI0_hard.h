#ifndef __SPI0_HARD_H__
#define __SPI0_HARD_H__

#include "gpio_cfg.h"

//SCL=SCLK 
#define SPI0H_SCL_RCU     RCU_GPIOA
#define SPI0H_SCL_PORT    GPIOA
#define SPI0H_SCL_PIN     GPIO_PIN_5
#define SPI0H_SCL_AF      GPIO_AF_5

//SDA=MOSI
#define SPI0H_MOSI_RCU    RCU_GPIOA
#define SPI0H_MOSI_PORT   GPIOA
#define SPI0H_MOSI_PIN    GPIO_PIN_7
#define SPI0H_MOSI_AF     GPIO_AF_5

//FS0=MISO
#define SPI0H_MISO_RCU    RCU_GPIOA
#define SPI0H_MISO_PORT   GPIOA
#define SPI0H_MISO_PIN    GPIO_PIN_6
#define SPI0H_MISO_AF     GPIO_AF_5

// 预分频：SPI_PSC_x x为(2, 4, 8, 16, 32, 64, 128, 256)
#define SPI0H_PSC		  SPI_PSC_4

// 极性和相位：
//	SPI_CK_PL_LOW_PH_1EDGE		0，0
//	SPI_CK_PL_LOW_PH_2EDGE   	0，1
//	SPI_CK_PL_HIGH_PH_1EDGE		1，0
//	SPI_CK_PL_HIGH_PH_2EDGE		1，1
#define SPI0H_CPOL_CPHA		SPI_CK_PL_HIGH_PH_2EDGE

// 大小端：
//	SPI_ENDIAN_MSB： 大端
//	SPI_ENDIAN_LSB： 小端
#define SPI0H_PARAM_ENDIAN		SPI_ENDIAN_MSB

// 初始化
void SPI0_hard_init();
// 写数据
void SPI0_hard_write(uint8_t dat);
// 读数据
uint8_t SPI0_hard_read();

// 读写数据
uint8_t SPI0_hard_read_write(uint8_t dat);

#endif