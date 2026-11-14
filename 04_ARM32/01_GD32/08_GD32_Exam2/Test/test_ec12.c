/* =====================================================================
 * EC12 判向回归测试（主机侧，编译真实的 bsp_ec12.c）
 *
 * 两套判向方案都测：
 *   EC12_USE_QUADRATURE = 1 -> 转一格 4 个单步（正交状态机）
 *   EC12_USE_QUADRATURE = 0 -> 转一格 2 个脉冲（只在 A 边沿判向）
 *
 * 用法（工程根目录下）：
 *   gcc -std=c11 -I Test/stub -I Hardware -I Library \
 *       -o Test/test_ec12.exe Test/test_ec12.c Hardware/bsp_ec12.c
 *   ./Test/test_ec12.exe
 *
 * 注意：bsp_ec12.c 必须【单独编译】，不能 include 进来。
 *   weak 属性只在跨翻译单元链接时生效；include 会变成同一个 TU，
 *   本文件里的强定义会和 bsp_ec12.c 里的 weak 定义直接报重定义。
 * ===================================================================== */
#include <stdio.h>
#include <stdint.h>

#include "gpio_cfg.h"      /* Test/stub 提供：引脚宏 + gpio_input_bit_get 声明 */
#include "EXTI.h"          /* EXTI11_on_trig / EXTI13_on_trig 声明 */
#include "bsp_ec12.h"      /* 关键：拿到 EC12_USE_QUADRATURE 开关，否则测试的期望值会和驱动方案不一致 */

/* ---------- 桩：由测试控制 A/B 电平 ---------- */
static int g_a_level = 0;
static int g_b_level = 0;

FlagStatus gpio_input_bit_get(int port, uint32_t pin) {
    (void)port;
    if (pin == GPIO_PIN_11) return g_a_level ? SET : RESET;
    if (pin == GPIO_PIN_13) return g_b_level ? SET : RESET;
    return SET;                 /* PD15 默认抬起 */
}

void GPIO_output(int rcu, int port, int pin, int otype, int flag) {
    (void)rcu; (void)port; (void)pin; (void)otype; (void)flag;
}
void GPIO_input(int rcu, int port, int pin, int pull) {
    (void)rcu; (void)port; (void)pin; (void)pull;
}

/* ---------- 计数回调（覆盖 bsp_ec12.c 的 weak 定义，跨 TU 生效）---------- */
static int g_right = 0, g_left = 0;

void EC12_on_turn_right(void) { g_right++; }
void EC12_on_turn_left(void)  { g_left++;  }

/* ---------- 断言 ---------- */
static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg) do { \
    if (cond) g_pass++; \
    else { g_fail++; printf("  [FAIL] %s\n", msg); } \
} while (0)

/* 设置 AB 电平（state = (A<<1)|B），并调真实的中断入口 */
static void set_state(int st) {
    int prev_a = g_a_level, prev_b = g_b_level;
    g_a_level = (st >> 1) & 1;
    g_b_level = st & 1;

    if (g_a_level != prev_a) EXTI11_on_trig();
#if EC12_USE_QUADRATURE
    if (g_b_level != prev_b) EXTI13_on_trig();   /* 只有新方案 B 相进中断 */
#else
    (void)prev_b;
#endif
}

static void walk(const int *seq, int n) {
    int i;
    for (i = 0; i < n; i++) set_state(seq[i]);
}

static void reset_to_00(void) {
    g_right = 0; g_left = 0;
    g_a_level = 0; g_b_level = 0;
    EXTI11_on_trig();
    g_right = 0; g_left = 0;
}

int main(void) {
#if EC12_USE_QUADRATURE
    const int PER_TURN = 4;
    const char *mode = "正交状态机（A/B 都进中断）";
#else
    const int PER_TURN = 2;
    const char *mode = "老方案（只在 A 边沿判向）";
#endif

    printf("===== EC12 判向回归测试 =====\n");
    printf("方案: %s\n", mode);
    printf("每格理想脉冲数: %d\n\n", PER_TURN);

    printf("[1] 右转一格\n");
    reset_to_00();
    { int seq[] = {2, 3, 1, 0}; walk(seq, 4); }
    CHECK(g_right == PER_TURN, "右转一格 right 回调数应等于每格脉冲数");
    CHECK(g_left == 0,         "右转一格不应产生 left");
    printf("    right=%d left=%d（期望 %d/0）\n", g_right, g_left, PER_TURN);

    printf("[2] 右转三格\n");
    reset_to_00();
    { int seq[] = {2,3,1,0, 2,3,1,0, 2,3,1,0}; walk(seq, 12); }
    CHECK(g_right == PER_TURN * 3, "右转三格应 3 倍");
    CHECK(g_left == 0,             "右转三格不应产生 left");
    printf("    right=%d left=%d（期望 %d/0）\n", g_right, g_left, PER_TURN * 3);

    printf("[3] 左转一格\n");
    reset_to_00();
    { int seq[] = {1, 3, 2, 0}; walk(seq, 4); }
    CHECK(g_left == PER_TURN, "左转一格 left 回调数应等于每格脉冲数");
    CHECK(g_right == 0,       "左转一格不应产生 right");
    printf("    right=%d left=%d（期望 0/%d）\n", g_right, g_left, PER_TURN);

    printf("[4] 左转三格\n");
    reset_to_00();
    { int seq[] = {1,3,2,0, 1,3,2,0, 1,3,2,0}; walk(seq, 12); }
    CHECK(g_left == PER_TURN * 3, "左转三格应 3 倍");
    CHECK(g_right == 0,           "左转三格不应产生 right");
    printf("    right=%d left=%d（期望 0/%d）\n", g_right, g_left, PER_TURN * 3);

    printf("[5] 右转一格再左转一格\n");
    reset_to_00();
    { int seq[] = {2,3,1,0, 1,3,2,0}; walk(seq, 8); }
    CHECK(g_right == PER_TURN && g_left == PER_TURN, "应为对称");
    CHECK(g_right - g_left == 0, "净值应回 0");
    printf("    right=%d left=%d 净值=%+d（期望 %d/%d 净值 0）\n",
           g_right, g_left, g_right - g_left, PER_TURN, PER_TURN);

    printf("\n===== 结果：通过 %d，失败 %d =====\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
