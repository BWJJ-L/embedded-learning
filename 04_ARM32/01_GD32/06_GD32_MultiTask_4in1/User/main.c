#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

#include "TIMER.h"
#include "Task.h"

#include "App.h"

int main(void) {
	// 抢占优先级[0, 3]
	// 响应优先级[0, 3]
	// 配置全局优先级分组规则
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);

	// 系统滴答定时器初始化
	systick_config();

	// 定时器初始化，其中 TIMER5 每 1ms 产生一次中断，为任务调度提供时基
	// 注意：这一句内部的 rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4)
	// 把定时器时钟倍频到 168M。蜂鸣器的音调计算公式依赖这个前提，
	// 所以 TIMER_init() 必须排在 App_init()（里面会调 Buzzer_init）之前。
	TIMER_init();

	// 外设初始化：LED 灯组(含 PC6 总开关)、数码管、按键、蜂鸣器
	App_init();

	while (1) {
		// 任务调度：按 task_list[] 里的周期调用各任务，不会阻塞
		Task_exec_handler();
	}

	return 0;
}
