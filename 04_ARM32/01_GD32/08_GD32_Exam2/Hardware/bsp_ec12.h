#ifndef __BSP_EC12_H__
#define __BSP_EC12_H__

#include "gpio_cfg.h"

//////////////////// EC12 旋钮 ////////////////////
/*  接线说明
交互板           GD32F407
        A  ===>  PD11
        B  ===>  PD13
        D  ===>  PD15
       3v3 ===>  3v3
       GND ===>	 GND
*/

//// 外部中断实现
#define EC12_A_RCU			RCU_GPIOD
#define EC12_A_PIN			GPIOD, GPIO_PIN_11
#define EC12_A_STATE()		gpio_input_bit_get(EC12_A_PIN)
#define EC12_A_ON_TRIG()	EXTI11_on_trig() // 外部中断11回调函数

#define EC12_B_RCU			RCU_GPIOD
#define EC12_B_PIN			GPIOD, GPIO_PIN_13
#define EC12_B_STATE()		gpio_input_bit_get(EC12_B_PIN)
#define EC12_B_ON_TRIG()	EXTI13_on_trig() // 外部中断13回调函数（B相）

#define EC12_D_RCU			RCU_GPIOD
#define EC12_D_PIN			GPIOD, GPIO_PIN_15
#define EC12_D_STATE()		gpio_input_bit_get(EC12_D_PIN)
#define EC12_D_ON_TRIG()	EXTI15_on_trig() // 外部中断15回调函数

//// 中断实现 
void EC12_init();

#define EC12_USE_DOWN	1
#define EC12_USE_UP		1
void EC12_on_down();
void EC12_on_up();

#define EC12_USE_RIGHT	1
#define EC12_USE_LEFT	1
void EC12_on_turn_right(); // 顺时针，在时钟的上半部分，指针是向右移动的
void EC12_on_turn_left();  // 逆时针，在时钟的上半部分，指针是向左移动的

// =====================================================================
// 判向方案开关
//
// 0 = 老方案：只在 A 相边沿读 B 判方向（原始课程代码）
//     转一格产生 2 个脉冲 -> App.h 里 EC12_PULSES_PER_STEP 要配 1 或 2
//     ★ 用户 2026-11-05 实测：配 1 时手感正常，可用
//     缺点：A 边沿那一刻若 B 正在抖动，方向会判反，表现为「偶尔转两次才动」
//
// 1 = 新方案：正交状态机，A、B 两相都进中断
//     转一格产生 4 个脉冲 -> App.h 里 EC12_PULSES_PER_STEP 必须配 4
//     优点：抖动回弹被 +1/-1 抵消，不破坏真实计数
//     代价：需要把 B 相（PD13）也接进 EXTI13（EXTI_config.h 的 USE_EXTI_13）
//     ★ 未经真机验证
//
// 两套互斥，切换时 EC12_PULSES_PER_STEP 必须跟着改，见 App.h 的说明。
// =====================================================================
#define EC12_USE_QUADRATURE	0

#endif