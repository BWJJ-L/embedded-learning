#ifndef __EXTI_H__
#define __EXTI_H__

#include "gpio_cfg.h"
#include "EXTI_config.h"

// EXTI_config.h 开关打开后，对应的回调函数要定义
// 非函数指针的回调函数，声明
void EXTI0_on_trig(void);
void EXTI1_on_trig(void);
void EXTI2_on_trig(void);
void EXTI3_on_trig(void);
void EXTI4_on_trig(void);
void EXTI5_on_trig(void);
void EXTI6_on_trig(void);
void EXTI7_on_trig(void);
void EXTI8_on_trig(void);
void EXTI9_on_trig(void);
void EXTI10_on_trig(void);
void EXTI11_on_trig(void);
void EXTI12_on_trig(void);
void EXTI13_on_trig(void);
void EXTI14_on_trig(void);
void EXTI15_on_trig(void);


void EXTI_init(); // 初始化 

#endif