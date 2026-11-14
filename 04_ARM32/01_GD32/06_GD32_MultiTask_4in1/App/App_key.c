#include "App.h"

// 全局运行标志，0 暂停 / 1 运行
volatile uint8_t g_run = 1;

// ============ 各外设当前的业务模式，按键切换后由对应任务读取
// LED 组模式：0 走马灯 / 1 流水灯 / 2 全灭
volatile uint8_t g_led_mode = 0;

// 数码管当前显示的数值
volatile uint16_t g_nixie_value = 0;

// 蜂鸣器开关：0 关 / 1 开
volatile uint8_t g_buzzer_on = 0;

// 按键按下回调，index 取值 1~4
// 本函数由 Keys_scan 在任务上下文调用（不在中断里），可安全修改业务状态
void Keys_on_keydown(uint8_t index) {
	switch (index) {
	// 按键 1：切换 LED 灯组模式
	case 1:
		g_led_mode++;
		if (g_led_mode > 2) g_led_mode = 0;
		break;

	// 按键 2：数码管数值 +1，到 9999 后回 0
	case 2:
		if (g_nixie_value >= 9999) {
			g_nixie_value = 0;
		} else {
			g_nixie_value++;
		}
		break;

	// 按键 3：蜂鸣器开 / 关
	case 3:
		g_buzzer_on = !g_buzzer_on;
		break;

	// 按键 4：全局暂停 / 恢复
	case 4:
		g_run = !g_run;
		break;

	default:
		break;
	}
}

// 按键抬起回调，本工程不需要，留空
void Keys_on_keyup(uint8_t index) {
	(void)index;
}

// ============== 按键业务：10ms 轮询一次
void App_key_scan_task(void) {
	Keys_scan();
}
