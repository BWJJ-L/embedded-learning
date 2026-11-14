#include "App.h"

// LED 灯组业务状态
// 当前点亮的位置 1~8，走马灯/流水灯共用
static uint8_t s_led_pos = 1;

// ============== LED 灯组业务：200ms 轮询一次
// 每次调用只推进一步，绝不在这里写 for + delay 的阻塞循环，
// 否则会拖住数码管扫描，屏幕会闪。
void App_led_task(void) {
	// 全局暂停时，保持当前灯态不动
	if (g_run == 0) return;

	switch (g_led_mode) {
	// 模式 0：走马灯，同一时刻只有 1 个灯亮
	case 0:
        Leds_turn_on(s_led_pos);
		s_led_pos++;
		if (s_led_pos > 9) {
			s_led_pos = 1;
			Leds_turn_all(0);        // 走完一轮全部熄灭，下一轮重新累积
		}
		break;

	// 模式 1：流水灯，新点亮的累积不灭，满 8 个后一起清掉
	case 1:
		
		Leds_turn_all(0);            // 先全灭
		Leds_turn_on(s_led_pos);     // 只点亮当前位置
		s_led_pos++;
		if (s_led_pos > 8) s_led_pos = 1;
		break;

	// 模式 2：全灭
	default:
		Leds_turn_all(0);
		s_led_pos = 1;
		break;
	}
}
