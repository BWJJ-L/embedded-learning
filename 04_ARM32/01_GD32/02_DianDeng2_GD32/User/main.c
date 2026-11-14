#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
/*********************
任务目标:

模板工程

**********************/

#define GPIO_LED    GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15

#define LED_SW  GPIOC, GPIO_PIN_6
#define LED    GPIOD, GPIO_LED
//#define LED2    GPIOD, GPIO_PIN_9
//#define LED3    GPIOD, GPIO_PIN_10
//#define LED4    GPIOD, GPIO_PIN_11
//#define LED5    GPIOD, GPIO_PIN_12
//#define LED6    GPIOD, GPIO_PIN_13
//#define LED7    GPIOD, GPIO_PIN_14
//#define LED8    GPIOD, GPIO_PIN_15




void GPIO_Config(){
    //==================================PB2   核心板点灯
    // 1. 时钟使能
    rcu_periph_clock_enable(RCU_GPIOB);
    // 2. IO模式配置：输出
    //           GPIO端口     输出模式      无上拉下拉        引脚
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_2);
    // 3. 输出选项：推挽
    //                     GPIO端口     推挽              速度         引脚
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_2);
    
    
    //==================================PC6 扩展板 三极管
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_6);
    
    //==================================扩展板 LED
    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_LED);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_LED);
    
    // 三极管低电平导通
    gpio_bit_reset(LED_SW);
    // STC8，配置推挽IO后，默认为高电平，GD32，配置完，默认低电平，人为配置高电平，灭
    gpio_bit_set(LED); //高灭
}

int main(void) {
    uint8_t i = 0;

	// 系统滴答定时器初始化
	systick_config();
    
    GPIO_Config();// GPIO配置
    
    while(i < 3){
        gpio_bit_set(GPIOB, GPIO_PIN_2);
        delay_1ms(500);
        gpio_bit_reset(GPIOB, GPIO_PIN_2);
        delay_1ms(500);
        
        gpio_bit_reset(LED);
        delay_1ms(500);
        gpio_bit_set(LED);
        delay_1ms(500);
        
        i++;
    }
  
	while(1) {
        
	}
	
}