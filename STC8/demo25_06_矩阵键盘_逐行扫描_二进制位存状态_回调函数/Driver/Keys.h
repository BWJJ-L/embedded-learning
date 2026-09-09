#ifndef __KEYS_H__
#define __KEYS_H__

#include "Config.h"

#define KEY1    P51
#define KEY2    P52
#define KEY3    P53
#define KEY4    P54

#define KEYS_GPIO_INIT() P5_MODE_IO_PU(GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4)

void Keys_init(void);

void Keys_scan(void);

typedef void (* Keys_on_state)(u8 key_index);


extern void Keys_on_keyup(u8 key_index);
extern void Keys_on_keydown(u8 key_index);


#endif