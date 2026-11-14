#include "EXTI.h"

/**********************************************************
 * @brief    配置外部中断引脚
 **********************************************************/
static void EXTI_config(rcu_periph_enum rcu, uint32_t port, uint32_t pin, uint32_t pull_up_down,
    uint8_t exti_port, uint8_t exti_pin, exti_line_enum linex, exti_trig_type_enum trig_type,
    uint8_t nvic_irq, uint8_t nvic_irq_pre_priority, uint8_t nvic_irq_sub_priority) {

    // 引脚输入初始化
    GPIO_input(rcu, port, pin, pull_up_down);

    // 时钟使能 syscfg
    rcu_periph_clock_enable(RCU_SYSCFG);

    // 配置 EXTI 中断线来源
    syscfg_exti_line_config(exti_port, exti_pin);

    // 初始化外部中断
    exti_init(linex, EXTI_INTERRUPT, trig_type);

    // 外部中断使能
    exti_interrupt_enable(linex);
    exti_interrupt_flag_clear(linex);

    // NVIC 中断请求使能，中断优先级配置
    nvic_irq_enable(nvic_irq, nvic_irq_pre_priority, nvic_irq_sub_priority);
}

// 维护外部中断 0~15，使用宏定义开关
void EXTI_init(void) {

#if USE_EXTI_0
    EXTI_config(EXTI0_RCU, EXTI0_PORT, GPIO_PIN_0, EXTI0_PUPD,
        EXTI0_SOURCE_PORT, EXTI_SOURCE_PIN0,
        EXTI_0, EXTI0_TRIG_TYPE,
        EXTI0_IRQn, EXTI0_PRIORITY);
#endif

#if USE_EXTI_1
    EXTI_config(EXTI1_RCU, EXTI1_PORT, GPIO_PIN_1, EXTI1_PUPD,
        EXTI1_SOURCE_PORT, EXTI_SOURCE_PIN1,
        EXTI_1, EXTI1_TRIG_TYPE,
        EXTI1_IRQn, EXTI1_PRIORITY);
#endif

#if USE_EXTI_2
    EXTI_config(EXTI2_RCU, EXTI2_PORT, GPIO_PIN_2, EXTI2_PUPD,
        EXTI2_SOURCE_PORT, EXTI_SOURCE_PIN2,
        EXTI_2, EXTI2_TRIG_TYPE,
        EXTI2_IRQn, EXTI2_PRIORITY);
#endif

#if USE_EXTI_3
    EXTI_config(EXTI3_RCU, EXTI3_PORT, GPIO_PIN_3, EXTI3_PUPD,
        EXTI3_SOURCE_PORT, EXTI_SOURCE_PIN3,
        EXTI_3, EXTI3_TRIG_TYPE,
        EXTI3_IRQn, EXTI3_PRIORITY);
#endif

#if USE_EXTI_4
    EXTI_config(EXTI4_RCU, EXTI4_PORT, GPIO_PIN_4, EXTI4_PUPD,
        EXTI4_SOURCE_PORT, EXTI_SOURCE_PIN4,
        EXTI_4, EXTI4_TRIG_TYPE,
        EXTI4_IRQn, EXTI4_PRIORITY);
#endif

#if USE_EXTI_5
    EXTI_config(EXTI5_RCU, EXTI5_PORT, GPIO_PIN_5, EXTI5_PUPD,
        EXTI5_SOURCE_PORT, EXTI_SOURCE_PIN5,
        EXTI_5, EXTI5_TRIG_TYPE,
        EXTI5_9_IRQn, EXTI5_PRIORITY);
#endif

#if USE_EXTI_6
    EXTI_config(EXTI6_RCU, EXTI6_PORT, GPIO_PIN_6, EXTI6_PUPD,
        EXTI6_SOURCE_PORT, EXTI_SOURCE_PIN6,
        EXTI_6, EXTI6_TRIG_TYPE,
        EXTI5_9_IRQn, EXTI6_PRIORITY);
#endif

#if USE_EXTI_7
    EXTI_config(EXTI7_RCU, EXTI7_PORT, GPIO_PIN_7, EXTI7_PUPD,
        EXTI7_SOURCE_PORT, EXTI_SOURCE_PIN7,
        EXTI_7, EXTI7_TRIG_TYPE,
        EXTI5_9_IRQn, EXTI7_PRIORITY);
#endif

#if USE_EXTI_8
    EXTI_config(EXTI8_RCU, EXTI8_PORT, GPIO_PIN_8, EXTI8_PUPD,
        EXTI8_SOURCE_PORT, EXTI_SOURCE_PIN8,
        EXTI_8, EXTI8_TRIG_TYPE,
        EXTI5_9_IRQn, EXTI8_PRIORITY);
#endif

#if USE_EXTI_9
    EXTI_config(EXTI9_RCU, EXTI9_PORT, GPIO_PIN_9, EXTI9_PUPD,
        EXTI9_SOURCE_PORT, EXTI_SOURCE_PIN9,
        EXTI_9, EXTI9_TRIG_TYPE,
        EXTI5_9_IRQn, EXTI9_PRIORITY);
#endif

#if USE_EXTI_10
    EXTI_config(EXTI10_RCU, EXTI10_PORT, GPIO_PIN_10, EXTI10_PUPD,
        EXTI10_SOURCE_PORT, EXTI_SOURCE_PIN10,
        EXTI_10, EXTI10_TRIG_TYPE,
        EXTI10_15_IRQn, EXTI10_PRIORITY);
#endif

#if USE_EXTI_11
    EXTI_config(EXTI11_RCU, EXTI11_PORT, GPIO_PIN_11, EXTI11_PUPD,
        EXTI11_SOURCE_PORT, EXTI_SOURCE_PIN11,
        EXTI_11, EXTI11_TRIG_TYPE,
        EXTI10_15_IRQn, EXTI11_PRIORITY);
#endif

#if USE_EXTI_12
    EXTI_config(EXTI12_RCU, EXTI12_PORT, GPIO_PIN_12, EXTI12_PUPD,
        EXTI12_SOURCE_PORT, EXTI_SOURCE_PIN12,
        EXTI_12, EXTI12_TRIG_TYPE,
        EXTI10_15_IRQn, EXTI12_PRIORITY);
#endif

#if USE_EXTI_13
    EXTI_config(EXTI13_RCU, EXTI13_PORT, GPIO_PIN_13, EXTI13_PUPD,
        EXTI13_SOURCE_PORT, EXTI_SOURCE_PIN13,
        EXTI_13, EXTI13_TRIG_TYPE,
        EXTI10_15_IRQn, EXTI13_PRIORITY);
#endif

#if USE_EXTI_14
    EXTI_config(EXTI14_RCU, EXTI14_PORT, GPIO_PIN_14, EXTI14_PUPD,
        EXTI14_SOURCE_PORT, EXTI_SOURCE_PIN14,
        EXTI_14, EXTI14_TRIG_TYPE,
        EXTI10_15_IRQn, EXTI14_PRIORITY);
#endif

#if USE_EXTI_15
    EXTI_config(EXTI15_RCU, EXTI15_PORT, GPIO_PIN_15, EXTI15_PUPD,
        EXTI15_SOURCE_PORT, EXTI_SOURCE_PIN15,
        EXTI_15, EXTI15_TRIG_TYPE,
        EXTI10_15_IRQn, EXTI15_PRIORITY);
#endif
}

