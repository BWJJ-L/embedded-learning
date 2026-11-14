#ifndef __TASK_H__
#define __TASK_H__

#include "gpio_cfg.h"

// 系统滴答，单位 ms，由 TIMER5 中断每 1ms 累加
// volatile 必须加：主循环读、中断里写，不加会被编译器优化成只读一次
extern volatile uint64_t g_t5_tick_ms;

// 任务调度，在 while(1) 里不断调用
void Task_exec_handler(void);

// TIMER5 中断回调，由 TIMER.c 的 TIMER5_DAC_IRQHandler 调用
void TIMER5_irq_callback(void);

#endif
