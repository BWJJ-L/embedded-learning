#ifndef __EXTI_CONFIG_H__
#define __EXTI_CONFIG_H__

#include "gd32f4xx.h"

#define USE_EXTI_0     0
#define USE_EXTI_1     0
#define USE_EXTI_2     0
#define USE_EXTI_3     0
#define USE_EXTI_4     0
#define USE_EXTI_5     0
#define USE_EXTI_6     0
#define USE_EXTI_7     0
#define USE_EXTI_8     0
#define USE_EXTI_9     0
#define USE_EXTI_10    0
#define USE_EXTI_11    0
#define USE_EXTI_12    0
#define USE_EXTI_13    0
#define USE_EXTI_14    0
#define USE_EXTI_15    0

#if USE_EXTI_0
	#define EXTI0_RCU           RCU_GPIOA
	#define EXTI0_PORT          GPIOA
	#define EXTI0_PUPD			GPIO_PUPD_NONE
	#define EXTI0_SOURCE_PORT   EXTI_SOURCE_GPIOA
	#define EXTI0_TRIG_TYPE     EXTI_TRIG_BOTH
	#define EXTI0_PRIORITY		2, 2
#endif

#if USE_EXTI_1
	#define EXTI1_RCU           RCU_GPIOC
	#define EXTI1_PORT          GPIOC  
	#define EXTI1_PUPD          GPIO_PUPD_PULLUP
	#define EXTI1_SOURCE_PORT   EXTI_SOURCE_GPIOC
	#define EXTI1_TRIG_TYPE     EXTI_TRIG_BOTH
	#define EXTI1_PRIORITY		2, 2
#endif

#if USE_EXTI_2
	#define EXTI2_RCU           RCU_GPIOC
	#define EXTI2_PORT          GPIOC
	#define EXTI2_PUPD			GPIO_PUPD_PULLUP
	#define EXTI2_SOURCE_PORT   EXTI_SOURCE_GPIOC
	#define EXTI2_TRIG_TYPE     EXTI_TRIG_BOTH
	#define EXTI2_PRIORITY	2, 2
#endif

#if USE_EXTI_3
    #define EXTI3_RCU           RCU_GPIOC
    #define EXTI3_PORT          GPIOC
    #define EXTI3_PUPD          GPIO_PUPD_PULLUP
    #define EXTI3_SOURCE_PORT   EXTI_SOURCE_GPIOC
    #define EXTI3_TRIG_TYPE     EXTI_TRIG_BOTH
    #define EXTI3_PRIORITY      2, 2
#endif

#if USE_EXTI_4
    #define EXTI4_RCU           RCU_GPIOC
    #define EXTI4_PORT          GPIOC
    #define EXTI4_PUPD          GPIO_PUPD_PULLUP
    #define EXTI4_SOURCE_PORT   EXTI_SOURCE_GPIOC
    #define EXTI4_TRIG_TYPE     EXTI_TRIG_BOTH
    #define EXTI4_PRIORITY      2, 2
#endif

#if USE_EXTI_5
    #define EXTI5_RCU           RCU_GPIOC
    #define EXTI5_PORT          GPIOC
    #define EXTI5_PUPD          GPIO_PUPD_PULLUP
    #define EXTI5_SOURCE_PORT   EXTI_SOURCE_GPIOC
    #define EXTI5_TRIG_TYPE     EXTI_TRIG_BOTH
    #define EXTI5_PRIORITY      2, 2
#endif

#if USE_EXTI_6
    #define EXTI6_RCU           RCU_GPIOC
    #define EXTI6_PORT          GPIOC
    #define EXTI6_PUPD          GPIO_PUPD_PULLUP
    #define EXTI6_SOURCE_PORT   EXTI_SOURCE_GPIOC
    #define EXTI6_TRIG_TYPE     EXTI_TRIG_BOTH
    #define EXTI6_PRIORITY      2, 2
#endif

