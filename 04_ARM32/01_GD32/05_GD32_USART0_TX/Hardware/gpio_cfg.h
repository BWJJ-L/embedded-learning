#ifndef __GPIO_CFG_H__
#define __GPIO_CFG_H__

#include "gd32f4xx.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_gpio.h"
#include "systick.h"

/**********************************************************
 * @brief   复用输出配置 
 * @param 	rcu  	时钟端口  	RCU_GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param 	port  	引脚端口  	GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param 	pin  	引脚  	  	GPIO_PIN_x(x=0..15)
 * @param 	otype  	输出模式  	GPIO_OTYPE_PP 推挽; GPIO_OTYPE_OD 开漏
 * @param 	af_num  复用引脚  	GPIO_AF_x
 * @return  
 **********************************************************/
static inline void GPIO_output_(rcu_periph_enum rcu, uint32_t port, uint32_t pin, uint8_t otype, uint32_t af_num) {
	// 时钟初始化
	rcu_periph_clock_enable(rcu);
	// GPIO模式：复用功能
	gpio_mode_set(port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, pin);
	// 输出选项设置
	gpio_output_options_set(port, otype, GPIO_OSPEED_MAX, pin);
	// 复用引脚配置
	gpio_af_set(port, af_num, pin);
}


/**********************************************************
 * @brief   复用输出配置 
 * @param 	rcu  	时钟端口  	RCU_GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param 	port  	引脚端口  	GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param 	pin  	引脚  	  	GPIO_PIN_x(x=0..15)
 * @param 	otype  	输出模式  	GPIO_OTYPE_PP 推挽; GPIO_OTYPE_OD 开漏
 * @param 	af_num  复用引脚  	GPIO_AF_x
 * @return  
 **********************************************************/
static inline void GPIO_output_af(rcu_periph_enum rcu, uint32_t port, 
		uint32_t pin, uint8_t otype, uint32_t af_num) {
	// 时钟初始化
	rcu_periph_clock_enable(rcu);
	// GPIO模式：复用功能
	gpio_mode_set(port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, pin);
	// 输出选项设置
	gpio_output_options_set(port, otype, GPIO_OSPEED_MAX, pin);
	// 复用引脚配置
	gpio_af_set(port, af_num, pin);
}
        



/**********************************************************
 * @brief   gpio输入配置 
 * @param 	rcu  			时钟端口  	RCU_GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param 	port  			引脚端口  	GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param 	pin  			引脚  	  	GPIO_PIN_x(x=0..15)
 * @param 	pull_up_down  	上下拉模式  	GPIO_PUPD_NONE 浮空
										GPIO_PUPD_PULLUP 上拉
										GPIO_PUPD_PULLDOWN 下拉
 * @return  
 **********************************************************/
static inline void GPIO_input(rcu_periph_enum rcu, uint32_t port, uint32_t pin, uint32_t pull_up_down) {
	// 时钟初始化
	rcu_periph_clock_enable(rcu);
	// GPIO模式：输入
	gpio_mode_set(port, GPIO_MODE_INPUT, pull_up_down, pin);
}



#endif