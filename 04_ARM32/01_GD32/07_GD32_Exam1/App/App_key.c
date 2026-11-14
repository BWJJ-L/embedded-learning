#include "App.h"

// ============ 全局状态
// 谁在控制 LED 灯组。KEY1 和 KEY2 共用 LED 灯组，所以它们靠这个变量互斥。
volatile LedOwner_t g_led_owner = LED_OWNER_NONE;

// 四个独立的使能开关，彼此不影响
volatile uint8_t g_flow_running  = 0;   // KEY1 流水灯
volatile uint8_t g_metro_running = 0;   // KEY2 节拍器
volatile uint8_t g_nixie_running = 0;   // KEY3 数码管
volatile uint8_t g_music_running = 0;   // KEY4 音乐

// ============================================================
// KEY1：流水灯（累积点亮）
//   启动：1 -> 1,2 -> ... -> 全亮 -> 全灭 -> 循环
//   再次按下停止：【定格】亮着几颗就保持几颗常亮
//   LED 灯组与 KEY2 冲突，所以启动前把节拍器停掉
// ============================================================
static void App_toggle_flow(void) {
	if (g_flow_running) {
		// 停止：只清标志，不动 LED —— 这样灯就定格在当前状态
		g_flow_running = 0;
		if (g_led_owner == LED_OWNER_FLOW) {
			g_led_owner = LED_OWNER_NONE;
		}
		return;
	}

	// 启动：LED 灯组被节拍器占用时，先接管过来
	if (g_led_owner == LED_OWNER_METRONOME) {
		g_metro_running = 0;
		Leds_turn_all(0);        // 节拍器的灯态不能留，否则流水灯起点不干净
	}

	g_led_owner    = LED_OWNER_FLOW;
	g_flow_running = 1;
}

// ============================================================
// KEY2：节拍器
//   启动：LED1/2/3 同时按各自节拍闪
//   再次按下停止：【清空】LED1~8 全灭
//   LED 灯组与 KEY1 冲突，所以启动前把流水灯停掉
// ============================================================
static void App_toggle_metronome(void) {
	if (g_metro_running) {
		g_metro_running = 0;
		if (g_led_owner == LED_OWNER_METRONOME) {
			g_led_owner = LED_OWNER_NONE;
		}
		Leds_turn_all(0);        // 节拍器停止 = 清空
		return;
	}

	// 启动：LED 灯组被流水灯占用时，先接管过来
	if (g_led_owner == LED_OWNER_FLOW) {
		g_flow_running = 0;
		Leds_turn_all(0);        // 流水灯的定格灯态要清掉
	}

	g_led_owner     = LED_OWNER_METRONOME;
	g_metro_running = 1;
}

// ============================================================
// KEY3：数码管流水
//   只占数码管，与 LED、蜂鸣器都无关 —— 可以和 KEY1/KEY2/KEY4 同时运行
//   再次按下停止：【定格】当前显示什么就保持什么，不熄灭
// ============================================================
static void App_toggle_nixie(void) {
	g_nixie_running = !g_nixie_running;
	// 停止时不动数码管：App_nixie_task 会保持最后一帧的内容继续扫描输出，
	// 所以屏幕上那个数字会一直亮着，直到再次按下 KEY3。
}

// ============================================================
// KEY4：音乐（两只老虎）
//   只占蜂鸣器，与 LED、数码管都无关 —— 可以同时运行
//   再次按下是【暂停】，不是复位；再按一次【从断点续播】
// ============================================================
static void App_toggle_music(void) {
	if (g_music_running) {
		g_music_running = 0;     // 暂停：只停发声
		Buzzer_stop();           // 播放进度(s_music_idx)保留在 App_buzzer.c 里
	} else {
		g_music_running = 1;     // 续播：从暂停处的音符继续
	}
}

// index 取值 1~4
// 本函数由 Keys_scan 在任务上下文调用（不在中断里）
void Keys_on_keydown(uint8_t index) {
	switch (index) {
	case 1: App_toggle_flow();      break;   // 流水灯
	case 2: App_toggle_metronome(); break;   // 节拍器
	case 3: App_toggle_nixie();     break;   // 数码管流水
	case 4: App_toggle_music();     break;   // 音乐
	default: break;
	}
}

// 按键抬起回调，本工程不需要
void Keys_on_keyup(uint8_t index) {
	(void)index;
}

// ============== 按键业务：10ms 轮询一次
void App_key_scan_task(void) {
	Keys_scan();
}
