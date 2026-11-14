#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
/*********************
任务目标:  核心板PA0 控制  PB2灯
PB2 高电平亮，PB2 低电平灭
PA0 按下是高，抬起是低

需求：按下  灯亮，抬起灯灭
**********************/


/**********************************************************
 * @brief   输出配置 
 * @param 	rcu  	时钟端口  	RCU_GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param 	port  	引脚端口  	GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param 	pin  	引脚  	  	GPIO_PIN_x(x=0..15)
 * @param 	otype  	输出模式  	GPIO_OTYPE_PP 推挽; GPIO_OTYPE_OD 开漏
 * @return  
 **********************************************************/
void GPIO_output(rcu_periph_enum rcu, uint32_t port, uint32_t pin, uint8_t otype) {
	// 时钟初始化
	rcu_periph_clock_enable(rcu);
	// GPIO模式：输出
	gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pin);
	// 输出选项设置
	gpio_output_options_set(port, otype, GPIO_OSPEED_MAX, pin);
}

/**********************************************************
 * @brief   输入配置 
 * @param 	rcu  			时钟端口  	RCU_GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param 	port  			引脚端口  	GPIOx(x = A,B,C,D,E,F,G,H,I)
 * @param 	pin  			引脚  	  	GPIO_PIN_x(x=0..15)
 * @param 	pull_up_down  	上下拉模式  	GPIO_PUPD_NONE 浮空
										GPIO_PUPD_PULLUP 上拉
										GPIO_PUPD_PULLDOWN 下拉
 * @return  
 **********************************************************/
void GPIO_input(rcu_periph_enum rcu, uint32_t port, uint32_t pin, uint32_t pull_up_down) {
	// 时钟初始化
	rcu_periph_clock_enable(rcu);
	// GPIO模式：输入
	gpio_mode_set(port, GPIO_MODE_INPUT, pull_up_down, pin);
}

void GPIO_config() {
	// =============== PB2 推挽输出
	GPIO_output(RCU_GPIOB, GPIOB, GPIO_PIN_2, GPIO_OTYPE_PP);

	// ============== PA0  浮空输入     外围电路已经下拉了，芯片内部浮空即可
	GPIO_input(RCU_GPIOA, GPIOA, GPIO_PIN_0, GPIO_PUPD_NONE);
}

#define  	KEY0	GPIOA, GPIO_PIN_0
#define   	DOWN0	SET
#define   	UP0		RESET
#define     PB2		GPIOB, GPIO_PIN_2

// 上一步状态
FlagStatus last_state0 = UP0;  // 有分号

int main(void) {
	GPIO_config();

	// 系统滴答定时器初始化
	systick_config();
  
	while(1) {
		// ==================核心板  PA0=================
		FlagStatus cur_state0 = gpio_input_bit_get(KEY0);
		if (last_state0 == UP0 && cur_state0 == DOWN0) {// 上一次是抬起，当前是按下，按下才有效
			last_state0 = DOWN0; // 保存状态
			
			gpio_bit_set(PB2);  // 高电平亮
			
		} else if (last_state0 == DOWN0 && cur_state0 == UP0) {// 上一次是按下，当前是抬起，抬起才有效
			last_state0 = UP0; // 保存状态
			gpio_bit_reset(PB2);  // 低电平灭
		}
			
		delay_1ms(20);
	}
	
}