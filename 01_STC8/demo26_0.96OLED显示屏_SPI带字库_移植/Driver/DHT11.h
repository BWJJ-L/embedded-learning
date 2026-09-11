#ifndef __DHT11_H__
#define __DHT11_H__

#include "Config.h"

void DHT11_init();

int8 DHT11_get_info(float* p_humidity, float* p_temperature);

#endif