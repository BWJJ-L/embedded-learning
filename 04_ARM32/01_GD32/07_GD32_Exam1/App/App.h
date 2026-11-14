#ifndef __APP_H__
#define __APP_H__

#include "gpio_cfg.h"

// ======= 包含驱动头文件
#include "bsp_keys.h"
#include "bsp_leds.h"
#include "bsp_buzzer.h"
#include "Nixie.h"

// ============ 功能划分（按"用哪个外设"分组，不是按"全局互斥"）
//
// 四个按键是独立的业务，但它们对硬件资源的占用不同：
//
//   KEY1 流水灯   ┐ 都用 LED 灯组(LED1~8)
//   KEY2 节拍器   ┘ => 这两个互斥，后按的接管，先按的停
//
//   KEY3 数码管   -> 只占数码管，与 LED / 蜂鸣器 完全独立，可以同时跑
//   KEY4 音乐     -> 只占蜂鸣器，与 LED / 数码管 完全独立，可以同时跑
//
// 所以这里不用"一个全局模式"来表达状态，而是每个"资源"一个开关：
// 这样 KEY1 跑流水灯时按 KEY3，数码管照样能工作，互不打断。

// ============ LED 灯组占用者
// 0 = LED 没人用；1 = 流水灯；2 = 节拍器
typedef enum {
	LED_OWNER_NONE = 0,
	LED_OWNER_FLOW,        // KEY1
	LED_OWNER_METRONOME,   // KEY2
} LedOwner_t;

extern volatile LedOwner_t g_led_owner;   // 谁在控制 LED 灯组

// ============ 三个独立的使能开关
extern volatile uint8_t g_flow_running;    // KEY1 流水灯是否在跑
extern volatile uint8_t g_metro_running;   // KEY2 节拍器是否在跑
extern volatile uint8_t g_nixie_running;   // KEY3 数码管是否在跑
extern volatile uint8_t g_music_running;   // KEY4 音乐是否在播

// ============ 时间常量（单位 ms，时基来自 TIMER5 的 1ms 中断）
// 流水灯：250ms 一步，9 步一轮（8 次累积点亮 + 1 次全灭）
#define FLOW_STEP_MS        250
#define FLOW_STEP_TOTAL     9

// 节拍器：125ms 一步，8 步 = 1 秒
#define METRO_STEP_MS       125
#define METRO_STEP_TOTAL    8

// 数码管流水：400ms 一步，8 步一轮
#define NIXIE_STEP_MS       400
#define NIXIE_STEP_TOTAL    8

// 数码管动态扫描：2ms 一位，8 位一轮 16ms = 62.5Hz
#define NIXIE_SCAN_MS       2

// 音乐：半拍时长
#define MUSIC_HALF_BEAT_MS  250

// ============ 业务函数（无参、无返回值，供任务表调用）
void App_key_scan_task(void);   // 10ms
void App_led_task(void);        // 10ms（流水灯 + 节拍器，共享 LED 灯组）
void App_nixie_task(void);      // 2ms（数码管流水 + 动态扫描）
void App_buzzer_task(void);     // 20ms（音乐播放）

// ============ 初始化
static inline void App_init(void) {
	Leds_init();     // LED 灯组，含 PC6 总开关
	Nixie_init();    // 数码管
	Keys_init();     // 独立按键
	Buzzer_init();   // 蜂鸣器
}

#endif
