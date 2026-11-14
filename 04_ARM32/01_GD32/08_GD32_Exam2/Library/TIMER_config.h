#ifndef __TIMER_CONFIG_H__
#define __TIMER_CONFIG_H__

#include "gd32f4xx.h"

#define USE_TIMER_0     0  // 高级
#define USE_TIMER_1     0  
#define USE_TIMER_2     0
#define USE_TIMER_3     0  // LED5 ~ 8
#define USE_TIMER_4     0
#define USE_TIMER_5     1	// 打开开关，需定义void TIMER5_irq_callback()函数
#define USE_TIMER_6     0	// 打开开关，需定义void TIMER6_irq_callback()函数
#define USE_TIMER_7     0
#define USE_TIMER_8     0  // 舵机
#define USE_TIMER_9     0  // 震动马达
#define USE_TIMER_10    0
#define USE_TIMER_11    0
#define USE_TIMER_12    0
#define USE_TIMER_13    0

#if USE_TIMER_0
// 启用TIMER0的情况下，才需要配置如下参数
#define TM0_PRESCALER   10
#define TM0_FREQ        1000
#define TM0_PERIOD      (SystemCoreClock / TM0_PRESCALER / TM0_FREQ)

// TIMER0CHx对应的引脚所有参数
// TIMER0_CH0 ON  PE8
#define TM0_CH0_ON     RCU_GPIOE, GPIOE, GPIO_PIN_8, GPIO_OTYPE_PP, GPIO_AF_1
// TIMER0_CH0 OP  PE9
#define TM0_CH0_OP     RCU_GPIOE, GPIOE, GPIO_PIN_9, GPIO_OTYPE_PP, GPIO_AF_1

// TIMER0_CH1 ON  PE10
#define TM0_CH1_ON     RCU_GPIOE, GPIOE, GPIO_PIN_10, GPIO_OTYPE_PP, GPIO_AF_1
// TIMER0_CH1 OP  PE11
#define TM0_CH1_OP     RCU_GPIOE, GPIOE, GPIO_PIN_11, GPIO_OTYPE_PP, GPIO_AF_1

#endif

#if USE_TIMER_1
// 启用TIMER1的情况下，才需要配置如下参数
#define TM1_PRESCALER   10
#define TM1_FREQ        1000
#define TM1_PERIOD      (SystemCoreClock / TM1_PRESCALER / TM1_FREQ)

// TIMER1CHx对应的引脚所有参数
#define TM1_CH0        RCU_GPIOB, GPIOB, GPIO_PIN_8, GPIO_OTYPE_PP, GPIO_AF_1

#endif

#if USE_TIMER_3
// 启用TIMER3的情况下，才需要配置如下参数
#define TM3_PRESCALER   10
#define TM3_FREQ        1000
#define TM3_PERIOD      (SystemCoreClock / TM3_PRESCALER / TM3_FREQ)

// TIMER3CHx对应的引脚所有参数
#define TM3_CH0        RCU_GPIOD, GPIOD, GPIO_PIN_12, GPIO_OTYPE_PP, GPIO_AF_2
#define TM3_CH1        RCU_GPIOD, GPIOD, GPIO_PIN_13, GPIO_OTYPE_PP, GPIO_AF_2
#define TM3_CH2        RCU_GPIOD, GPIOD, GPIO_PIN_14, GPIO_OTYPE_PP, GPIO_AF_2
#define TM3_CH3        RCU_GPIOD, GPIOD, GPIO_PIN_15, GPIO_OTYPE_PP, GPIO_AF_2
#endif

#if USE_TIMER_5 // 基本定时器，和引脚没有关系，时间到，触发中断

// 启用TIMER5的情况下，才需要配置如下参数
#define TM5_PRESCALER   10
#define TM5_FREQ        1000	// 1000hz ==> 1ms时间间隔
#define TM5_PERIOD      (SystemCoreClock / TM5_PRESCALER / TM5_FREQ)
#define TM5_PRIORITY	2, 2

void TIMER5_irq_callback(); // 中断函数回调函数

#endif

#if USE_TIMER_6 // 基本定时器，和引脚没有关系，时间到，触发中断

// 启用TIMER6的情况下，才需要配置如下参数
#define TM6_PRESCALER   10
#define TM6_FREQ        1000	// 1000hz ==> 1ms时间间隔
#define TM6_PERIOD      (SystemCoreClock / TM6_PRESCALER / TM6_FREQ)
#define TM6_PRIORITY	2, 2

void TIMER6_irq_callback(); // 中断函数回调函数

#endif

#if USE_TIMER_8
// 启用TIMER8的情况下，才需要配置如下参数
#define TM8_PRESCALER   1000
#define TM8_FREQ        50		// 50hz频率
#define TM8_PERIOD      (SystemCoreClock / TM8_PRESCALER / TM8_FREQ)

// TIMER8CHx对应的引脚所有参数   
#define TM8_CH0        RCU_GPIOE, GPIOE, GPIO_PIN_5, GPIO_OTYPE_PP, GPIO_AF_3

#endif

#if USE_TIMER_9
// 启用TIMER9的情况下，才需要配置如下参数
#define TM9_PRESCALER   10
#define TM9_FREQ        1000
#define TM9_PERIOD      (SystemCoreClock / TM9_PRESCALER / TM9_FREQ)

// TIMER9CHx对应的引脚所有参数
#define TM9_CH0        RCU_GPIOB, GPIOB, GPIO_PIN_8, GPIO_OTYPE_PP, GPIO_AF_3

#endif

#endif