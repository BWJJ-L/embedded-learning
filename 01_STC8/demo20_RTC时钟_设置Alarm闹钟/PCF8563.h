#ifndef __PCF8563_H__
#define __PCF8563_H__

#include "Config.h"
#include "GPIO.h"
#include "I2C.h"
#include "NVIC.h"
#include "Switch.h"

#define TIME_NUMBER     7
#define ALARM_NUMBER    4
#define PCF8563_ADDR    (0x51 << 1) //Éè±¸Ð´µØÖ· 0xA2
#define PCF8563_REG     0x02    //¼Ä´æÆ÷µØÖ·: ´ÓÃëÖÓ¼Ä´æÆ÷¿ªÊ¼¶Á
#define PCF8563_ALARM   0x09
#define PCF8563_ENALARM 0x01

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

#endif