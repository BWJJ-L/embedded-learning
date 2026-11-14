#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

void GPIO_config() {
	// 时钟初始化
	rcu_periph_clock_enable(RCU_GPIOB);
	// GPIO模式：输出
	gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);
	// 输出选项设置：推挽
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_4);
}

int main(void) {
    systick_config();  // 滴答定时器
	GPIO_config(); // GPIO

    while(1) {
		gpio_bit_toggle(GPIOB, GPIO_PIN_4); // 状态切换
		delay_1ms(500);
    }
}