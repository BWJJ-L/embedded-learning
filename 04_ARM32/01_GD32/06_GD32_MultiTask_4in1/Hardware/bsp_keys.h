#ifndef __BSP_KEYS_H__
#define __BSP_KEYS_H__

#include "gpio_cfg.h"

// 按下抬起回调，在合适位置定义函数即可
// index 取值范围 1~4 代表 key1~key4
void Keys_on_keydown(uint8_t index);
void Keys_on_keyup(uint8_t index);

void Keys_init(); // 初始化
void Keys_scan(); // 按键扫描

#endif