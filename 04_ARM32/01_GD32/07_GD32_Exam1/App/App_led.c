#include "App.h"

// 本任务由 10ms 的槽位调用（见 User/Task.c 的任务表）。
// 250ms 和 125ms 都能被 10 整除，所以"到点检测"正好落在步长整数倍上，
// 实测间隔精确 250ms / 125ms，不会有 ±10ms 抖动。
// 时间基准统一用 g_t5_tick_ms（Timer5 每 1ms 累加），不自己数调用次数。
extern volatile uint64_t g_t5_tick_ms;

// ============================================================
// 流水灯状态
//   步序号 0~8：0~7 -> 亮 (step+1) 颗；8 -> 全灭
//   累积点亮：1 -> 1,2 -> 1,2,3 -> ... -> 全亮 -> 全灭 -> 循环
// ============================================================
static uint8_t  s_flow_step  = 0;
static uint8_t  s_flow_armed = 0;     // 0 未启动 / 1 已启动
static uint64_t s_flow_base  = 0;     // 本模式启动时的 tick

// ============================================================
// 节拍器状态
//   步序号 0~7，每步 125ms，8 步 = 1 秒
//   位模式：LED1 = step<4，LED2 = step%4<2，LED3 = step%2==0
// ============================================================
static uint8_t  s_metro_step  = 0;
static uint8_t  s_metro_armed = 0;
static uint64_t s_metro_base  = 0;

// 只控制 LED1~LED3，其余全灭
static void Metro_apply(uint8_t on1, uint8_t on2, uint8_t on3) {
	Leds_turn(1, on1);
	Leds_turn(2, on2);
	Leds_turn(3, on3);
	Leds_turn(4, 0);
	Leds_turn(5, 0);
	Leds_turn(6, 0);
	Leds_turn(7, 0);
	Leds_turn(8, 0);
}

static void Metro_update(uint8_t step) {
	uint8_t on1 = (step < 4) ? 1 : 0;
	uint8_t on2 = ((step % 4) < 2) ? 1 : 0;
	uint8_t on3 = ((step % 2) == 0) ? 1 : 0;
	Metro_apply(on1, on2, on3);
}

// ============== LED 业务：10ms 轮询一次
void App_led_task(void) {
	uint64_t now = g_t5_tick_ms;

	// ---------------- 流水灯 ----------------
	if (g_flow_running) {
		if (s_flow_armed == 0) {
			s_flow_armed = 1;
			s_flow_base  = now;
			s_flow_step  = 0;
			Leds_turn_all(0);      // 先全灭，保证起点干净
			Leds_turn_on(1);       // 第 0 步：亮 1 颗
			return;                // 本拍先输出，下一拍再判断换步
		}

		uint32_t elapsed = (uint32_t)((now - s_flow_base) / FLOW_STEP_MS);
		uint32_t target  = elapsed % FLOW_STEP_TOTAL;

		if (target != s_flow_step) {
			s_flow_step = (uint8_t)target;

			if (s_flow_step < 8) {
				// 累积点亮：只点亮新的一颗，前面的保持
				Leds_turn_on(s_flow_step + 1);
			} else {
				// 第 8 步：全灭，下一轮从 1 颗重新累积
				Leds_turn_all(0);
			}
		}
	} else {
		// 流水灯已停止。这里【故意不动 LED】—— 停止的语义是"定格"，
		// 亮着几颗就让它保持几颗常亮。
		// 需要清场的情况（比如切到节拍器）由 App_key.c 负责清。
		s_flow_armed = 0;
		s_flow_step  = 0;
	}

	// ---------------- 节拍器 ----------------
	if (g_metro_running) {
		if (s_metro_armed == 0) {
			s_metro_armed = 1;
			s_metro_base  = now;
			s_metro_step  = 0;
			Metro_update(0);       // 立即输出第 0 步，按下即可见
			return;
		}

		uint32_t elapsed = (uint32_t)((now - s_metro_base) / METRO_STEP_MS);
		uint32_t target  = elapsed % METRO_STEP_TOTAL;

		if (target != s_metro_step) {
			s_metro_step = (uint8_t)target;
			Metro_update(s_metro_step);
		}
	} else {
		// 节拍器停止时，App_key.c 已经执行了 Leds_turn_all(0) 清空，
		// 这里只复位内部计时状态即可，不要再碰 LED。
		s_metro_armed = 0;
		s_metro_step  = 0;
	}
}
