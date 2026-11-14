#include "App.h"

// =====================================================================
// 编码器脉冲计数
//
// 【关键设计】中断里只累加这个计数，绝不碰显示。
// 因为 EC12_on_turn_right/left 是在 EXTI10_15 中断里被调用的，
// 而 LED188_set_num 会改显示模块的全局 state，
// 如果在中断里直接改显示，就会和 2ms 的扫描任务抢同一个变量。
// =====================================================================
volatile int32_t g_ec12_pulse = 0;

// =====================================================================
// ADKey 短按回调
//   i: 0 -> KEY1, 1 -> KEY2, 2 -> KEY3
//
// 这个回调是在 ADKey_scan() 里被调用的，也就是在任务上下文，
// 不是中断上下文，所以可以直接调业务函数。
// =====================================================================
void ADCKey_on_click(uint8_t i) {
	switch (i) {
	case 0:                        // KEY1：模式切换
		App_fryer_mode_toggle();
		break;

	case 1:                        // KEY2：运行 / 暂停
		App_fryer_run_toggle();
		break;

	default:                       // KEY3 短按：无功能（KEY3 只用长按）
		break;
	}
}

// =====================================================================
// ADKey 长按回调
//   KEY3 长按 2000ms -> 开关机
//   阈值在 bsp_adkey.c 的 ADKEY_LONG_MS 里，按考试2.md 的「长按2秒」设成 2000
// =====================================================================
void ADCKey_on_long_click(uint8_t i) {
	if (i == 2) {                  // KEY3
		App_fryer_power_toggle();
	}
}

// =====================================================================
// 编码器旋转回调（在 EXTI10_15 中断里执行，只做计数）
// =====================================================================
void EC12_on_turn_right(void) {    // 顺时针 / 右转 -> 增加
	g_ec12_pulse++;
}

void EC12_on_turn_left(void) {     // 逆时针 / 左转 -> 减少
	g_ec12_pulse--;
}

// =====================================================================
// 按键/编码器任务：KEY_SCAN_MS 轮询一次
//
// ADKey 自己的采样与短按/长按状态机在 ADKey_scan() 里，
// 由 App_adkey_scan_task 以 1ms 周期单独调用（见 App_fryer.c）。
// 这里只负责把编码器脉冲取走并换算成步进。
// =====================================================================
void App_key_scan_task(void) {
	int32_t pulse;
	int32_t step;
	static int32_t acc = 0;   // 不足一格的残余脉冲

	// 关机时把残余脉冲一起清掉。
	// 否则关机前转半格留下的残余，会在开机后第一次旋转时"突然多跳一格"，
	// 表现为温度莫名其妙多走 5 度。
	if (!g_power_on) {
		acc = 0;

		// 同样用 PRIMASK 保存/恢复，不要直接 __enable_irq()，
		// 否则万一外层本来就在临界区，会被这里误打开。
		uint32_t pm = __get_PRIMASK();
		__disable_irq();
		g_ec12_pulse = 0;
		__set_PRIMASK(pm);
		return;
	}

	// 关中断取走计数，避免和中断里的 ++ / -- 抢同一个变量。
	// 用 PRIMASK 保存/恢复，这样即使外面已经在临界区也不会被误打开。
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	pulse = g_ec12_pulse;
	g_ec12_pulse = 0;
	__set_PRIMASK(primask);

	if (pulse == 0) return;

	// 累加残余，每满 EC12_PULSES_PER_STEP 个脉冲算一格步进
	acc  += pulse;
	step  = acc / EC12_PULSES_PER_STEP;
	if (step == 0) return;
	acc  -= step * EC12_PULSES_PER_STEP;

	App_fryer_value_add(step);
}

// =====================================================================
// ADKey 采样任务：ADKEY_SCAN_MS 轮询一次
//
// 必须是 1ms。驱动内部是「每 1ms 采 1 次 ADC、累加 50 次出一帧」，
// 周期一旦拉长，出帧时间跟着拉长，短按/长按的判定时刻就会偏移。
// =====================================================================
void App_adkey_scan_task(void) {
	ADKey_scan();
}
