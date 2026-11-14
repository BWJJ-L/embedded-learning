/* =====================================================================
 * 主机侧回归测试：把 App 层的业务状态机拉出来，逐条断言评分表的得分点
 *
 * 用法（在工程根目录 08_GD32_Exam2/ 下）：
 *
 *   gcc -std=c11 -I Test/stub -I App -I Hardware -o Test/test_fryer.exe Test/test_fryer.c
 *   ./Test/test_fryer.exe
 *
 * 退出码 0 = 全部通过，1 = 有失败。
 *
 * 为什么这样做：评分表考的是"交互逻辑"，不是"寄存器配置"。
 * 逻辑对不对，在主机上能逐条断言；寄存器对不对，只能上真机。
 *
 * 注意：本测试直接 include 工程的 App_fryer.c / App_key.c，
 *       不改它们一个字节。所以改了业务代码后重跑本测试即可发现回归。
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* 先把 App.h 拉进来：FlagStatus / SET / RESET 等类型由 Test/stub/gpio_cfg.h 提供，
 * 下面的桩函数定义必须排在它之后 */
#include "App.h"

/* ---------- 硬件桩：记录最后一次状态，供断言使用 ---------- */
static int      g_num       = -1;   /* 数码管显示的数字 */
static int      g_charge    = -1;   /* 闪电标志 */
static int      g_percent   = -1;   /* 百分号标志 */
static int      g_cleared   = 0;    /* 是否调用过 LED188_clear */

/* 彩灯：同时记录"缩放过亮度之后"的最终颜色和最后一次用的亮度参数。
 * 断言颜色要按缩放后的值比 —— 因为产品代码走的是亮度接口。 */
