#ifndef __BSP_LED188_H__
#define __BSP_LED188_H__

#include "gpio_cfg.h"

/*  接线说明
交互板           GD32F407
        1  ===>  PE7
        2  ===>  PE9
        3  ===>  PE11
        4  ===>  PE13
        5  ===>  PE15
       3v3 ===>  3v3(可以不接，因为是直接引脚高低控制灯)
       GND ===>	 GND
*/

// 初始化
void LED188_init();

#endif