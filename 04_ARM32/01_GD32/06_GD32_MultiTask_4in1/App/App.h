#ifndef __APP_H__
#define __APP_H__

#include "gpio_cfg.h"

// ======= 包含驱动头文件
#include "bsp_keys.h"
#include "bsp_leds.h"
#include "bsp_buzzer.h"
#include "Nixie.h"

// ============ 全局变量声明（定义在 App_key.c）
// 全局运行标志，0 暂停 / 1 运行，按键 4 切换
extern volatile uint8_t g_run;

// LED 组模式：0 走马灯 / 1 流水灯 / 2 全灭，按键 1 切换
extern volatile uint8_t g_led_mode;

// 数码管当前显示的数值，按键 2 递增
extern volatile uint16_t g_nixie_value;

// 蜂鸣器开关：0 关 / 1 开，按键 3 切换
extern volatile uint8_t g_buzzer_on;

// ============ 业务函数（无参、无返回值，供任务表调用）
// ============== 按键业务，10ms
void App_key_scan_task();

// ============== LED 灯组业务，200ms
void App_led_task();

// ============== 数码管业务，5ms（动态扫描，必须高频）
void App_nixie_task();

// ============== 蜂鸣器业务，20ms
void App_buzzer_task();

// ============ 初始化
// 各外设依次初始化，顺序有依赖：
//   1. LED 组先使能 PC6 三极管总开关，否则 PD8~PD15 无效
//   2. 数码管 595 与 LED 组共用 GPIOD，但引脚不重叠
//   3. 蜂鸣器 PB9 复用 Timer1，与 Timer5（系统滴答）不冲突
static inline void App_init(void) {
	Leds_init();     // LED 灯组，含 PC6 总开关
	Nixie_init();    // 数码管
	Keys_init();     // 独立按键
	Buzzer_init();   // 蜂鸣器
}

#endif
