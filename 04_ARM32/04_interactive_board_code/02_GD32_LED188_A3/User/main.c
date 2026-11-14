#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

#include "USART0.h"
#include "TIMER.h"

#include "Task.h"
#include "App.h"

void USART0_on_recv(uint8_t* data, uint32_t len) {
    printf("recv[%d]:%s\n", len, data);
}

int main(void) {
	// 配置全局优先级分组规则
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);

    // 初始化系统嘀嗒定时器
    systick_config();

    USART0_init();	// 串口
	TIMER_init();   // 定时器
	
	App_init();	// (业务)驱动的初始化

    while(1) {
		// 任务调度处理
		Task_exec_handler();
    }
	
	return 0;
}
