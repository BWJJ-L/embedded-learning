#ifndef __APP_H__
#define __APP_H__

#include "gpio_cfg.h"

// ======= 包含驱动头文件
#include "bsp_led188.h"

// ============ 全局变量声明


//    \后面不要加空格
#define 	App_init()		App_debug_init();\
							LED188_init()
							

// ======== 调试业务
void App_debug_init();
void App_debug_test_task();

// ======== 188数码管业务
void App_debug_led188_test_task();

#endif