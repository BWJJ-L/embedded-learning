#include "bsp_ec12.h"

//// 中断实现 
void EC12_init() {
	printf("==EC12_init==\n");
	
	// B相：原理图上 B 有 R22(10k) 外部上拉到 3V3，
	// 所以这里配浮空输入是对的，不要改成内部上拉（会和外部上拉并联）。
	GPIO_input(EC12_B_RCU, EC12_B_PIN, GPIO_PUPD_NONE);
}

// EC12_D_ON_TRIG() 为 EXTI15_on_trig() 的别名，.h 有对应的宏定义
void EC12_D_ON_TRIG() {
	// 抬起 是 高    按下 是 低
	static FlagStatus last_state = SET;
	FlagStatus cur_state = EC12_D_STATE();

	if (last_state != cur_state) {
		last_state = cur_state;

		if (cur_state == RESET) {
			#if EC12_USE_DOWN
			EC12_on_down();
			#endif
		} else {
			#if EC12_USE_UP
			EC12_on_up();
			#endif
		}
	}
}

/* =====================================================================
 * EC12 判向：两套方案，用 bsp_ec12.h 的 EC12_USE_QUADRATURE 切换
 * ===================================================================== */

#if EC12_USE_QUADRATURE

/* ---------- 方案 1：正交状态机（A、B 两相都进中断）----------
 *
 * 状态 = (A << 1) | B，按转移查表计数。
 *   一个完整周期 = 4 次合法单步；
 *   抖动回弹表现为 +1/-1 交替，净值抵消，不破坏真实计数；
 *   跳 2 步的非法转移直接丢弃，不会误计。
 *
 * 方向约定的来源：老方案的规则是
 *   「A 下降沿时 B 高 = 右转；A 上升沿时 B 低 = 右转」。
 * 用穷举求与该规则一致的四态闭环，得到
 *   右转 0 -> 2 -> 3 -> 1 -> 0
 *   左转 0 -> 1 -> 3 -> 2 -> 0
 * 下面这张表按「右转每步 +1、左转每步 -1」填出。
 * （曾填反过一次导致左右颠倒，已用穷举 + 主机测试复核）
 *
 * 注意：用这套时 App.h 的 EC12_PULSES_PER_STEP 必须是 4。
 * ------------------------------------------------------------------ */

static const int8_t quad_table[16] = {
    /*              new=0  new=1  new=2  new=3  */
    /* old=0 */      0,    -1,    +1,     0,
    /* old=1 */     +1,     0,     0,    -1,
    /* old=2 */     -1,     0,     0,    +1,
    /* old=3 */      0,    +1,    -1,     0,
};

static uint8_t ec12_last_ab = 0;   // 上一次的状态

static uint8_t EC12_read_ab(void) {
    uint8_t a = (EC12_A_STATE() == SET) ? 1 : 0;
    uint8_t b = (EC12_B_STATE() == SET) ? 1 : 0;
    return (uint8_t)((a << 1) | b);
}

static void EC12_on_ab_change(void) {
    uint8_t now_ab = EC12_read_ab();
    int8_t  d;

    if (now_ab == ec12_last_ab) return;

    d = quad_table[ec12_last_ab * 4 + now_ab];
    ec12_last_ab = now_ab;

    if (d > 0) {
        #if EC12_USE_RIGHT
        EC12_on_turn_right();
        #endif
    } else if (d < 0) {
        #if EC12_USE_LEFT
        EC12_on_turn_left();
        #endif
    }
    // d == 0：非法转移（一次跳了 2 步，通常是被抖动打断），不计
}

// A 相中断回调（EXTI11，PD11）
// 注意：不能写成 void EC12_A_ON_TRIG(void) ——
//   EC12_A_ON_TRIG() 是宏，会展开成 EXTI11_on_trig()，
//   当函数名定义会报 "too many arguments to macro"。
void EXTI11_on_trig(void) {
    EC12_on_ab_change();
}

// B 相中断回调（EXTI13，PD13），同理
void EXTI13_on_trig(void) {
    EC12_on_ab_change();
}

#else  /* EC12_USE_QUADRATURE == 0：老方案 */

/* ---------- 方案 0：只在 A 相边沿读 B 判方向（原始课程代码）----------
 *
 * 判据：
 *   A 下降沿 & B 高 -> 右转
 *   A 下降沿 & B 低 -> 左转
 *   A 上升沿 & B 低 -> 右转
 *   A 上升沿 & B 高 -> 左转
 *
 * 转一格只产生 2 个 A 边沿 = 2 个脉冲，
 * 所以 App.h 的 EC12_PULSES_PER_STEP 配 1（转一格走 2 步）
 * 或 2（转一格走 1 步）。用户 2026-11-05 实测配 1 手感正常。
 *
 * 已知缺陷：A 边沿那一刻若 B 正在机械抖动，方向会判反，
 * 两个脉冲互相抵消 -> 那一格不走步 -> 「偶尔转两次才动一格」。
 * 想彻底解决就切到 EC12_USE_QUADRATURE = 1。
 * ------------------------------------------------------------------ */

void EXTI11_on_trig(void) {
    static FlagStatus last_a_state = SET;
    FlagStatus cur_a_state = EC12_A_STATE();

    if (last_a_state != cur_a_state) {
        last_a_state = cur_a_state;

        FlagStatus cur_b_state = EC12_B_STATE();

        if (cur_a_state == RESET) {              // A 下降沿
            if (cur_b_state == SET) {
                #if EC12_USE_RIGHT
                EC12_on_turn_right();
                #endif
            } else {
                #if EC12_USE_LEFT
                EC12_on_turn_left();
                #endif
            }
        } else {                                 // A 上升沿
            if (cur_b_state == RESET) {
                #if EC12_USE_RIGHT
                EC12_on_turn_right();
                #endif
            } else {
                #if EC12_USE_LEFT
                EC12_on_turn_left();
                #endif
            }
        }
    }
}

// 老方案不用 B 相，但 EXTI_config.h 里 USE_EXTI_13 仍为 1，
// 必须提供空回调，否则链接报 Undefined symbol EXTI13_on_trig。
// 想彻底不占这个中断，就把 USE_EXTI_13 改回 0。
void EXTI13_on_trig(void) {
}

#endif /* EC12_USE_QUADRATURE */

void EC12_on_down() {
	printf("down\n");
}

void EC12_on_up() {
	printf("up\n");
}

__attribute__((weak)) void EC12_on_turn_right(void) { // 顺时针
	printf("顺时针旋转(右转)\n");
}

__attribute__((weak)) void EC12_on_turn_left(void) {  // 逆时针
	printf("逆时针旋转(左转)\n");
}
