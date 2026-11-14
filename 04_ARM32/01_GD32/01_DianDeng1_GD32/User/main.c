#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

/**********************************************************
 * @brief   输出配置 
 * @param 	rcu  	时钟端口  	RCU_GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param 	port  	引脚端口  	GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param 	pin  	引脚  	  	GPIO_PIN_x(x=0..15)
 * @param 	otype  	输出模式  	GPIO_OTYPE_PP 推挽; GPIO_OTYPE_OD 开漏
 * @return  
 **********************************************************/
void GPIO_output(rcu_periph_enum rcu, uint32_t port, uint32_t pin, uint8_t otype){
    // 时钟初始化
	rcu_periph_clock_enable(rcu);
	// GPIO模式：输出    GPIO_PUPD_PULLUP这个参数只对开头输出有作用
	gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pin);
	// 输出选项设置：推挽
	gpio_output_options_set(port, otype, GPIO_OSPEED_MAX, pin);
}

void GPIO_config() {
    // =============== PB2 推挽输出
    GPIO_output(RCU_GPIOB, GPIOB, GPIO_PIN_2, GPIO_OTYPE_PP);
    
    // ============== 底板  有别的引脚也要配置输出
}

int main(void) {
    bit_status flag = RESET;
    
    systick_config();  // 滴答定时器
    
	GPIO_config(); // GPIO
    
    
    while(1) {
		gpio_bit_write(GPIOB, GPIO_PIN_2, flag); // 状态切换
		flag = !flag;
        delay_1ms(500);
    }
}