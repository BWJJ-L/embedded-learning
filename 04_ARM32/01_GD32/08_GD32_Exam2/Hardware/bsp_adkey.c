#include "bsp_adkey.h"
#include "ADC0.h"
#include <stdio.h>

extern volatile uint64_t g_t5_tick_ms;   /* 每 1ms 自动 +1 */

/* =====================================================================
 *  可配置时间参数（单位：ms）
 * ===================================================================== */
#define ADKEY_SHORT_MS                50u    /* 短按时间阈值 */
#define ADKEY_LONG_MS                 2000u   /* 长按时间阈值：考试2.md 要求「长按2秒」 */

/*
 * 短按触发方式选择：
 *
 * 0 —— 短按在“释放”时触发（推荐）
 *      按住时间在 [ADKEY_SHORT_MS, ADKEY_LONG_MS) 内，释放时触发短按；
 *      按住达到 ADKEY_LONG_MS 触发长按，长按释放后不会再触发短按。
 *
 * 1 —— 短按在“按住达到 ADKEY_SHORT_MS”时立即触发（不需要抬起）
 *      注意：此模式下长按会先触发一次短按，再触发长按，
 *            因为短按时间比长按时间先到，这是“不抬起就触发短按”
 *            这一要求本身决定的。
 */
#define ADKEY_SHORT_TRIGGER_ON_PRESS  0

#define ADKEY_KEY_NUM                 3       /* KEY1 / KEY2 / KEY3 */

/* =====================================================================
 *  按键 ADC 阈值
 * ===================================================================== */
#define KEY1_VALUE              950
#define KEY2_VALUE              2048
#define KEY3_VALUE              3140
#define KEY1_KEY2_VALUE         770
#define KEY1_KEY3_VALUE         880
#define KEY2_KEY3_VALUE         1780
#define KEY1_KEY2_KEY3_VALUE    730
#define KEY_ALL_UP              4095

#define KEY_RANGE               20      /* 判断是否在 ±20 范围内 */

/* 判断是否在 ±KEY_RANGE 范围内 */
#define IS_IN(a, r)   (((a) < ((r) + KEY_RANGE)) && ((a) > ((r) - KEY_RANGE)))

#define KEY1    (1u << 0)
#define KEY2    (1u << 1)
#define KEY3    (1u << 2)

/* =====================================================================
 *  采样参数
 *  转换 1 次约 4.6us，轮询 1ms 采 1 次，采 50 次约 50ms 出一帧
 * ===================================================================== */
#define SAMPLE_CNT      50

/* =====================================================================
 *  按键状态
 * ===================================================================== */
static uint8_t  key_state      = 0x00;   /* 当前采样得到的按下状态 */
static uint8_t  last_key_state = 0x00;   /* 上一次处理的按下状态 */

static uint64_t press_tick[ADKEY_KEY_NUM];   /* 按下时刻 */
static uint8_t  long_fired[ADKEY_KEY_NUM];   /* 长按是否已触发 */
static uint8_t  short_fired[ADKEY_KEY_NUM];  /* 短按是否已触发 */

/* =====================================================================
 *  按键事件回调（可在业务层自行修改实现）
 * ===================================================================== */
__attribute__((weak)) void ADCKey_on_click(uint8_t i)
{
    /* i: 0 -> KEY1, 1 -> KEY2, 2 -> KEY3 */
    printf("key%d click\n", i + 1);
}

__attribute__((weak)) void ADCKey_on_long_click(uint8_t i)
{
    printf("key%d long click\n", i + 1);
}

/* =====================================================================
 *  初始化
 * ===================================================================== */
void ADKey_init(void)
{
    uint8_t i;

    key_state      = 0x00;
    last_key_state = 0x00;

    for (i = 0; i < ADKEY_KEY_NUM; i++) {
        press_tick[i]  = 0;
        long_fired[i]  = 0;
        short_fired[i] = 0;
    }

    printf("==ADKey_init==\n");
}

/* =====================================================================
 *  按键状态机处理（每 1ms 调用一次）
 * ===================================================================== */
