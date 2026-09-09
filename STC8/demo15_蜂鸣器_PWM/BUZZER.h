#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "Config.h"
#include "NVIC.h"
#include "GPIO.h"
#include "Switch.h"
#include "STC8H_PWM.h"

void Buzzer_Init(void);

void Buzzer_Play(u16 hz_Value);

void Buzzer_Beep(u8 tone);

void Buzzer_Stop();

#endif