#ifndef __APP_H__
#define __APP_H__

#include "Config.h"
#include "GPIO.h"
#include "STC8H_PWM.h"
#include "ADC.h"
#include "NVIC.h"
#include "Switch.h"
#include "UART.h"
#include "Timer.h"
#include "I2C_OLED.h"
#include "MatrixKey.h"


#define TASK_KEYS       1
#define TASK_SPI_OLED   2
#define TASK_I2C_OLED   3
#define TASK_LED        4
#define TASK_NTC        5
#define TASK_Motor      6
#define TASK_RTC        7
#define TASK_NIXIE      8


extern int global_cur_pos;

//---------------App_OLED--------------------
void APP_I2C_OLED_Refresh(u8 clear_screen);

//---------------App_LED--------------------
void LED_show();
void TASK_LED_reset();

//---------------App_NTC--------------------
void NTC_show();
void TASK_NTC_reset();

//---------------App_MOTOR--------------------
void MOTOR_show();
void TASK_MOTOR_reset();

//---------------App_RTC--------------------
void RTC_show();
void TASK_RTC_reset();

//---------------App_NixieDigital--------------------
void NixieDigital_show();
void TASK_NIXIE_reset();

#endif