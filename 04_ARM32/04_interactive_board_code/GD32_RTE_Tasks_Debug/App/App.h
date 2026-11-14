#ifndef __APP_H__
#define __APP_H__

#include "gpio_cfg.h"

// ======= 包含驱动头文件

// ============ 全局变量声明


//    \后面不要加空格
#define 	App_init()		App_debug_init()
							

// ======== 调试业务
void App_debug_init();
void App_debug_test_task();

#endif