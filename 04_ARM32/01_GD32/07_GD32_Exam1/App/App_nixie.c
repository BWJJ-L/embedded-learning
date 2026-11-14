#include "App.h"

// 本任务由 2ms 的槽位调用，负责 8 位数码管的动态扫描。
extern volatile uint64_t g_t5_tick_ms;

// ============ 数码管显示状态 ============
// 计时方式：记下启动时刻，每拍用 (now - base) / 400 算目标位，避免误差累计
static uint8_t  s_nixie_index = 0;        // 当前轮到第几位，0~7 对应 1~8 位
static uint8_t  s_nixie_armed = 0;
static uint64_t s_nixie_base  = 0;

// 动态扫描的位置，0~7 对应位码 0x01~0x80
static uint8_t  s_scan_pos = 0;

// 当前要显示的内容（段码表下标，0~7 表示数字 0~7）
// 0xFF 表示"什么都不显示"，即整屏熄灭
static uint8_t  s_nixie_digit = 0xFF;

// ============ 400ms 流水推进 ============
// 需求：1号管显示0、2号显示1 …… 8号显示7，间隔 400ms，每轮只有一位亮
static void Nixie_flow_update(uint64_t now) {
	// 刚启动：立即显示第 1 位，数字 0
	if (s_nixie_armed == 0) {
		s_nixie_armed = 1;
		s_nixie_base  = now;
		s_nixie_index = 0;
		s_nixie_digit = 0;
		return;
	}

	uint32_t elapsed = (uint32_t)((now - s_nixie_base) / NIXIE_STEP_MS);
	uint32_t target  = elapsed % NIXIE_STEP_TOTAL;

	if (target != s_nixie_index) {
		s_nixie_index = (uint8_t)target;
		s_nixie_digit = s_nixie_index;   // 第 n 位显示数字 n-1
	}
}

// ============== 数码管业务：2ms 轮询一次 ==============
// 两部分：
//   1) 400ms 的流水推进（决定哪一位、显示什么数字）
//   2) 2ms 的动态扫描（把当前内容真正送到 595）
void App_nixie_task(void) {
	uint64_t now = g_t5_tick_ms;

	// ---- 1) 业务推进 ----
	if (g_nixie_running) {
		Nixie_flow_update(now);
	} else {
		// 数码管已停止。停止的语义是【定格】：
		//   s_nixie_index 和 s_nixie_digit 保持最后一帧不动，
		//   下面的扫描继续按它输出，所以屏幕上那个数字会一直亮着。
		//
		// 这里只把"启动计时"清掉，这样下次按 KEY3 会从当前位继续
		// （因为 armed=0 会重新对齐 base，index 不变 = 保持原位）。
		s_nixie_armed = 0;
	}

	// ---- 2) 动态扫描 ----
	// 每次只点亮一位：扫描位置等于"当前该亮的位"就显示数字，否则整屏熄灭。
	// 595 每次 Nixie_show 都完整刷新 16 位（段码+位码），所以送一次即可。
	if (s_nixie_digit == 0xFF) {
		Nixie_clear();                                    // 无内容 -> 全灭
	} else if (s_scan_pos == s_nixie_index) {
		Nixie_display(s_nixie_digit, s_scan_pos + 1);     // 目标位 -> 显示数字
	} else {
		Nixie_clear();                                    // 其余位 -> 熄灭
	}

	// 扫到下一位
	s_scan_pos++;
	if (s_scan_pos >= NIXIE_STEP_TOTAL) s_scan_pos = 0;
}
