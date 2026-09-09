#ifndef __NIXIE_H__
#define __NIXIE_H__

#include "Config.h"


// NIXIE Tube

// 数据输入引脚 (Data Input)
#define NIX_DI  P44
// 移位寄存器 Shift Clock
#define NIX_SCK P42
// 锁存寄存器 
#define NIX_RCK P43

#define NIXIE_GPIO_INIT() P4_MODE_OUT_PP(GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4)

void NIXIE_init(void);

/**********************************************************
 * @brief 根据两个字节决定显示内容
 * @param a_num 显示的内容(0是点亮, 1是熄灭) 段位端
 * @param b_idx 哪几个位显示(1是点亮, 0是熄灭) 公共端
 **********************************************************/
void NIXIE_show(u8 a_num, u8 b_idx);

/**********************************************************
 * @brief 在指定位置pos显示指定内容num_id, 每次只显示1个数字
 * @param num_id 要显示的内容的下标[0,9] [10, 19] ...
 * @param pos 显示的位置 [0, 7]
 **********************************************************/
void NIXIE_display(u8 num_id, u8 pos);

#endif