/* ==================== EXTI0~4 独立中断 ==================== */

#if USE_EXTI_0
void EXTI0_IRQHandler(void) {
    if (SET == exti_interrupt_flag_get(EXTI_0)) {
        exti_interrupt_flag_clear(EXTI_0);
        EXTI0_on_trig();
    }
}
#endif

#if USE_EXTI_1
void EXTI1_IRQHandler(void) {
    if (SET == exti_interrupt_flag_get(EXTI_1)) {
        exti_interrupt_flag_clear(EXTI_1);
        EXTI1_on_trig();
    }
}
#endif

#if USE_EXTI_2
void EXTI2_IRQHandler(void) {
    if (SET == exti_interrupt_flag_get(EXTI_2)) {
        exti_interrupt_flag_clear(EXTI_2);
        EXTI2_on_trig();
    }
}
#endif

#if USE_EXTI_3
void EXTI3_IRQHandler(void) {
    if (SET == exti_interrupt_flag_get(EXTI_3)) {
        exti_interrupt_flag_clear(EXTI_3);
        EXTI3_on_trig();
    }
}
#endif

#if USE_EXTI_4
void EXTI4_IRQHandler(void) {
    if (SET == exti_interrupt_flag_get(EXTI_4)) {
        exti_interrupt_flag_clear(EXTI_4);
        EXTI4_on_trig();
    }
}
#endif

