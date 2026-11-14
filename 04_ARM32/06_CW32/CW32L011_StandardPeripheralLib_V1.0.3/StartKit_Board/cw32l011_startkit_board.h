/**
 * @file cw32l011_startkit_board.h
 * @brief 文件描述
 * @author AE TEAM
 * @version 1.0.0
 * @date 2025-03-26
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CW32L011_STARTKIT_BOARD_H
#define __CW32L011_STARTKIT_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "cw32l011.h"
#include "cw32l011_sysctrl.h"
#include "cw32l011_systick.h"
#include "cw32l011_gpio.h"
#include "cw32l011_uart.h"


/******************************************************************************/
/* Global type definitions                                                    */
/******************************************************************************/
typedef enum{
    KEY_STATE_PRESSED,  // 按下     
    KEY_STATE_RELEASED  // 释放
} KeyStateTypeDef;


typedef struct
{
    GPIO_TypeDef* Port;
    uint32_t      Pin;
}LED_TypeDef, KEY_TypeDef;



/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/

//StartKit硬件资源配置：

/********************* 配置LED端口信息 ****************************************/
#define BSP_LED1_PIN                              GPIO_PIN_8
#define BSP_LED1_PORT                             CW_GPIOA
#define BSP_LED1_PERIPH_CLK_ENABLE()              __SYSCTRL_GPIOA_CLK_ENABLE()
#define BSP_LED1_PERIPH_CLK_DISABLE()             __SYSCTRL_GPIOA_CLK_DISABLE()
#define BSP_LED1_SETHIGH()                        PA08_SETHIGH()
#define BSP_LED1_SETLOW()                         PA08_SETLOW()
#define BSP_LED1_TOG()                            PA08_TOG()

#define BSP_LED2_PIN                              GPIO_PIN_11
#define BSP_LED2_PORT                             CW_GPIOA
#define BSP_LED2_PERIPH_CLK_ENABLE()              __SYSCTRL_GPIOB_CLK_ENABLE()
#define BSP_LED2_PERIPH_CLK_DISABLE()             __SYSCTRL_GPIOB_CLK_DISABLE()
#define BSP_LED2_SETHIGH()                        PA11_SETHIGH()
#define BSP_LED2_SETLOW()                         PA11_SETLOW()
#define BSP_LED2_TOG()                            PA11_TOG()


/******************* 配置用户按键端口信息 *************************************/
#define BSP_KEY1_PIN                              GPIO_PIN_12
#define BSP_KEY1_PORT                             CW_GPIOA
#define BSP_KEY1_PERIPH_CLK_ENABLE()              __SYSCTRL_GPIOA_CLK_ENABLE()
#define BSP_KEY1_PERIPH_CLK_DISABLE()             __SYSCTRL_GPIOA_CLK_DISABLE()
#define BSP_KEY1_GETVALUE()                       PA12_GETVALUE()
#define BSP_KEY1_IRQn                             GPIOA_IRQn

#define BSP_KEY2_PIN                              GPIO_PIN_15
#define BSP_KEY2_PORT                             CW_GPIOA
#define BSP_KEY2_PERIPH_CLK_ENABLE()              __SYSCTRL_GPIOA_CLK_ENABLE()
#define BSP_KEY2_PERIPH_CLK_DISABLE()             __SYSCTRL_GPIOA_CLK_DISABLE()
#define BSP_KEY2_GETVALUE()                       PA15_GETVALUE()
#define BSP_KEY2_IRQn                             GPIOA_IRQn

/******************* 配置用于Printf的端口和外设和信息 *************************/
#define BSP_UART_TXD_PIN                         GPIO_PIN_2
#define BSP_UART_TXD_PORT                        CW_GPIOA
#define BSP_UART_TXD_AF()                        PA02_AFx_UART1TXD()
#define BSP_UART_TXD_PORT_PERIPH_CLK_ENABLE()    __SYSCTRL_GPIOA_CLK_ENABLE()
#define BSP_UART_TXD_PORT_PERIPH_CLK_DISABLE()   __SYSCTRL_GPIOA_CLK_DISABLE()

#define BSP_UART_RXD_PIN                         GPIO_PIN_3
#define BSP_UART_RXD_PORT                        CW_GPIOA
#define BSP_UART_RXD_IRQn                        GPIOA_IRQn
#define BSP_UART_RXD_AF()                        PA03_AFx_UART1RXD()
#define BSP_UART_RXD_PORT_PERIPH_CLK_ENABLE()    __SYSCTRL_GPIOA_CLK_ENABLE()
#define BSP_UART_RXD_PORT_PERIPH_CLK_DISABLE()   __SYSCTRL_GPIOA_CLK_DISABLE()

#define BSP_UART                                 CW_UART1
#define BSP_UART_IRQn                            UART1_IRQn
#define BSP_UART_PERIPH_CLK_ENABLE()             __SYSCTRL_UART1_CLK_ENABLE()
#define BSP_UART_PERIPH_CLK_DISABLE()            __SYSCTRL_UART1_CLK_DISABLE()
#define BSP_UART_PERIPH_RST_ENABLE()             __SYSCTRL_UART1_RST_ENABLE()
#define BSP_UART_PERIPH_RST_DISABLE()            __SYSCTRL_UART1_RST_DISABLE()


/******************************************************************************/
/* Global macro function                                                      */
/******************************************************************************/


/******************************************************************************/
/* Exported variables ('extern', definition in C source)                      */
/******************************************************************************/
extern LED_TypeDef LED1, LED2;
extern KEY_TypeDef KEY1, KEY2;

/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/
extern void Bsp_Uart_Init(uint32_t pclkFreq, uint32_t baudRate);
extern void Bsp_Uart_Closed(void);

extern void Bsp_Key_Init(KEY_TypeDef *KEY);
extern KeyStateTypeDef Bsp_Read_Key(KEY_TypeDef *KEY);

extern void Bsp_Led_Init(LED_TypeDef *LED);
extern void Bsp_Led_On(LED_TypeDef *LED);
extern void Bsp_Led_Off(LED_TypeDef *LED);
extern void Bsp_Led_Tog(LED_TypeDef *LED);

extern void Bsp_Init(uint32_t ClkFreq);

#ifdef __cplusplus
}
#endif

#endif /* __SYM32L010_STARTKIT_BOARD_H */