#if USE_EXTI_7
    #define EXTI7_RCU           RCU_GPIOC
    #define EXTI7_PORT          GPIOC
    #define EXTI7_PUPD          GPIO_PUPD_PULLUP
    #define EXTI7_SOURCE_PORT   EXTI_SOURCE_GPIOC
    #define EXTI7_TRIG_TYPE     EXTI_TRIG_BOTH
    #define EXTI7_PRIORITY      2, 2
#endif

#if USE_EXTI_8
    #define EXTI8_RCU           RCU_GPIOC
    #define EXTI8_PORT          GPIOC
    #define EXTI8_PUPD          GPIO_PUPD_PULLUP
    #define EXTI8_SOURCE_PORT   EXTI_SOURCE_GPIOC
    #define EXTI8_TRIG_TYPE     EXTI_TRIG_BOTH
    #define EXTI8_PRIORITY      2, 2
#endif

#if USE_EXTI_9
    #define EXTI9_RCU           RCU_GPIOC
    #define EXTI9_PORT          GPIOC
    #define EXTI9_PUPD          GPIO_PUPD_PULLUP
    #define EXTI9_SOURCE_PORT   EXTI_SOURCE_GPIOC
    #define EXTI9_TRIG_TYPE     EXTI_TRIG_BOTH
    #define EXTI9_PRIORITY      2, 2
#endif

#if USE_EXTI_10
    #define EXTI10_RCU          RCU_GPIOC
    #define EXTI10_PORT         GPIOC
    #define EXTI10_PUPD         GPIO_PUPD_PULLUP
    #define EXTI10_SOURCE_PORT  EXTI_SOURCE_GPIOC
    #define EXTI10_TRIG_TYPE    EXTI_TRIG_BOTH
    #define EXTI10_PRIORITY     2, 2
#endif

#if USE_EXTI_11
    #define EXTI11_RCU          RCU_GPIOC
    #define EXTI11_PORT         GPIOC
    #define EXTI11_PUPD         GPIO_PUPD_PULLUP
    #define EXTI11_SOURCE_PORT  EXTI_SOURCE_GPIOC
    #define EXTI11_TRIG_TYPE    EXTI_TRIG_BOTH
    #define EXTI11_PRIORITY     2, 2
#endif

#if USE_EXTI_12
    #define EXTI12_RCU          RCU_GPIOC
    #define EXTI12_PORT         GPIOC
    #define EXTI12_PUPD         GPIO_PUPD_PULLUP
    #define EXTI12_SOURCE_PORT  EXTI_SOURCE_GPIOC
    #define EXTI12_TRIG_TYPE    EXTI_TRIG_BOTH
    #define EXTI12_PRIORITY     2, 2
#endif

#if USE_EXTI_13
    #define EXTI13_RCU          RCU_GPIOC
    #define EXTI13_PORT         GPIOC
    #define EXTI13_PUPD         GPIO_PUPD_PULLUP
    #define EXTI13_SOURCE_PORT  EXTI_SOURCE_GPIOC
    #define EXTI13_TRIG_TYPE    EXTI_TRIG_BOTH
    #define EXTI13_PRIORITY     2, 2
#endif

#if USE_EXTI_14
    #define EXTI14_RCU          RCU_GPIOC
    #define EXTI14_PORT         GPIOC
    #define EXTI14_PUPD         GPIO_PUPD_PULLUP
    #define EXTI14_SOURCE_PORT  EXTI_SOURCE_GPIOC
    #define EXTI14_TRIG_TYPE    EXTI_TRIG_BOTH
    #define EXTI14_PRIORITY     2, 2
#endif

#if USE_EXTI_15
    #define EXTI15_RCU          RCU_GPIOC
    #define EXTI15_PORT         GPIOC
    #define EXTI15_PUPD         GPIO_PUPD_PULLUP
    #define EXTI15_SOURCE_PORT  EXTI_SOURCE_GPIOC
    #define EXTI15_TRIG_TYPE    EXTI_TRIG_BOTH
    #define EXTI15_PRIORITY     2, 2
#endif

#endif