#ifndef __TIMER_H__
#define __TIMER_H__

#include "gd32f4xx.h"
#include "TIMER_config.h"

// 初始化
void TIMER_init();

/**
 * @brief 更新指定Timer的channel的占空比
 *
 * @param timer_periph  TIMERx(x=0,1,2,3,4,5,6,7,8,9,10,11,12,13)
 * @param channel   	TIMER_CH_0, TIMER_CH_1, TIMER_CH_2, TIMER_CH_3
 * @param t_period      周期 [1, 65536]
 * @param duty  		占空比 [0.0, 100.0]
 */
void TIMER_channel_duty_update(uint32_t timer_periph, uint16_t channel, uint32_t t_period, float duty);

#endif