static uint32_t g_led_color[4] = {0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
static int      g_led_bright[4] = {-1, -1, -1, -1};

/* 按驱动里的同一套算法缩放，保证测试与产品的换算口径一致 */
static uint32_t scale_by_brightness(uint32_t color, uint8_t brightness) {
    uint8_t r, g, b;
    if (brightness > 100) brightness = 100;
    r = (uint8_t)((((uint32_t)((color >> 16) & 0xFF)) * brightness + 50) / 100);
    g = (uint8_t)((((uint32_t)((color >>  8) & 0xFF)) * brightness + 50) / 100);
    b = (uint8_t)((((uint32_t)( color        & 0xFF)) * brightness + 50) / 100);
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

void LED188_init(void)    { g_num = -1; g_charge = -1; g_percent = -1; }
void LED188_clear(void)   { g_num = -1; g_charge = 0; g_percent = 0; g_cleared = 1; }
void LED188_display(void) { }
void LED188_set_num(uint8_t n)  { g_num = n; }
void LED188_charge(uint8_t e)   { g_charge = e; }
void LED188_percent(uint8_t e)  { g_percent = e; }

void ADKey_init(void) { }
void ADKey_scan(void) { }
void EC12_init(void)  { }
void WS2812_init(void) {
    int i;
    for (i = 0; i < 4; i++) { g_led_color[i] = 0; g_led_bright[i] = -1; }
}
void WS2812_display(void) { }
void WS2812_set_color(uint16_t idx, uint32_t color) {
    if (idx < 4) g_led_color[idx] = color;
}
void WS2812_set_color_brightness(uint16_t idx, uint32_t color, uint8_t brightness) {
    if (idx < 4) {
        g_led_color[idx]  = scale_by_brightness(color, brightness);
        g_led_bright[idx] = brightness;
    }
}

/* GPIO 桩 */
void GPIO_output(int rcu, int port, int pin, int otype, int flag) {
    (void)rcu; (void)port; (void)pin; (void)otype; (void)flag;
}
void GPIO_input(int rcu, int port, int pin, int pull) {
    (void)rcu; (void)port; (void)pin; (void)pull;
}
FlagStatus gpio_input_bit_get(int port, uint32_t pin) { (void)port; (void)pin; return SET; }

/* 时基：测试里由测试自己推进 */
volatile uint64_t g_t5_tick_ms = 0;

/* 被测代码（原样 include，不改一字节） */
#include "App_fryer.c"
#include "App_key.c"

/* ---------- 断言框架 ---------- */
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, fmt, ...) do {                                        \
    if (cond) { g_pass++; }                                               \
    else {                                                                \
        g_fail++;                                                         \
        printf("  [FAIL] " fmt "\n", ##__VA_ARGS__);                      \
    }                                                                     \
} while (0)

/* 推进时间：按真实任务表的周期同时跑两个任务，模拟真实调度。
 *
 * 【本轮踩过的坑】这里一开始只跑了 App_fryer_task，漏掉 App_key_scan_task，
 * 导致"关机清残余脉冲"那段逻辑根本没被执行 —— 是测试脚本自己的坑，
 * 不是产品代码的问题。 */
static void tick_ms(uint32_t ms) {
    uint32_t i;
    for (i = 0; i < ms; i++) {
        g_t5_tick_ms++;

        if (g_t5_tick_ms % FRYER_TASK_MS == 0)  App_fryer_task();
        if (g_t5_tick_ms % KEY_SCAN_MS == 0)    App_key_scan_task();
    }
}

/* 按键动作封装 */
static void key1_click(void) { ADCKey_on_click(0); }        /* 模式切换 */
static void key2_click(void) { ADCKey_on_click(1); }        /* 运行/暂停 */
static void key3_long(void)  { ADCKey_on_long_click(2); }   /* 开关机 */

/* 转"格"：一格 = EC12_PULSES_PER_STEP 个脉冲。
 * 注意不要和"脉冲"混为一谈 —— 这是本轮测试脚本自己踩过的坑。 */
static void ec12_turn(int cells) {
    int pulses = cells * EC12_PULSES_PER_STEP;
    int i;
    if (pulses > 0) for (i = 0; i < pulses; i++) EC12_on_turn_right();
    else            for (i = 0; i < -pulses; i++) EC12_on_turn_left();
    App_key_scan_task();   /* 消费脉冲 */
}

/* 转"脉冲"（不换算），只用来验证残余累积逻辑 */
static void ec12_turn_pulse(int pulses) {
    int i;
    if (pulses > 0) for (i = 0; i < pulses; i++) EC12_on_turn_right();
    else            for (i = 0; i < -pulses; i++) EC12_on_turn_left();
    App_key_scan_task();
}

/* 复位到上电态 */
static void reset_all(void) {
    g_power_on = 0; g_running = 0; g_mode = FRYER_MODE_TEMP;
    g_temp = FRYER_TEMP_DEFAULT; g_time_set = FRYER_TIME_DEFAULT;
    g_time_left = FRYER_TIME_DEFAULT;
    g_ec12_pulse = 0;
    g_t5_tick_ms = 0;
    g_num = -1; g_charge = -1; g_percent = -1; g_cleared = 0;
    int i; for (i = 0; i < 4; i++) { g_led_color[i] = 0xDEADBEEF; g_led_bright[i] = -1; }
}

int main(void) {
    printf("===== exam2 业务逻辑主机侧回归测试 =====\n");
    printf("EC12_PULSES_PER_STEP = %d\n\n", EC12_PULSES_PER_STEP);

    /* ============================================================
     * 5 分：KEY3 长按开机 —— 数码管点亮、彩灯点亮（默认绿色）
     * ============================================================ */
    printf("[1] KEY3 长按开机\n");
    reset_all();
    key3_long();
    tick_ms(20);
    CHECK(g_power_on == 1, "开机后 g_power_on 应为 1，实际 %d", g_power_on);
    /* 走亮度接口后，绿色 0x00ff00 会按 FRYER_LED_BRIGHTNESS 缩放，不能写死 0x00ff00。
     * 用同一个缩放函数算期望值，保证与产品口径一致。 */
    CHECK(g_led_color[0] == scale_by_brightness(0x00ff00u, FRYER_LED_BRIGHTNESS),
          "开机彩灯应为绿色(按亮度%d缩放)=0x%06X，实际 0x%06X",
          FRYER_LED_BRIGHTNESS, scale_by_brightness(0x00ff00u, FRYER_LED_BRIGHTNESS), g_led_color[0]);
    CHECK(g_led_color[1] == scale_by_brightness(0x00ff00u, FRYER_LED_BRIGHTNESS),
          "第2颗彩灯也应为绿色，实际 0x%06X", g_led_color[1]);
    CHECK(g_led_bright[0] == FRYER_LED_BRIGHTNESS,
          "亮度参数应传 %d，实际 %d", FRYER_LED_BRIGHTNESS, g_led_bright[0]);
    CHECK(g_num >= 0, "开机后数码管应有内容（不是熄灭态），实际 %d", g_num);
    CHECK(g_charge == 1, "开机默认温度模式 -> 闪电标志应亮，实际 %d", g_charge);
    CHECK(g_percent == 0, "开机默认温度模式 -> 百分号标志应灭，实际 %d", g_percent);

    /* ============================================================
     * 5 分：长按开机 数码管显示默认值（温度 100，时间 10）
     * ============================================================ */
    printf("[2] 开机默认值\n");
    CHECK(g_num == 100, "开机应显示默认温度 100，实际 %d", g_num);
    CHECK(g_temp == 100, "g_temp 应为 100，实际 %u", g_temp);
    CHECK(g_time_left == 10, "默认时间应为 10，实际 %u", g_time_left);
    CHECK(g_mode == FRYER_MODE_TEMP, "默认应为温度模式，实际 %d", g_mode);

    /* ============================================================
     * 5 分：长按关机 —— 数码管熄灭、彩灯熄灭
     * ============================================================ */
    printf("[3] KEY3 长按关机\n");
    key3_long();
    tick_ms(20);
    CHECK(g_power_on == 0, "关机后 g_power_on 应为 0，实际 %d", g_power_on);
    CHECK(g_num == -1, "关机后数码管应熄灭（无内容），实际 %d", g_num);
    CHECK(g_led_color[0] == 0x000000u, "关机彩灯应灭 0x000000，实际 0x%06X", g_led_color[0]);

    /* ============================================================
     * 关机状态下所有操作无效
     * ============================================================ */
    printf("[4] 关机时功能无效\n");
    key1_click(); tick_ms(20);
    CHECK(g_mode == FRYER_MODE_TEMP, "关机时 KEY1 不应切模式，实际 %d", g_mode);
    ec12_turn(1); tick_ms(20);
    CHECK(g_temp == 100, "关机时编码器不应改温度，实际 %u", g_temp);
    key2_click(); tick_ms(20);
    CHECK(g_running == 0, "关机时 KEY2 不应启动加热，实际 %d", g_running);

    /* ============================================================
     * 5 分：KEY1 点击模式切换，闪电亮=温度，百分号亮=时间
     * ============================================================ */
    printf("[5] KEY1 模式切换\n");
    reset_all();
    key3_long();
    tick_ms(20);
    CHECK(g_mode == FRYER_MODE_TEMP && g_num == 100 && g_charge == 1 && g_percent == 0,
          "开机态：温度模式 100 度 闪电亮");

    key1_click(); tick_ms(20);
    CHECK(g_mode == FRYER_MODE_TIME, "KEY1 一次 -> 时间模式，实际 %d", g_mode);
    CHECK(g_num == 10, "时间模式应显示时间 10，实际 %d", g_num);
    CHECK(g_percent == 1, "时间模式 -> 百分号应亮，实际 %d", g_percent);
    CHECK(g_charge == 0, "时间模式 -> 闪电应灭，实际 %d", g_charge);

    key1_click(); tick_ms(20);
    CHECK(g_mode == FRYER_MODE_TEMP, "KEY1 两次 -> 回温度模式，实际 %d", g_mode);
    CHECK(g_num == 100, "回温度模式应显示 100，实际 %d", g_num);
    CHECK(g_charge == 1, "回温度模式 -> 闪电应亮，实际 %d", g_charge);
    CHECK(g_percent == 0, "回温度模式 -> 百分号应灭，实际 %d", g_percent);

    /* ============================================================
     * 5 分：没有运行时可以加减；开机默认没有运行
     * ============================================================ */
    printf("[6] 未运行时可以加减\n");
    CHECK(g_running == 0, "开机默认不应在运行，实际 %d", g_running);
    ec12_turn(1); tick_ms(20);
    CHECK(g_temp == 105, "未运行时右转一格应 +5 -> 105，实际 %u", g_temp);

    /* ============================================================
     * 5 分：温度模式步长 5
     * ============================================================ */
    printf("[7] 温度步长 5\n");
    ec12_turn(1); tick_ms(20);
    CHECK(g_temp == 110, "再右转一格 -> 110，实际 %u", g_temp);
    ec12_turn(-1); tick_ms(20);
    CHECK(g_temp == 105, "左转一格 -> 105，实际 %u", g_temp);

    /* ============================================================
     * 5 分：温度模式最小 80、最大 190
     * ============================================================ */
    printf("[8] 温度范围 80~190\n");
    int guard = 0;
    while (g_temp > 80 && guard++ < 100) { ec12_turn(-1); tick_ms(20); }
    CHECK(g_temp == 80, "连续左转应停在下限 80，实际 %u", g_temp);
    ec12_turn(-1); tick_ms(20);
    CHECK(g_temp == 80, "到 80 后继续左转应仍为 80，实际 %u", g_temp);

    guard = 0;
    while (g_temp < 190 && guard++ < 200) { ec12_turn(1); tick_ms(20); }
    CHECK(g_temp == 190, "连续右转应停在上限 190，实际 %u", g_temp);
    ec12_turn(1); tick_ms(20);
    CHECK(g_temp == 190, "到 190 后继续右转应仍为 190，实际 %u", g_temp);

    /* ============================================================
     * 5 分：时间模式步长 1
     * ============================================================ */
    printf("[9] 时间步长 1\n");
    key1_click(); tick_ms(20);
    CHECK(g_mode == FRYER_MODE_TIME, "应已切到时间模式");
    int t0 = g_time_left;
    ec12_turn(1); tick_ms(20);
    CHECK(g_time_left == t0 + 1, "时间模式右转一格应 +1：%d -> %u", t0, g_time_left);

    /* ============================================================
     * 5 分：时间模式最小 1、最大 60
     * ============================================================ */
    printf("[10] 时间范围 1~60\n");
    guard = 0;
    while (g_time_left > 1 && guard++ < 100) { ec12_turn(-1); tick_ms(20); }
    CHECK(g_time_left == 1, "连续左转应停在下限 1，实际 %u", g_time_left);
    ec12_turn(-1); tick_ms(20);
    CHECK(g_time_left == 1, "到 1 后继续左转应仍为 1，实际 %u", g_time_left);

    guard = 0;
    while (g_time_left < 60 && guard++ < 100) { ec12_turn(1); tick_ms(20); }
    CHECK(g_time_left == 60, "连续右转应停在上限 60，实际 %u", g_time_left);
    ec12_turn(1); tick_ms(20);
    CHECK(g_time_left == 60, "到 60 后继续右转应仍为 60，实际 %u", g_time_left);

    /* ============================================================
     * 5 分：暂停时点 KEY2 -> 显示时间 + 加热开启（红灯）
     * ============================================================ */
    printf("[11] 暂停态点 KEY2 启动\n");
    key2_click(); tick_ms(20);
    CHECK(g_running == 1, "点 KEY2 应进入运行，实际 %d", g_running);
    CHECK(g_mode == FRYER_MODE_TIME, "启动后应显示时间（时间模式），实际 %d", g_mode);
    CHECK(g_percent == 1, "启动后百分号应亮，实际 %d", g_percent);
    CHECK(g_led_color[0] == scale_by_brightness(0xff0000u, FRYER_LED_BRIGHTNESS),
          "加热中彩灯应为红色(缩放后 0x%06X)，实际 0x%06X",
          scale_by_brightness(0xff0000u, FRYER_LED_BRIGHTNESS), g_led_color[0]);

    /* ============================================================
     * 5 分：运行时不可以加减
     *
     * 【本轮踩过的坑】上一版这组断言是无效的：
     * 测试走到这里时温度正好 190 上限、时间正好 60 上限，
     * 即便代码允许加减，值也会被范围钳制住"看起来没变"，断言照样通过 ——
     * 变异测试因此漏掉了这个 bug。
     * 修法：先在未运行状态下把值调到区间中部，再启动，然后试加减。
     * ============================================================ */
    printf("[12] 运行中禁止加减\n");

    reset_all();
    key3_long(); tick_ms(20);           /* 开机，温度模式 100 */
    ec12_turn(1); tick_ms(20);          /* 温度 105 */
    key1_click(); tick_ms(20);          /* 时间模式，默认 10 */
    int i;
    for (i = 0; i < 20; i++) { ec12_turn(1); tick_ms(20); }   /* 时间 30 */
    CHECK(g_temp == 105, "前置：温度应为 105，实际 %u", g_temp);
    CHECK(g_time_left == 30, "前置：时间应为 30，实际 %u", g_time_left);

    key2_click(); tick_ms(20);
    CHECK(g_running == 1, "前置：应已启动");

    int t_before = g_time_left;
    ec12_turn(1); tick_ms(20);
    CHECK(g_time_left == t_before, "运行中右转不应增加时间：%u -> %u", t_before, g_time_left);
    ec12_turn(-1); tick_ms(20);
    CHECK(g_time_left == t_before, "运行中左转不应减少时间：%u -> %u", t_before, g_time_left);

    key1_click(); tick_ms(20);
    int temp_before = g_temp;
    ec12_turn(1); tick_ms(20);
    CHECK(g_temp == temp_before, "运行中右转不应改温度：%d -> %u", temp_before, g_temp);
    ec12_turn(-1); tick_ms(20);
    CHECK(g_temp == temp_before, "运行中左转不应改温度：%d -> %u", temp_before, g_temp);

    key1_click(); tick_ms(20);   /* 切回时间模式 */

    /* ============================================================
     * 5 分：暂停时点 KEY2 -> 显示时间（关键场景：从温度模式启动）
     *
     * 【本轮踩过的坑】上面 [11] 段测试时已经在时间模式了，
     * 所以即使把"启动时强制切时间模式"删掉，断言也照样通过 ——
     * 变异测试因此漏掉了它。
     * 评分表的真实场景是「用户在温度模式下直接按启动」，必须专门补用例。
     * ============================================================ */
    printf("[11b] 从温度模式启动应自动切到时间模式\n");
    reset_all();
    key3_long(); tick_ms(20);
    CHECK(g_mode == FRYER_MODE_TEMP, "前置：应为温度模式，实际 %d", g_mode);
    CHECK(g_num == 100, "前置：应显示温度 100，实际 %d", g_num);

    key2_click(); tick_ms(20);
    CHECK(g_running == 1, "应进入运行");
    CHECK(g_mode == FRYER_MODE_TIME, "温度模式启动后应自动切时间模式，实际 %d", g_mode);
    CHECK(g_num == 10, "启动后应显示时间 10，实际 %d", g_num);
    CHECK(g_percent == 1, "启动后百分号应亮，实际 %d", g_percent);
    CHECK(g_charge == 0, "启动后闪电应灭，实际 %d", g_charge);
    CHECK(g_led_color[0] == scale_by_brightness(0xff0000u, FRYER_LED_BRIGHTNESS),
          "启动后应红灯(缩放后 0x%06X)，实际 0x%06X",
          scale_by_brightness(0xff0000u, FRYER_LED_BRIGHTNESS), g_led_color[0]);

    /* ============================================================
     * 5 分：运行时，时间模式可以倒数计时
     * ============================================================ */
    printf("[13] 倒计时\n");
    reset_all();
    key3_long(); tick_ms(20);
    key1_click(); tick_ms(20);
    CHECK(g_time_left == 10, "时间模式默认应为 10，实际 %u", g_time_left);
    key2_click(); tick_ms(20);
    CHECK(g_running == 1, "应已启动");

    uint32_t t_start = g_t5_tick_ms;
    tick_ms(3000);
    CHECK(g_time_left == 7, "启动 3 秒后剩余应为 7，实际 %u", g_time_left);
    CHECK(g_t5_tick_ms - t_start == 3000, "时间推进应为 3000ms");

    tick_ms(3000);
    CHECK(g_time_left == 4, "再过 3 秒剩余应为 4，实际 %u", g_time_left);

    /* ============================================================
     * 5 分：运行时点 KEY2 -> 倒计时暂停 + 加热暂停（绿灯）
     * ============================================================ */
    printf("[14] 运行中点 KEY2 暂停\n");
    key2_click(); tick_ms(20);
    CHECK(g_running == 0, "点 KEY2 应暂停，实际 %d", g_running);
    CHECK(g_led_color[0] == scale_by_brightness(0x00ff00u, FRYER_LED_BRIGHTNESS),
          "暂停后彩灯应为绿色(缩放后 0x%06X)，实际 0x%06X",
          scale_by_brightness(0x00ff00u, FRYER_LED_BRIGHTNESS), g_led_color[0]);
    int paused_left = g_time_left;
    tick_ms(3000);
    CHECK(g_time_left == paused_left, "暂停后倒计时应停住：%d -> %u", paused_left, g_time_left);

    /* 恢复运行应从暂停处继续，不重头 */
    printf("[15] 暂停后恢复运行\n");
    key2_click(); tick_ms(20);
    CHECK(g_running == 1, "再点 KEY2 应恢复运行");
    CHECK(g_led_color[0] == scale_by_brightness(0xff0000u, FRYER_LED_BRIGHTNESS),
          "恢复后彩灯应为红色(缩放后 0x%06X)，实际 0x%06X",
          scale_by_brightness(0xff0000u, FRYER_LED_BRIGHTNESS), g_led_color[0]);
    tick_ms(1000);
    CHECK(g_time_left == (uint16_t)(paused_left - 1),
          "恢复 1 秒后应从 %d 继续 -> %u，实际 %u",
          paused_left, paused_left - 1, g_time_left);

    /* ============================================================
     * 5 分：倒计时归 0 -> 加热暂停（绿灯）+ 时间/温度显示默认值
     * ============================================================ */
    printf("[16] 倒计时归零\n");
    tick_ms(10000);
    CHECK(g_running == 0, "归零后应自动停止加热，实际 %d", g_running);
    CHECK(g_led_color[0] == scale_by_brightness(0x00ff00u, FRYER_LED_BRIGHTNESS),
          "归零后彩灯应为绿色(缩放后 0x%06X)，实际 0x%06X",
          scale_by_brightness(0x00ff00u, FRYER_LED_BRIGHTNESS), g_led_color[0]);
    CHECK(g_time_left == 10, "归零后时间应回默认值 10，实际 %u", g_time_left);
    CHECK(g_temp == 100, "归零后温度应回默认值 100，实际 %u", g_temp);
    CHECK(g_mode == FRYER_MODE_TIME, "归零后保持时间模式（当场显示时间默认值），实际 %d", g_mode);
    CHECK(g_num == 10, "归零后数码管应显示时间默认值 10，实际 %d", g_num);

    key1_click(); tick_ms(20);
    CHECK(g_mode == FRYER_MODE_TEMP, "归零后 KEY1 可切回温度模式");
    CHECK(g_num == 100, "切回温度模式应显示温度默认值 100，实际 %d", g_num);
    CHECK(g_charge == 1 && g_percent == 0, "温度模式标志应正确");

    /* ============================================================
     * 编码器脉冲换算自洽性
     * ============================================================ */
    printf("[17] 编码器脉冲换算自洽性\n");
    reset_all();
    key3_long(); tick_ms(20);
    ec12_turn(5);
    tick_ms(20);
    CHECK(g_temp == 125, "右转 5 格（%d 个脉冲）应 +25 -> 125，实际 %u",
          5 * EC12_PULSES_PER_STEP, g_temp);

    ec12_turn(-2);
    tick_ms(20);
    CHECK(g_temp == 115, "左转 2 格应 -10 -> 115，实际 %u", g_temp);

    /* 不足一格：给 (PULSES_PER_STEP - 1) 个脉冲，不应走步；
     * 再补 1 个凑满一格，应 +5。
     * 不写死"1+1"，否则 PULSES_PER_STEP=4 时这两步凑不满。 */
    if (EC12_PULSES_PER_STEP > 1) {
        int need = EC12_PULSES_PER_STEP;
        printf("  PULSES_PER_STEP=%d，先给 %d 个脉冲（差 1 个不满一格）\n", need, need - 1);

        ec12_turn_pulse(need - 1);
        tick_ms(20);
        CHECK(g_temp == 115, "差 1 个脉冲不满一格，温度不应变，实际 %u", g_temp);

        ec12_turn_pulse(1);
        tick_ms(20);
        CHECK(g_temp == 120, "补满一格后应 +5 -> 120，实际 %u", g_temp);
    }

    /* ============================================================
     * 关机清残余脉冲：半格残余不会带到开机后
     * ============================================================ */
    printf("[18] 关机清残余脉冲\n");
    if (EC12_PULSES_PER_STEP > 1) {
        /* 关机前留下 (PULSES_PER_STEP-1) 个残余脉冲 */
        ec12_turn_pulse(EC12_PULSES_PER_STEP - 1);
        key3_long(); tick_ms(20);      /* 关机 -> 应清残余 */
        key3_long(); tick_ms(20);      /* 开机 */
        CHECK(g_temp == 100, "开机后温度应为默认 100（残余已清），实际 %u", g_temp);

        /* 开机后再给同样数量的残余，不应因旧残余而跳格 */
        ec12_turn_pulse(EC12_PULSES_PER_STEP - 1);
        tick_ms(20);
        CHECK(g_temp == 100, "开机后不足一格不应跳格，实际 %u", g_temp);
    }

    /* ============================================================
     * 汇总
     * ============================================================ */
    /* ============================================================
     * 亮度：满亮度 100 时取 10
     *
     * 上面那些颜色断言用的是"同一个缩放函数算出来的期望值"，
     * 如果宏本身被改错（比如 10 写成 100），那些断言抓不到 ——
     * 因为期望值和实际值会一起变。所以这里必须写死数值。
     * ============================================================ */
    printf("[19] 亮度参数\n");
    CHECK(FRYER_LED_BRIGHTNESS == 10,
          "亮度宏应为 10（满亮度 100 的 10%%），实际 %d", FRYER_LED_BRIGHTNESS);

    /* 写死换算结果：255 * 10 / 100 = 25.5 -> 四舍五入 26 = 0x1A
     * 红 0xff0000 -> 0x1A0000；绿 0x00ff00 -> 0x001A00 */
    reset_all();
    key3_long(); tick_ms(20);
    CHECK(g_led_color[0] == 0x001A00u,
          "绿灯按亮度 10 缩放应为 0x001A00，实际 0x%06X", g_led_color[0]);

    key2_click(); tick_ms(20);
    CHECK(g_led_color[0] == 0x1A0000u,
          "红灯按亮度 10 缩放应为 0x1A0000，实际 0x%06X", g_led_color[0]);

    /* 满亮度对照：证明换算函数本身对 */
    CHECK(scale_by_brightness(0xff0000u, 100) == 0xff0000u,
          "满亮度 100 应保持原色，实际 0x%06X", scale_by_brightness(0xff0000u, 100));
    CHECK(scale_by_brightness(0x000000u, 10) == 0x000000u,
          "黑色缩放到任何亮度都应仍是黑，实际 0x%06X", scale_by_brightness(0x000000u, 10));

    printf("\n===== 结果：通过 %d，失败 %d =====\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
