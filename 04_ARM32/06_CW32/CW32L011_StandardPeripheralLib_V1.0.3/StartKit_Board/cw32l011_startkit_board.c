/**
 * @file cw32l011_startkit_board.c
 * @brief 文件描述
 * @author AE TEAM
 * @version 1.0.0
 * @date 2025-03-26
 */

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "cw32l011_startkit_board.h"

KEY_TypeDef KEY1 = {.Port = BSP_KEY1_PORT, .Pin = BSP_KEY1_PIN};
KEY_TypeDef KEY2 = {.Port = BSP_KEY2_PORT, .Pin = BSP_KEY2_PIN};
LED_TypeDef LED1 = {.Port = BSP_LED1_PORT, .Pin = BSP_LED1_PIN};
LED_TypeDef LED2 = {.Port = BSP_LED2_PORT, .Pin = BSP_LED2_PIN};



/*******************************************************************************
 * @brief  用于支持Printf打印的函数，
 * @retval None
 * @note   需要使用microlib库
 */
#if (__ARMCC_VERSION > 6000000)
    __asm (".global __use_no_semihosting\n\t");
    void _sys_exit(int x)
    {
        x = x;
    }
    /* __use_no_semihosting was requested, but _ttywrch was */
    void _ttywrch(int ch)
    {
        ch = ch;
    }
    FILE __stdout;
#else
    #ifdef __CC_ARM
    #pragma import(__use_no_semihosting)
    struct __FILE
    {
        int handle;
    };
    FILE __stdout;
    void _sys_exit(int x)
    {
        x = x;
    }
    /* __use_no_semihosting was requested, but _ttywrch was */
    void _ttywrch(int ch)
    {
        ch = ch;
    }
    #endif
#endif
    
// 检测 IAR 版本
#if defined(__IAR_SYSTEMS_ICC__) && (__VER__ >= 9000000)
    // IAR 9.x 版本
    #define IAR_VERSION 9
#elif defined(__IAR_SYSTEMS_ICC__) && (__VER__ >= 8000000)
    // IAR 8.x 版本
    #define IAR_VERSION 8
#endif
    
#if defined (__GNUC__) && !defined (__clang__)
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

// 发送单个字符
void UART_SendChar(char ch)
{
    while ((BSP_UART->ISR & UARTx_ISR_TXBUSY_Msk) == UARTx_ISR_TXBUSY_Msk); // 等待发送完成
    BSP_UART->TDR = ch; // 发送字符
}
/*******************************************************************************
 * @brief  重定向C库中Printf函数到UART.
 * @retval None
 * @note   None
 */
__WEAK PUTCHAR_PROTOTYPE
{
    UART_SendChar((char)ch);
    return ch;
}

// IAR 9.x 使用 __write 重定向
#if IAR_VERSION == 9
size_t __write(int handle, const unsigned char *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        UART_SendChar(buffer[i]);
    }
    return size;
}
#endif

/*******************************************************************************
  * @brief  板载UART打印初始化
  * @param  pclkFreq: pclk freq
  * @param  baudRate: uart baudrate
  * @retval none
  * @note   
  */
void Bsp_Uart_Init(uint32_t pclkFreq, uint32_t baudRate)
{
    uint32_t BRR;    
    /* configure the uart txd pin */
    BSP_UART_TXD_PORT_PERIPH_CLK_ENABLE();
    BSP_UART_TXD_AF();
    BSP_UART_TXD_PORT->ANALOG &= ~(BSP_UART_TXD_PIN);

    /* configure the uart rxd pin */
    BSP_UART_RXD_PORT_PERIPH_CLK_ENABLE();
    BSP_UART_RXD_AF();
    BSP_UART_RXD_PORT->ANALOG &= ~(BSP_UART_RXD_PIN);
    
    /* configure uart */
    BSP_UART_PERIPH_CLK_ENABLE();
    BSP_UART_PERIPH_RST_ENABLE();
    BSP_UART_PERIPH_RST_DISABLE();
    
    BRR = (pclkFreq + (baudRate >> 1) )/ baudRate;    // 加0.5倍的baudRate，相对于4舍5入
    BSP_UART->BRRI =  BRR >> 4;
    BSP_UART->BRRF =  BRR % 16;
    BSP_UART->CR1  = UARTx_CR1_TXEN_Msk | UARTx_CR1_RXEN_Msk;
}

void Bsp_Uart_Closed(void)
{
    BSP_UART_PERIPH_CLK_DISABLE();
    
    BSP_UART_TXD_PORT->ANALOG |= (BSP_UART_TXD_PIN);
    BSP_UART_RXD_PORT->ANALOG |= (BSP_UART_RXD_PIN);
    
    BSP_UART_TXD_PORT_PERIPH_CLK_DISABLE();
    BSP_UART_RXD_PORT_PERIPH_CLK_DISABLE(); 
}

/*******************************************************************************
  * @brief  板载按键初始化
  * @param  KEY
  * @retval None
  * @note   
  */
void Bsp_Key_Init(KEY_TypeDef *KEY)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.IT = GPIO_IT_FALLING;    // 下降沿中断使能
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_InitStruct.Pins = KEY->Pin;

    GPIO_Init(KEY->Port, &GPIO_InitStruct);

//    NVIC_ClearPendingIRQ(BSP_KEY_IRQn);
//    NVIC_EnableIRQ(BSP_KEY_IRQn);

}

/*******************************************************************************
  * @brief  读取板载按键的状态
  * @param  none
  * @retval 返回板载按键的电平状态
  *         
  */
KeyStateTypeDef Bsp_Read_Key(KEY_TypeDef *KEY)
{    
    uint32_t currentInput;

    currentInput= !!(KEY->Port->IDR & KEY->Pin);  // 读取引脚状态  
    
    return currentInput ? KEY_STATE_RELEASED : KEY_STATE_PRESSED;
}


/*******************************************************************************
  * @brief  板载LED初始化
  * @param  LED
  * @retval None
  * @note   
  */
void Bsp_Led_Init(LED_TypeDef *LED)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pins = LED->Pin;

    GPIO_Init(LED->Port, &GPIO_InitStruct);

    Bsp_Led_Off(LED);
}


/*******************************************************************************
  * @brief  点亮板载LED
  * @param  LED
  * @retval None
  */
void Bsp_Led_On(LED_TypeDef *LED)
{
    LED->Port->BSRR |= LED->Pin; 
}


/*******************************************************************************
  * @brief  熄灭板载LED
  * @param  LED
  * @retval None
  */
void Bsp_Led_Off(LED_TypeDef *LED)
{
    LED->Port->BRR |= LED->Pin; 
}


/*******************************************************************************
  * @brief  翻转板载LED
  * @param  LED
  * @retval None
  */
void Bsp_Led_Tog(LED_TypeDef *LED)
{
    LED->Port->TOG |= LED->Pin; 
}


/*******************************************************************************
  * @brief  板载硬件接口初始化
  * @param  None
  * @retval None
  */
 void Bsp_Init(uint32_t ClkFreq)
 {     
     Bsp_Led_Init(&LED1);
     Bsp_Led_Init(&LED2);
     Bsp_Key_Init(&KEY1);
     Bsp_Key_Init(&KEY2);
     Bsp_Uart_Init(ClkFreq, 115200);
 }

/************************ (C) COPYRIGHT  *****END OF FILE*************/
