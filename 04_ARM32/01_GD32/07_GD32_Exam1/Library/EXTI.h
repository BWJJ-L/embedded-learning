#ifndef __EXTI_H__
#define __EXTI_H__

#include "gd32f4xx.h"

#include "EXTI_config.h"

void EXTI_init();

// 非函数指针的回调函数
void EXTI0_on_trig();
void EXTI1_on_trig();
void EXTI2_on_trig();
void EXTI3_on_trig();
void EXTI4_on_trig();
void EXTI5_on_trig();
void EXTI6_on_trig();
void EXTI7_on_trig();
void EXTI8_on_trig();
void EXTI9_on_trig();
void EXTI10_on_trig();
void EXTI11_on_trig();
void EXTI12_on_trig();
void EXTI13_on_trig();
void EXTI14_on_trig();
void EXTI15_on_trig();

#endif