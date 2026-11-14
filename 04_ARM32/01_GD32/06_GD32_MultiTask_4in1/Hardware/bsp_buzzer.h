#ifndef __BSP_BUZZER_H__
#define __BSP_BUZZER_H__

#include "gpio_cfg.h"

// 音阶个数：1~8 对应 C D E F G A B C
#define BUZZER_SCALE_NUM    8

// 初始化（配置 PB9 为 Timer1_CH1 复用输出）
void Buzzer_init();

// 按指定频率发声，hz_value 为 0 时静音
// 注意：本函数只改寄存器，不阻塞；发声会一直持续到下一次调用或 Buzzer_stop()
void Buzzer_play(uint16_t hz_value);

// 按音阶发声，hz_value_index 取 1~8，0 表示停止
void Buzzer_beep(uint8_t hz_value_index);

// 停止发声
void Buzzer_stop();

// 查询当前是否正在发声，1 发声 / 0 静音
uint8_t Buzzer_is_playing();

// 取音阶对应的频率，index 取 1~8，越界返回 0
uint16_t Buzzer_get_hz(uint8_t hz_value_index);

#endif
