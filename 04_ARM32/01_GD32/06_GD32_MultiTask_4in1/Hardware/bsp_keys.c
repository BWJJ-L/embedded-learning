#include "bsp_keys.h"

// 扩展板按键
#define KEY_PORT    GPIOC
#define KEY_RCU     RCU_GPIOC

#define DOWN        RESET
#define UP          SET

// 消抖次数：按键状态连续稳定这么多次才确认
// Keys_scan 由 10ms 的任务调用，3 次 = 30ms，覆盖常见机械抖动
#define KEY_DEBOUNCE_CNT    3

// 按键信息结构体
typedef struct {
    uint32_t   gpio_port;   // GPIO 端口
    uint32_t   gpio_pin;    // GPIO 引脚
    uint8_t    key_id;      // 按键编号，传给 Keys_on_keydown / Keys_on_keyup
    FlagStatus last_state;  // 已确认的状态（消抖后）
    FlagStatus raw_state;   // 上一次采样的原始状态
    uint8_t    stable_cnt;  // 连续稳定计数
} KeyInfo;

// 按键数组：以后增加按键只需要在这里加一行
static KeyInfo s_keys[] = {
    {KEY_PORT, GPIO_PIN_0, 1, UP, UP, 0},
    {KEY_PORT, GPIO_PIN_1, 2, UP, UP, 0},
    {KEY_PORT, GPIO_PIN_2, 3, UP, UP, 0},
    {KEY_PORT, GPIO_PIN_3, 4, UP, UP, 0},
};

#define KEY_COUNT   (sizeof(s_keys) / sizeof(s_keys[0]))

void Keys_init(void)
{
    uint32_t pins = 0;

    for (uint32_t i = 0; i < KEY_COUNT; i++) {
        pins |= s_keys[i].gpio_pin;
    }

    // 扩展板按键：上拉输入，按键导通后为低电平
    GPIO_input(KEY_RCU, KEY_PORT, pins, GPIO_PUPD_PULLUP);
}

// 计数器式消抖，由 10ms 的任务周期调用
void Keys_scan(void)
{
    for (uint32_t i = 0; i < KEY_COUNT; i++) {
        FlagStatus cur_state = gpio_input_bit_get(s_keys[i].gpio_port,
                                                  s_keys[i].gpio_pin);

        // 采样值和上一次不同，说明正在抖动，重新计数
        if (cur_state != s_keys[i].raw_state) {
            s_keys[i].raw_state  = cur_state;
            s_keys[i].stable_cnt = 0;
            continue;
        }

        // 采样值稳定但与"已确认状态"不同，累计稳定次数
        if (cur_state != s_keys[i].last_state) {
            s_keys[i].stable_cnt++;
            if (s_keys[i].stable_cnt < KEY_DEBOUNCE_CNT) continue;

            // 连续稳定够次数，确认状态变化
            s_keys[i].last_state  = cur_state;
            s_keys[i].stable_cnt  = 0;

            if (cur_state == DOWN) {
                Keys_on_keydown(s_keys[i].key_id);
            } else {
                Keys_on_keyup(s_keys[i].key_id);
            }
        } else {
            // 状态与已确认的一致，清掉计数
            s_keys[i].stable_cnt = 0;
        }
    }
}

// index 取值范围 1~4 代表 key1~key4
__attribute__((weak)) void Keys_on_keydown(uint8_t index) {
	(void)index;
}

__attribute__((weak)) void Keys_on_keyup(uint8_t index) {
	(void)index;
}
