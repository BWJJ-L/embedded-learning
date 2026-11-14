#include "Task.h"
#include "App.h"

// 任务类型
typedef struct {
	// 上一次的时间
	uint64_t cnt;
	// 周期（时间间隔）
	uint16_t period;
	// 函数指针变量
	void (*exec)();
}Task_t;

// 任务列表(数组)
Task_t task_list[] = {
	// 上一次时间  周期    函数名
//	{0,            1000,      App_debug_test_task},
	{0,            2,      App_debug_led188_test_task}, 
};

// 宏 数组元素个数
#define TASK_LEN()	( sizeof(task_list)/sizeof(task_list[0]) )

/*
extern volatile uint64_t g_t5_tick_ms;

static uint64_t cnt = 0; // 上一次的时间记录，默认上一次是0

// 时间没有到，不往下执行
if (g_t5_tick_ms - cnt < 200) return;
// g_t5_tick_ms - cnt >= 周期 说明时间到了
cnt = g_t5_tick_ms; // 更新上一次的时间
*/
volatile uint64_t g_t5_tick_ms = 0;
// 别的文件要使用g_t5_tick_ms，需要先声明 extern volatile uint64_t g_t5_tick_ms;

// 中断函数回调函数
void TIMER5_irq_callback() { // 每隔1ms执行1次
	g_t5_tick_ms++;
}

void Task_exec_handler() {
	// for 循环管理任务，时间到，调用对应的函数(函数指针)
	//     上一次计数        周期                函数指针
	// task_list[0].cnt  task_list[0].period    task_list[0].exec()
	for(uint8_t i = 0; i < TASK_LEN(); i++) {
		if (g_t5_tick_ms - task_list[i].cnt < task_list[i].period) continue;
		task_list[i].cnt = g_t5_tick_ms; // 更新上一次的时间
		
		// 时间到了，调用函数
		 task_list[i].exec();
	}

}