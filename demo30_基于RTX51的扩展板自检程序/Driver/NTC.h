#ifndef __NTC_H__
#define __NTC_H__

#include "Config.h"


#define NTC_GPIO_INIT()  P0_MODE_IN_HIZ(GPIO_Pin_4)
#define NTC_CHANNEL      ADC_CH12

void NTC_init();

int NTC_get_temperature();

#endif