/* ==================== EXTI5~9 共用中断 ==================== */
#if USE_EXTI_5 || USE_EXTI_6 || USE_EXTI_7 || USE_EXTI_8 || USE_EXTI_9
void EXTI5_9_IRQHandler(void) {
#if USE_EXTI_5
    if (SET == exti_interrupt_flag_get(EXTI_5)) {
        exti_interrupt_flag_clear(EXTI_5);
        EXTI5_on_trig();
    }
#endif

#if USE_EXTI_6
    if (SET == exti_interrupt_flag_get(EXTI_6)) {
        exti_interrupt_flag_clear(EXTI_6);
        EXTI6_on_trig();
    }
#endif

#if USE_EXTI_7
    if (SET == exti_interrupt_flag_get(EXTI_7)) {
        exti_interrupt_flag_clear(EXTI_7);
        EXTI7_on_trig();
    }
#endif

#if USE_EXTI_8
    if (SET == exti_interrupt_flag_get(EXTI_8)) {
        exti_interrupt_flag_clear(EXTI_8);
        EXTI8_on_trig();
    }
#endif

#if USE_EXTI_9
    if (SET == exti_interrupt_flag_get(EXTI_9)) {
        exti_interrupt_flag_clear(EXTI_9);
        EXTI9_on_trig();
    }
#endif
}
#endif

/* ==================== EXTI10~15 共用中断 ==================== */
#if USE_EXTI_10 || USE_EXTI_11 || USE_EXTI_12 || USE_EXTI_13 || USE_EXTI_14 || USE_EXTI_15
void EXTI10_15_IRQHandler(void) {
#if USE_EXTI_10
    if (SET == exti_interrupt_flag_get(EXTI_10)) {
        exti_interrupt_flag_clear(EXTI_10);
        EXTI10_on_trig();
    }
#endif

#if USE_EXTI_11
    if (SET == exti_interrupt_flag_get(EXTI_11)) {
        exti_interrupt_flag_clear(EXTI_11);
        EXTI11_on_trig();
    }
#endif

#if USE_EXTI_12
    if (SET == exti_interrupt_flag_get(EXTI_12)) {
        exti_interrupt_flag_clear(EXTI_12);
        EXTI12_on_trig();
    }
#endif

#if USE_EXTI_13
    if (SET == exti_interrupt_flag_get(EXTI_13)) {
        exti_interrupt_flag_clear(EXTI_13);
        EXTI13_on_trig();
    }
#endif

#if USE_EXTI_14
    if (SET == exti_interrupt_flag_get(EXTI_14)) {
        exti_interrupt_flag_clear(EXTI_14);
        EXTI14_on_trig();
    }
#endif

#if USE_EXTI_15
    if (SET == exti_interrupt_flag_get(EXTI_15)) {
        exti_interrupt_flag_clear(EXTI_15);
        EXTI15_on_trig();
    }
#endif
}

#endif

