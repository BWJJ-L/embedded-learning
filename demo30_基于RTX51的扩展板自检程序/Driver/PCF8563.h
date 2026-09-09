#ifndef __PCF8563_H__
#define __PCF8563_H__

#include "Config.h"
#include "GPIO.h"
#include "I2C.h"
#include "NVIC.h"
#include "Switch.h"

#define TIME_NUMBER     7
#define ALARM_NUMBER    4
#define PCF8563_ADDR    (0x51 << 1) //设备写地址 0xA2
#define PCF8563_REG     0x02 //寄存器地址: 从秒钟寄存器开始读,VL_seconds - 秒和时钟完整状态寄存器（地址02h）位描述
#define PCF8563_ALARM   0x09 //Minute_alarm - 分报警寄存器（地址09h）位描述

#define TIMER_HZREG     0x0E //定时器控制 - 定时器控制寄存器（地址0Eh）位描述 
#define TIMER_REG       0x0F //定时器 - 定时器值寄存器（地址0Fh）位描述

typedef struct CLOCK {
    u16 years;
    u8  months;
    u8  weekdays;
    u8  days;
    u8  hours;
    u8  minutes;
    u8  seconds;
}Clock_t;

typedef struct ALARM {
    int8 alarm_minute;
    int8 alarm_hour;
    int8 alarm_day;
    int8 alarm_week;
} Alarm_t;

void PCF8563_init(void);

void PCF8563_set_Clock(Clock_t c);

void PCF8563_get_Clock(Clock_t * c);

void PCF8563_set_Alarm(Alarm_t);

void PCF8563_enable_Alarm(u8);

void PCF8563_set_Timer(u8 frequency,u8 countdown);

void PCF8563_enable_Timer(u8 enable);

extern void PCF8563_on_Alarm(void);

extern void PCF8563_on_Timer(void);

extern void exti_int3_call(void);

#endif