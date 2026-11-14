#include "bsp_buzzer.h"

// 硬件接线：PB9 -> Timer1_CH1 复用输出 -> BUZZER
// 原理图第 94 行 PB9 BUZZER；无源蜂鸣器，需要方波驱动
#define BUZZER_RCU          RCU_GPIOB
#define BUZZER_PORT         GPIOB
#define BUZZER_PIN          GPIO_PIN_9
#define BUZZER_AF           GPIO_AF_1

#define BUZZER_TIMER_RCU    RCU_TIMER1
#define BUZZER_TIMER        TIMER1
#define BUZZER_CHANNEL      TIMER_CH_1

// 分频系数，定时器时钟 168M（RCU_TIMER_PSC_MUL4）
// 约束：PRESCALER * 1000 >= 2564，才能覆盖最高音阶
#define BUZZER_PRESCALER    10

// 音阶频率表 C D E F G A B C（单位 Hz）
static const uint16_t s_hz[BUZZER_SCALE_NUM] = {
    1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093
};

// 当前是否发声，1 发声 / 0 静音
static uint8_t s_playing = 0;

void Buzzer_init() {
    // PB9 复用推挽输出
    GPIO_output_af(BUZZER_RCU, BUZZER_PORT, BUZZER_PIN, GPIO_OTYPE_PP, BUZZER_AF);

    // 定时器时钟倍频到 168M，与 TIMER_init() 里的设置一致
    rcu_periph_clock_enable(BUZZER_TIMER_RCU);

    timer_parameter_struct initpara;
    timer_struct_para_init(&initpara);
    // 周期和分频先给个初值，真正发声时由 Buzzer_play 改写
    // period 字段是 uint32_t，不要强转成 uint16_t
    initpara.period    = SystemCoreClock / BUZZER_PRESCALER / 1000; // 1kHz
    initpara.prescaler = BUZZER_PRESCALER - 1;
    timer_init(BUZZER_TIMER, &initpara);

    // 输出通道：正极、PWM0 模式
    timer_oc_parameter_struct ocpara;
    timer_channel_output_struct_para_init(&ocpara);
    ocpara.outputstate  = TIMER_CCX_ENABLE;
    ocpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_channel_output_config(BUZZER_TIMER, BUZZER_CHANNEL, &ocpara);
    timer_channel_output_mode_config(BUZZER_TIMER, BUZZER_CHANNEL, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(BUZZER_TIMER, BUZZER_CHANNEL, TIMER_OC_SHADOW_DISABLE);

    // 初始静音：占空比 0，然后不启动计数器
    timer_channel_output_pulse_value_config(BUZZER_TIMER, BUZZER_CHANNEL, 0);
    timer_disable(BUZZER_TIMER);

    s_playing = 0;
}

// 只改周期寄存器和占空比，不 deinit / init，避免产生毛刺
// 也不阻塞，供任务表调用
void Buzzer_play(uint16_t hz_value) {
    if (hz_value == 0) {
        Buzzer_stop();
        return;
    }

    // period = 168M / 10 / hz
    // 最高音阶 2093Hz 时约 8026，最低 1047Hz 时约 16047，都在 16 位范围内
    uint32_t period = SystemCoreClock / BUZZER_PRESCALER / hz_value;
    if (period < 2) period = 2;         // 保护：至少能产生方波
    if (period > 65536) period = 65536; // 上限保护

    timer_disable(BUZZER_TIMER);
    // 计数器计数范围 = period - 1
    // 注意：TIMER1 是 32 位定时器，必须按 32 位写（与官方 timer_init 的
    // TIMER_CAR(timer_periph) = (uint32_t)initpara->period 保持一致）。
    // 这里若强转成 uint16_t，超过 65535 的周期会被截断，音调直接跑偏。
    TIMER_CAR(BUZZER_TIMER) = (uint32_t)(period - 1);
    // 50% 占空比
    timer_channel_output_pulse_value_config(BUZZER_TIMER, BUZZER_CHANNEL, period / 2);
    // 重装计数器，保证新周期立即生效
    timer_counter_value_config(BUZZER_TIMER, 0);
    timer_enable(BUZZER_TIMER);

    s_playing = 1;
}

void Buzzer_beep(uint8_t hz_value_index) {
    if (hz_value_index < 1 || hz_value_index > BUZZER_SCALE_NUM) {
        Buzzer_stop();
        return;
    }
    Buzzer_play(s_hz[hz_value_index - 1]);
}

void Buzzer_stop() {
    timer_disable(BUZZER_TIMER);
    // 占空比清零，输出保持低电平，蜂鸣器不响
    timer_channel_output_pulse_value_config(BUZZER_TIMER, BUZZER_CHANNEL, 0);
    s_playing = 0;
}

uint8_t Buzzer_is_playing() {
    return s_playing;
}

uint16_t Buzzer_get_hz(uint8_t hz_value_index) {
    if (hz_value_index < 1 || hz_value_index > BUZZER_SCALE_NUM) return 0;
    return s_hz[hz_value_index - 1];
}
