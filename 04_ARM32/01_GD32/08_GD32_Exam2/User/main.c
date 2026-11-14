#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

#include "USART0.h"
#include "TIMER.h"
#include "ADC0.h"
#include "EXTI.h"
#include "SPI1_hard.h"

#include "Task.h"
#include "App.h"

int main(void) {
	// 抢占优先级[0, 3]，响应优先级[0, 3]
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);

	// 系统滴答定时器初始化
	systick_config();

	USART0_init();      // 串口，printf 重定向到这里
	TIMER_init();       // 定时器，其中 TIMER5 每 1ms 中断，为任务调度提供时基
	ADC0_init();        // ADC0，通道 2 = ADKey
	EXTI_init();        // 外部中断，EC12 的 A 相(PD11) 与按压(PD15)
	SPI1_hard_init();   // SPI1，驱动 WS2812

	// 业务驱动初始化
	// 注意顺序：TIMER_init() 必须在 App_init() 之前，
	// 因为它内部的 rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4)
	// 把定时器时钟倍频到 168M。
	App_init();

	while (1) {
		// 任务调度：按 task_list[] 里的周期调用各任务，不会阻塞
		Task_exec_handler();
	}

	return 0;
}
