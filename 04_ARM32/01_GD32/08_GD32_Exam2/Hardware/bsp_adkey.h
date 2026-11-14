#ifndef __BSP_ADKEY_H__
#define __BSP_ADKEY_H__

#include "gpio_cfg.h"

//////////////////// ADKey ////////////////////
/*  接线说明
交互板           GD32F407
        K  ===>  PA2
       3v3 ===>  3v3
       GND ===>	 GND
*/
void ADKey_init(); // 初始化
void ADKey_scan(); // 扫描

void ADCKey_on_click(uint8_t i); // 单击，短按
void ADCKey_on_long_click(uint8_t i); // 长按

#endif