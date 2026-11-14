#include "Task.h"
#include "App.h"

// 任务类型
typedef struct {
	// 上一次执行时的系统滴答，单位 ms
	uint64_t cnt;
	// 周期（时间间隔），单位 ms
	uint16_t period;
	// 函数指针变量，任务函数必须无参无返回
	void (*exec)(void);
} Task_t;

// 任务列表(数组)
// 加任务就是在这里加一行：{上一次时间, 周期ms, 函数名}
// 周期要注意与别的任务配合：
//   数码管动态扫描必须比其它任务快得多，否则亮度不足且会闪。
//   2ms 一位 × 8 位 = 16ms 一轮 = 62.5Hz 刷新，人眼无闪烁。
Task_t task_list[] = {
	// 上一次时间  周期    函数名
	{0,            2,       App_nixie_task},      // 数码管扫描，2ms
	{0,            10,      App_key_scan_task},   // 按键扫描，10ms
	{0,            20,      App_buzzer_task},     // 蜂鸣器节拍，20ms
	{0,            200,     App_led_task},        // LED 灯组，200ms
};

// 宏 数组元素个数
#define TASK_LEN()	( sizeof(task_list)/sizeof(task_list[0]) )

volatile uint64_t g_t5_tick_ms = 0;
// 别的文件要使用 g_t5_tick_ms，需要先声明 extern volatile uint64_t g_t5_tick_ms;

// 中断函数回调函数，由 TIMER5_DAC_IRQHandler 调用
void TIMER5_irq_callback(void) { // 每隔1ms执行1次
	g_t5_tick_ms++;
}

void Task_exec_handler(void) {
	// for 循环管理任务，时间到就调用对应的函数（函数指针）
	// 用 uint32_t 而不是 uint8_t 做下标：任务数超过 255 时 uint8_t 会回绕成死循环
	for (uint32_t i = 0; i < TASK_LEN(); i++) {
		// 还没到周期，跳过这个任务
		if (g_t5_tick_ms - task_list[i].cnt < task_list[i].period) continue;

		// 更新时间基准
		task_list[i].cnt = g_t5_tick_ms;

		// 时间到了，执行任务
		task_list[i].exec();
	}
}
