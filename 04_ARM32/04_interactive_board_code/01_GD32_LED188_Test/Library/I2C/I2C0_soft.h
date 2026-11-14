#ifndef __I2C0_SOFT_H__
#define __I2C0_SOFT_H__

#include "gd32f4xx.h"
#include "systick.h"

// PB6 - SCL
#define I2C0S_SCL_RCU     RCU_GPIOB
#define I2C0S_SCL_PORT    GPIOB
#define I2C0S_SCL_PIN     GPIO_PIN_6
// PB7 - SDA
#define I2C0S_SDA_RCU     RCU_GPIOB
#define I2C0S_SDA_PORT    GPIOB
#define I2C0S_SDA_PIN     GPIO_PIN_7

#define  FAST    1   // 0 标准  1 快速

void I2C0_soft_init();

/* 写数据到I2C设备指定寄存器 */
uint8_t I2C0_soft_write(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len);

/* 从I2C设备指定寄存器读取数据 */
uint8_t I2C0_soft_read(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len);

#endif