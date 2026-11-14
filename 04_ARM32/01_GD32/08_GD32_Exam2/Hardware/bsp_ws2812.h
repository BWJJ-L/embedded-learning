#ifndef __BSP_WS2812_H__
#define __BSP_WS2812_H__

#include "gpio_cfg.h"

#define WS2812_NUM		4	// 彩灯数量

void WS2812_init();

// index  哪一个灯  0, 1, 2, 3
// color: 8bit(保留) + 8bit(RED) + 8bit(GREEN) + 8bit(BLUE)
void WS2812_set_color(uint16_t index, uint32_t color);

// 设置灯的亮度
// index  哪一个灯  0, 1, 2, 3
// color: 8bit(保留) + 8bit(RED) + 8bit(GREEN) + 8bit(BLUE)
// brightness: 亮度 参数0~100   代表0% ~ 100%
void WS2812_set_color_brightness(uint16_t index, uint32_t color, uint8_t brightness);

// 显示
void WS2812_display();

#endif