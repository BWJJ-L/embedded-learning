#ifndef __MATRIXKEY_H__
#define __MATRIXKEY_H__

#include "Config.h"
#include "GPIO.h"


#define COL1    P03
#define COL2    P06
#define COL3    P07
#define COL4    P17

#define ROW1    P34
#define ROW2    P35
#define ROW3    P40
#define ROW4    P41


#define ROW_NUM    4
#define COL_NUM    4


#define MK_GPIO_config                                   \
    P0_MODE_IO_PU(GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7); \
    P1_MODE_IO_PU(GPIO_Pin_7);                           \
    P3_MODE_IO_PU(GPIO_Pin_4 | GPIO_Pin_5);              \
    P4_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);              \


#define UP      1
#define DOWN    0

typedef void (* MK_CallBack)(u8, u8, u8);

void MK_init(void);
void MK_key_state(MK_CallBack callback);


#endif
