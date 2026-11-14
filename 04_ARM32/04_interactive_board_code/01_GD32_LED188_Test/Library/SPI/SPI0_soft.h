#ifndef __SPI0_SOFT_H__
#define __SPI0_SOFT_H__

#include "gpio_cfg.h"

//SCL=SCLK 
#define SPI0S_SCL_RCU     RCU_GPIOA
#define SPI0S_SCL_PORT    GPIOA
#define SPI0S_SCL_PIN     GPIO_PIN_5
//SDA=MOSI
#define SPI0S_MOSI_RCU    RCU_GPIOA
#define SPI0S_MOSI_PORT   GPIOA
#define SPI0S_MOSI_PIN    GPIO_PIN_7
//FS0=MISO
#define SPI0S_MISO_RCU    RCU_GPIOA
#define SPI0S_MISO_PORT   GPIOA
#define SPI0S_MISO_PIN    GPIO_PIN_6

#define SPI0S_SCL(bit)    gpio_bit_write(SPI0S_SCL_PORT,SPI0S_SCL_PIN, (bit) ? SET : RESET)
#define SPI0S_MOSI(bit)   gpio_bit_write(SPI0S_MOSI_PORT,SPI0S_MOSI_PIN,(bit) ? SET : RESET)
#define SPI0S_MISO()      gpio_input_bit_get(SPI0S_MISO_PORT,SPI0S_MISO_PIN)

// 初始化
void SPI0_soft_init();
// 写数据
void SPI0_soft_write(uint8_t dat);
// 读数据
uint8_t SPI0_soft_read();

// 读写数据
uint8_t SPI0_soft_read_write(uint8_t dat);

#endif