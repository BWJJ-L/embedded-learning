#ifndef __PCF8563_H__
#define __PCF8563_H__

#include "Config.h"
#include "GPIO.h"
#include "I2C.h"
#include "NVIC.h"
#include "Switch.h"

#define NUMBER      7
#define PCF8563_ADDR    (0x51 << 1) //Éè±¸Ð´µØÖ· 0xA2
#define PCF8563_REG     0x02    //¼Ä´æÆ÷µØÖ·: ´ÓÃëÖÓ¼Ä´æÆ÷¿ªÊ¼¶Á

typedef struct CLOCK {
    u16 years;
    u8  months;
    u8  weekdays;
    u8  days;
    u8  hours;
    u8  minutes;
    u8  seconds;
}Clock_t;

void PCF8563_init(void);

void PCF8563_set_Clock(Clock_t c);

void PCF8563_get_Clock(Clock_t * c);


#endif