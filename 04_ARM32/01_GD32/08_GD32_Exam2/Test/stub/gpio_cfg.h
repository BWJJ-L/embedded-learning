#ifndef __GPIO_CFG_H__
#define __GPIO_CFG_H__

/* =====================================================================
 * 主机侧测试替身：只提供 App 层编译所需的最小定义
 * 真实工程用的是 Library/gpio_cfg.h（含 gd32f4xx.h）
 *
 * 注意：gpio_input_bit_get 在这里是"函数"而不是宏，
 *       这样 bsp_ec12.h 里的 EC12_x_STATE() 才能当普通表达式用。
 * ===================================================================== */

#include <stdint.h>
#include <stdio.h>

typedef int FlagStatus;
#define SET    1
#define RESET  0

typedef int rcu_periph_enum;

/* 端口/引脚在主机上只是编号，不关心真实值 */
#define RCU_GPIOA 0
#define RCU_GPIOB 1
#define RCU_GPIOC 2
#define RCU_GPIOD 3
#define RCU_GPIOE 4

#define GPIOA 0
#define GPIOB 1
#define GPIOC 2
#define GPIOD 3
#define GPIOE 4

#define GPIO_PIN_0   (1u << 0)
#define GPIO_PIN_1   (1u << 1)
#define GPIO_PIN_2   (1u << 2)
#define GPIO_PIN_7   (1u << 7)
#define GPIO_PIN_9   (1u << 9)
#define GPIO_PIN_11  (1u << 11)
#define GPIO_PIN_13  (1u << 13)
#define GPIO_PIN_15  (1u << 15)

#define GPIO_OTYPE_PP        0
#define GPIO_PUPD_NONE       0
#define GPIO_PUPD_PULLUP     1
#define GPIO_PUPD_PULLDOWN   2

/* CMSIS intrinsic 的主机替身：单线程无中断，等价于 no-op */
static inline uint32_t __get_PRIMASK(void) { return 0u; }
static inline void     __disable_irq(void)  { }
static inline void     __set_PRIMASK(uint32_t v) { (void)v; }

void       GPIO_output(int rcu, int port, int pin, int otype, int flag);
void       GPIO_input(int rcu, int port, int pin, int pull);
FlagStatus gpio_input_bit_get(int port, uint32_t pin);

#endif
