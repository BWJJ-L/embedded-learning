#ifndef __DHT11_H__
#define __DHT11_H__

#include "Config.h"
#include "GPIO.h"
#include "delay.h" 

void DHT11_init(void);

int8 DHT11_get_info(float* p_humidity, float* p_temperature);

#endif