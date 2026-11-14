#ifndef __NIXIE_H__
#define __NIXIE_H__

#include "gd32f4xx.h"

#ifndef u8
#define u8 uint8_t
#endif

#ifndef u16
#define u16 uint16_t
#endif

#ifndef u32
#define u32 uint32_t
#endif

// 输入寄存器（595 的 SER）
#define NIX_DI_RCU      RCU_GPIOD
#define NIX_DI_GPIO     GPIOD, GPIO_PIN_0
// 锁存寄存器（595 的 RCLK）
#define NIX_RCK_RCU     RCU_GPIOD
#define NIX_RCK_GPIO    GPIOD, GPIO_PIN_1
// 移位寄存器（595 的 SRCLK）
#define NIX_SCK_RCU     RCU_GPIOD
#define NIX_SCK_GPIO    GPIOD, GPIO_PIN_4

// 段码表下标最大值，见 Nixie.c 的 LED_TABLE[]
#define NIX_TABLE_MAX   33

// 初始化
void Nixie_init();

// num: 段码（8 位）
// idx: 位码（8 位，某位为 1 则该位数码管导通）
void Nixie_show(u8 num, u8 idx);

// num: 段码表下标，0~9 对应数字 0~9，其它字符查表
// idx: 显示位置 1~8，只点亮这一位
void Nixie_display(u8 num, u8 idx);

// 全部熄灭
void Nixie_clear();

#endif
