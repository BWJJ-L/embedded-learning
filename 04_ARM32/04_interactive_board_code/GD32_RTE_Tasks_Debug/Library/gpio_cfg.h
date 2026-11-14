#ifndef __GPIO_CFG_H__
#define __GPIO_CFG_H__

#include "gd32f4xx.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_gpio.h"
#include "systick.h"
#include <stdio.h>

/**********************************************************
 * @brief  gpio输出配置
 * @param  rcu:  	时钟端口, RCU_GPIOx (x = A, B, C, D, E, F, G, H, I)
 * @param  port: 	GPIO端口, GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param  pin:  	GPIO引脚, GPIO_PIN_x(x=0..15)
 * @param  otype:   输出类型, 推挽 GPIO_OTYPE_PP, 开漏 GPIO_OTYPE_OD
 * @param  flag:    设置电平状态,  0低电平, 1高电平, 2不做处理为默认状态
 * @return  
 **********************************************************/
static inline void GPIO_output(rcu_periph_enum rcu, uint32_t port, uint32_t pin, uint8_t otype, uint8_t flag) {
	// 时钟使能
	rcu_periph_clock_enable(rcu);
	// mode配置  输出   GPIO_PUPD_PULLUP在开漏输出下才会起作用
	gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP,  pin);
	// 输出类型  
	gpio_output_options_set(port, otype, GPIO_OSPEED_MAX, pin);
	if (flag == 0) gpio_bit_reset(port, pin);
	else if (flag == 1) gpio_bit_set(port, pin);
}

/**********************************************************
 * @brief  gpio复用功能配置  输入输出都可以
 * @param  rcu:  	时钟端口, RCU_GPIOx (x = A, B, C, D, E, F, G, H, I)
 * @param  port: 	GPIO端口, GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param  pin:  	GPIO引脚, GPIO_PIN_x(x=0..15)
 * @param  otype:   输出类型, 推挽 GPIO_OTYPE_PP, 开漏 GPIO_OTYPE_OD
 * @param  af_num:  复用通道编号,  GPIO_AF_x(x=0..15)
 * @return  
 **********************************************************/
static inline void GPIO_output_af(rcu_periph_enum rcu, uint32_t port, uint32_t pin, uint8_t otype, uint32_t af_num) {
	// 时钟使能
	rcu_periph_clock_enable(rcu);
	// mode配置   GPIO_PUPD_PULLUP 只有再输入和OD才有作用
	gpio_mode_set(port, GPIO_MODE_AF, GPIO_PUPD_PULLUP,  pin);
	// 输出类型    gpio_output_options_set 只有在输出才有作用
	gpio_output_options_set(port, otype, GPIO_OSPEED_MAX, pin);
	
	// af通道配置
	gpio_af_set(port, af_num, pin);
}

/**********************************************************
 * @brief  gpio输入配置
 * @param  rcu:  	时钟端口, RCU_GPIOx (x = A, B, C, D, E, F, G, H, I)
 * @param  port: 	GPIO端口, GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param  pin:  	GPIO引脚, GPIO_PIN_x(x=0..15)
 * @param  pull_up_down:   上下拉配置
 *                         GPIO_PUPD_NONE: 浮空
 *                         GPIO_PUPD_PULLUP: 上拉
 *                         GPIO_PUPD_PULLDOWN: 下拉 
 * @return  
 **********************************************************/
static inline void GPIO_input(rcu_periph_enum rcu, uint32_t port, uint32_t pin, uint32_t pull_up_down) {
	// 时钟使能
	rcu_periph_clock_enable(rcu);
	// mode配置  输入   
	gpio_mode_set(port, GPIO_MODE_INPUT, pull_up_down,  pin);
}

/**********************************************************
 * @brief  gpio模拟输入配置
 * @param  rcu:  	时钟端口, RCU_GPIOx (x = A, B, C, D, E, F, G, H, I)
 * @param  port: 	GPIO端口, GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param  pin:  	GPIO引脚, GPIO_PIN_x(x=0..15)
 * @return  
 **********************************************************/
static inline void GPIO_analog(rcu_periph_enum rcu, uint32_t port, uint32_t pin) {
	// 时钟使能
	rcu_periph_clock_enable(rcu);
	// mode配置  模拟输入   
	gpio_mode_set(port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,  pin);
}



#endif