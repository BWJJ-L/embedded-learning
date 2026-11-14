#include "App.h"

// 本工程只用 4 位数码管显示 0~9999
#define NIXIE_SCAN_DIGITS   4

// 数码管动态扫描状态
// 当前扫描到第几位，1~NIXIE_SCAN_DIGITS
static uint8_t s_nixie_pos = 1;

// 上一次拆好的 4 个待显示数字，避免每次都做除法
static uint8_t s_digit[4] = {0, 0, 0, 0};

// 上一次拆解的数值，用于判断要不要重新拆
static uint16_t s_last_value = 0xFFFF;

// 把 0~9999 的数值拆成 4 位，高位在前
static void Nixie_split(uint16_t value) {
	if (value > 9999) value = 9999;

	s_digit[0] = (uint8_t)(value / 1000 % 10);
	s_digit[1] = (uint8_t)(value / 100 % 10);
	s_digit[2] = (uint8_t)(value / 10 % 10);
	s_digit[3] = (uint8_t)(value % 10);
}

// ============== 数码管业务：2ms 轮询一次
//
// 扫描原理：
//   Nixie_show(段码, 位码) 一次移出 16 位 —— 先 8 位段码，再 8 位位码。
//   595 级联后位码落在第一级、段码落在第二级，所以每次调用都完整刷新了
//   "哪几位导通 + 这几位显示什么"，位码为 0 的位自然熄灭。
//   因此这里不需要先 Nixie_clear()，那样只会白白损失亮度。
//
// 只扫 4 位而不是 8 位：
//   本工程用 4 位显示 0~9999，剩下 4 位不用。
//   只扫 4 位 => 一轮 4 * 2ms = 8ms => 刷新率 125Hz，且每位亮度是扫 8 位的两倍。
//
// 位码的排布：
//   Nixie_display(num, idx) 里 idx 取 1~8，位码是 1 << (idx-1)。
//   这里取 idx = 1~4，对应位码 0x01 / 0x02 / 0x04 / 0x08。
void App_nixie_task(void) {
	// 数值变了才重新拆分
	if (s_last_value != g_nixie_value) {
		s_last_value = g_nixie_value;
		Nixie_split(g_nixie_value);
	}

	// 暂停时整屏熄灭，但仍保留扫描节拍，恢复后立即正常
	if (g_run == 0) {
		Nixie_clear();
		s_nixie_pos = 1;
		return;
	}

	// 只扫第 1~4 位，每位显示对应数字
	Nixie_display(s_digit[s_nixie_pos - 1], s_nixie_pos);

	// 推到下一位，4 位循环
	s_nixie_pos++;
	if (s_nixie_pos > NIXIE_SCAN_DIGITS) s_nixie_pos = 1;
}
