#ifndef __I2C0_HARD_H__
#define __I2C0_HARD_H__

#include "gpio_cfg.h"

#define I2C0H_AF     GPIO_AF_4

// PB6 - SCL
#define I2C0H_SCL_RCU     RCU_GPIOB
#define I2C0H_SCL_PORT    GPIOB
#define I2C0H_SCL_PIN     GPIO_PIN_6

// PB7 - SDA
#define I2C0H_SDA_RCU     RCU_GPIOB
#define I2C0H_SDA_PORT    GPIOB
#define I2C0H_SDA_PIN     GPIO_PIN_7

#define I2C0H_SPEED  	400000UL
#define	I2C0H_TIMEOUT	50000

void I2C0_hard_init();

/* 写数据到I2C设备指定寄存器 */
uint8_t I2C0_hard_write(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len);

/* 从I2C设备指定寄存器读取数据 */
uint8_t I2C0_hard_read(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len);

#endif