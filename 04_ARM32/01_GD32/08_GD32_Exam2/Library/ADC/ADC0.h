#ifndef __ADC0_H__
#define __ADC0_H__

#include "gpio_cfg.h"

#define CH0_PIN	 	RCU_GPIOA,GPIOA,GPIO_PIN_0
#define CH1_PIN	 	RCU_GPIOA,GPIOA,GPIO_PIN_1
#define CH2_PIN	 	RCU_GPIOA,GPIOA,GPIO_PIN_2
#define CH3_PIN	 	RCU_GPIOA,GPIOA,GPIO_PIN_3
#define CH4_PIN	 	RCU_GPIOA,GPIOA,GPIO_PIN_4
#define CH5_PIN	 	RCU_GPIOA,GPIOA,GPIO_PIN_5
#define CH6_PIN	 	RCU_GPIOA,GPIOA,GPIO_PIN_6
#define CH7_PIN	 	RCU_GPIOA,GPIOA,GPIO_PIN_7
#define CH14_PIN	RCU_GPIOC,GPIOC,GPIO_PIN_4

#define ADC0_CLK		ADC_ADCCK_PCLK2_DIV4 // 84/分频系数

/**************** ADC 0 *****************/
// 分辨率
#define	ADC0_RESOLUTION 	ADC_RESOLUTION_12B 

//// 转换个数，容器个数
//#define ADC0_CHNS_LEN		3	
//// 通道
//#define ADC0_CHNS			{	\
//								ADC_CHANNEL_2,		\
//								ADC_CHANNEL_3,		\
//								ADC_CHANNEL_4,		\
//							}
//// 采样频率
//#define ADC0_CHNS_SAMPLES	{	\
//								ADC_SAMPLETIME_84,	\
//								ADC_SAMPLETIME_84,	\
//								ADC_SAMPLETIME_84,	\
//							}

// 转换个数，容器个数
#define ADC0_CHNS_LEN		1	
// 通道
#define ADC0_CHNS			{	\
								ADC_CHANNEL_2,		\
							}
// 采样频率
#define ADC0_CHNS_SAMPLES	{	\
								ADC_SAMPLETIME_84,	\
							}

void ADC0_init();	// 初始化

uint16_t ADC0_get(uint8_t index); // 获取采样结果


#endif //__ADC0_H__