static void ADKey_process(void)
{
    uint8_t  cur = key_state;
    uint64_t now = g_t5_tick_ms;
    uint8_t  i;

    for (i = 0; i < ADKEY_KEY_NUM; i++) {
        uint8_t bit       = (uint8_t)(1u << i);
        uint8_t cur_down  = (cur & bit) ? 1 : 0;
        uint8_t last_down = (last_key_state & bit) ? 1 : 0;

        if (cur_down && !last_down) {
            /* ---------------- 按下沿 ---------------- */
            press_tick[i]  = now;
            long_fired[i]  = 0;
            short_fired[i] = 0;

        } else if (cur_down && last_down) {
            /* ---------------- 持续按住 ---------------- */
            uint64_t elapsed = now - press_tick[i];

            /* 长按优先：按住时间达到长按阈值，立即触发长按 */
            if (!long_fired[i] && elapsed >= ADKEY_LONG_MS) {
                long_fired[i] = 1;
                ADCKey_on_long_click(i);
            }

#if ADKEY_SHORT_TRIGGER_ON_PRESS
            /* 短按不抬起触发：按住达到短按阈值立即触发 */
            if (!long_fired[i] && !short_fired[i] && elapsed >= ADKEY_SHORT_MS) {
                short_fired[i] = 1;
                ADCKey_on_click(i);
            }
#endif

        } else if (!cur_down && last_down) {
            /* ---------------- 释放沿 ---------------- */
            uint64_t elapsed = now - press_tick[i];

#if !ADKEY_SHORT_TRIGGER_ON_PRESS
            /* 释放时触发短按：
             * 时长在 [ADKEY_SHORT_MS, ADKEY_LONG_MS) 内，且长按未触发 */
            if (!long_fired[i] &&
                elapsed >= ADKEY_SHORT_MS &&
                elapsed <  ADKEY_LONG_MS) {
                ADCKey_on_click(i);
            }
#endif

            long_fired[i]  = 0;
            short_fired[i] = 0;
        }
    }

    last_key_state = cur;
}

/* =====================================================================
 *  按键扫描（每 1ms 调用一次）
 * ===================================================================== */
void ADKey_scan(void)
{
    /* 1. 每 1ms 处理一次短按 / 长按状态机 */
    ADKey_process();

    /* 2. 累积采样，去最大最小值求平均 */
    static uint16_t i         = 0;
    static uint32_t sum       = 0;
    static uint16_t max_value = 0;
    static uint16_t min_value = 4100;

    if (i < SAMPLE_CNT) {
        uint16_t value = ADC0_get(0);

        sum += value;
        if (value > max_value) max_value = value;
        if (value < min_value) min_value = value;

        i++;
    } else {
        /* 采样次数够了：去掉最大、最小后求平均 */
        uint16_t adc;

        sum = sum - max_value - min_value;
        adc = (uint16_t)(sum / (SAMPLE_CNT - 2));

        /* 重置采样累加器 */
        i         = 0;
        sum       = 0;
        max_value = 0;
        min_value = 4100;

        /* 3. 根据 ADC 值解析当前按下状态 */
        key_state = 0;

        if (IS_IN(adc, KEY1_VALUE)) {
            key_state |= KEY1;
        } else if (IS_IN(adc, KEY2_VALUE)) {
            key_state |= KEY2;
        } else if (IS_IN(adc, KEY3_VALUE)) {
            key_state |= KEY3;
        } else if (IS_IN(adc, KEY1_KEY2_VALUE)) {
            key_state |= KEY1;
            key_state |= KEY2;
        }
        /* 其他组合可按需扩展，例如：
         * else if (IS_IN(adc, KEY1_KEY3_VALUE))  { key_state |= KEY1 | KEY3; }
         * else if (IS_IN(adc, KEY2_KEY3_VALUE))  { key_state |= KEY2 | KEY3; }
         * else if (IS_IN(adc, KEY1_KEY2_KEY3_VALUE)) { key_state |= KEY1|KEY2|KEY3; }
         */

        /* 短按 / 长按的逻辑在 ADKey_process() 中统一处理 */
    }
}