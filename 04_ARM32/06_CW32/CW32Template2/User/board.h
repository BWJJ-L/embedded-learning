#ifndef __BOARD_H__
#define __BOARD_H__

#include "system_cw32l011.h"
#include "interrupts_cw32l011.h"
#include "cw32l011_sysctrl.h"
#include "cw32l011_gpio.h"
#include "cw32l011_uart.h"
#include "base_types.h"
#include "cw32l011_systick.h"
#include "interrupts_cw32l011.h"
#include "cw32l011_adc.h"
#include <math.h> // 必须包含这一行，否则 logf 可能会报错或行为异常
#include "cw32l011_i2c.h"
#include "cw32l011_spi.h"

void board_init(void);

#define delay_1us(__us)		delay_us(__us)
#define delay_1ms(ms)		delay_ms(ms)

//搭配滴答定时器实现的精确us延时
void delay_us(unsigned long __us);
void delay_ms(unsigned long ms);

#endif