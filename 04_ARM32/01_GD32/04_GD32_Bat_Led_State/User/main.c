#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

/*********************
任务：实现充电宝电量指示灯

测试案例设计：
1. 准备工作，4个灯，3个按钮
2. 按钮1按下时，模拟开始充电
3. 按钮2按下时，模拟停止充电
4. 按钮3按下时，模拟电量增加

业务需求：
1. 开始充电时，需要呈现出流水灯闪烁
2. 4盏灯表示当前的电量
3. 充电流水灯起始位置是当前电量，全部点亮后，再次从当前电量位置进入流水灯效果
4. 结束充电时，关闭充电显示，当前电量进行闪烁3次，然后熄灭

LED: PD8 PD9 PD10 PD11
KEY: PC0 PC1 PC2
**********************/

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