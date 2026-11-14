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

// =====================================================================
// 任务列表
//
// 周期安排的理由：
//   ADKey 必须 1ms：驱动内部是「每 1ms 采 1 次、累加 50 次出一帧」，
//     周期变长会让出帧时间拉长，按键响应变迟钝。
//   数码管必须 2ms：2ms × 5 位 = 10ms 一轮 = 100Hz，人眼无闪烁。
//   其余都是 10ms / 40ms，都是 2ms 的整数倍，不会和扫描任务打架。
// =====================================================================
Task_t task_list[] = {
	// 上一次时间  周期    函数名
	{0,            2,       App_led188_scan_task},   // 数码管动态扫描，2ms
	{0,            1,       App_adkey_scan_task},    // ADKey 采样 + 短按/长按，1ms
	{0,            10,      App_key_scan_task},      // 编码器脉冲消费，10ms
	{0,            10,      App_fryer_task},         // 状态机 + 倒计时 + 显示，10ms
	{0,            40,      App_ws2812_task},        // WS2812 刷新，40ms
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
	for (uint8_t i = 0; i < TASK_LEN(); i++) {
		// 还没到周期，跳过这个任务
		if (g_t5_tick_ms - task_list[i].cnt < task_list[i].period) continue;

		// 更新时间基准
		task_list[i].cnt = g_t5_tick_ms;

		// 时间到了，执行任务
		task_list[i].exec();
	}
}
