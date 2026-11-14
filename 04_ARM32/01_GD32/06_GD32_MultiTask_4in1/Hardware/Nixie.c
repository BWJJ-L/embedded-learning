#include "Nixie.h"

static void GPIO_init(uint32_t rcu, uint32_t port, uint32_t pin) {
    rcu_periph_clock_enable(rcu);
    gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);
    gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, pin);
}

// 初始化
void Nixie_init() {
    GPIO_init(NIX_DI_RCU, NIX_DI_GPIO);
    GPIO_init(NIX_RCK_RCU, NIX_RCK_GPIO);
    GPIO_init(NIX_SCK_RCU, NIX_SCK_GPIO);

    Nixie_clear(); // 上电先全部熄灭，避免残影
}

// 段码表
// 下标 0~9   ：数字 0~9
// 下标 10~19 ：带小数点的 0.~9.
// 下标 20    ：小数点单独一个点
// 下标 21    ：横线 -
// 下标 22~33 ：字母 AbCdEFHJLPqU
static u8 LED_TABLE[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90,
    0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10,
    0x7F, 0xBF,
    0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0x89, 0xF1, 0xC7, 0x8C, 0x98, 0xC1
};

// 锁存：RCK 产生一个上升沿，把移位寄存器内容送到输出
#define RCK_ACTION() { \
    gpio_bit_write(NIX_RCK_GPIO, RESET); \
    __NOP(); __NOP(); \
    gpio_bit_write(NIX_RCK_GPIO, SET); \
    __NOP(); __NOP(); \
}

// 移出一个字节，高位在前
// 用 int8_t 而不是 char：char 是否有符号依赖编译器设置，
// 原课程代码里 char i 从 7 递减到 0 的循环在无符号 char 下会死循环。
void Nixie_output(u8 dat) {
    int8_t i;
    for (i = 7; i >= 0; i--) {
        gpio_bit_write(NIX_DI_GPIO, ((dat >> i) & 1) ? SET : RESET);

        // SCK 产生一个上升沿，595 在此沿采样 DI
        gpio_bit_write(NIX_SCK_GPIO, RESET);
        __NOP();
        __NOP();
        gpio_bit_write(NIX_SCK_GPIO, SET);
        __NOP();
        __NOP();
    }
}

// num: 段码    idx: 位码
void Nixie_show(u8 num, u8 idx) {
    Nixie_output(num); // 先送段码
    Nixie_output(idx); // 再送位码，后进先出，位码落在第一级
    RCK_ACTION();      // 锁存输出
}

// num: 段码表下标    idx: 位置 1~8
void Nixie_display(u8 num, u8 idx) {
    if (num > NIX_TABLE_MAX) return; // 越界保护，避免读穿表
    if (idx < 1 || idx > 8) return;

    Nixie_show(LED_TABLE[num], 1 << (idx - 1));
}

// 全部熄灭：段码全 1（共阳极不亮），位码全 0
void Nixie_clear() {
    Nixie_show(0xFF, 0x00);
}
