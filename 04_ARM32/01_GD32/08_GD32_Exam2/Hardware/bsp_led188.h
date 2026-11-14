#ifndef __BSP_LED188_H__
#define __BSP_LED188_H__

#include "gpio_cfg.h"

/*  接线说明
交互板           GD32F407
        1  ===>  PE7
        2  ===>  PE9
        3  ===>  PE11
        4  ===>  PE13
        5  ===>  PE15
       3v3 ===>  3v3(可以不接，因为是直接引脚高低控制灯)
       GND ===>	 GND
*/

//============== 引脚
#define LED188_PIN1_RCU		RCU_GPIOE
#define LED188_PIN1_PIN		GPIOE, GPIO_PIN_7

#define LED188_PIN2_RCU		RCU_GPIOE
#define LED188_PIN2_PIN		GPIOE, GPIO_PIN_9

#define LED188_PIN3_RCU		RCU_GPIOE
#define LED188_PIN3_PIN		GPIOE, GPIO_PIN_11

#define LED188_PIN4_RCU		RCU_GPIOE
#define LED188_PIN4_PIN		GPIOE, GPIO_PIN_13

#define LED188_PIN5_RCU		RCU_GPIOE
#define LED188_PIN5_PIN		GPIOE, GPIO_PIN_15

//============== 输出：高低电平
#define LED188_PIN1_OUT(val)	GPIO_output(LED188_PIN1_RCU, LED188_PIN1_PIN, GPIO_OTYPE_PP, val) 
#define LED188_PIN2_OUT(val)	GPIO_output(LED188_PIN2_RCU, LED188_PIN2_PIN, GPIO_OTYPE_PP, val)
#define LED188_PIN3_OUT(val)	GPIO_output(LED188_PIN3_RCU, LED188_PIN3_PIN, GPIO_OTYPE_PP, val)
#define LED188_PIN4_OUT(val)	GPIO_output(LED188_PIN4_RCU, LED188_PIN4_PIN, GPIO_OTYPE_PP, val)
#define LED188_PIN5_OUT(val)	GPIO_output(LED188_PIN5_RCU, LED188_PIN5_PIN, GPIO_OTYPE_PP, val)
//============== 浮空输入(高阻态)
#define LED188_PIN1_IN()		GPIO_input(LED188_PIN1_RCU, LED188_PIN1_PIN, GPIO_PUPD_NONE)
#define LED188_PIN2_IN()		GPIO_input(LED188_PIN2_RCU, LED188_PIN2_PIN, GPIO_PUPD_NONE)
#define LED188_PIN3_IN()		GPIO_input(LED188_PIN3_RCU, LED188_PIN3_PIN, GPIO_PUPD_NONE)
#define LED188_PIN4_IN()		GPIO_input(LED188_PIN4_RCU, LED188_PIN4_PIN, GPIO_PUPD_NONE)
#define LED188_PIN5_IN()		GPIO_input(LED188_PIN5_RCU, LED188_PIN5_PIN, GPIO_PUPD_NONE)

//============== 所有引脚高阻(断开)
#define  LED188_ALL_IN()		LED188_PIN1_IN();LED188_PIN2_IN();LED188_PIN3_IN();LED188_PIN4_IN();LED188_PIN5_IN()


// 初始化
void LED188_init();

void LED188_display(); // 内容显示，点灯

void LED188_clear(void);             // 全部熄灭（.c 里有定义，原 .h 漏了声明）
void LED188_set_num(uint8_t num);    // 数字
void LED188_charge(uint8_t enable);  // 闪电
void LED188_percent(uint8_t enable); // %

#endif