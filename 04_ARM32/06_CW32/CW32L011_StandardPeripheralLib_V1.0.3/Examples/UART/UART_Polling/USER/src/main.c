/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2023-07-21
 *
 * @copyright Copyright (c) 2022
 *
 */
/*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"
#include "interrupts_cw32l011.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
//UARTx
#define  DEBUG_UARTx                   CW_UART1
#define  DEBUG_UART_CLK                SYSCTRL_APB1_PERIPH_UART1
#define  DEBUG_UART_APBClkENx          SYSCTRL_APBPeriphClk_Enable1
#define  DEBUG_UART_BaudRate           19200
#define  DEBUG_UART_UclkFreq           8000000

//UARTx GPIO
#define  DEBUG_UART_GPIO_CLK           (SYSCTRL_AHB_PERIPH_GPIOA)
#define  DEBUG_UART_TX_GPIO_PORT       CW_GPIOA
#define  DEBUG_UART_TX_GPIO_PIN        GPIO_PIN_2
#define  DEBUG_UART_RX_GPIO_PORT       CW_GPIOA
#define  DEBUG_UART_RX_GPIO_PIN        GPIO_PIN_3

//GPIO AF
#define  DEBUG_UART_AFTX               PA02_AFx_UART1TXD()
#define  DEBUG_UART_AFRX               PA03_AFx_UART1RXD()

//中断
#define  DEBUG_UART_IRQ                UART1_IRQn

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void UART_Configuration(void);
void UART_SendBuf_Polling(UART_TypeDef *UARTx, uint8_t *TxBuf, uint8_t TxCnt);
uint8_t UART_RecvBuf_Polling(UART_TypeDef *UARTx, uint8_t *RxBuf);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t  TxRxBufferSize;
uint8_t  TxRxBuffer[] = "\r\nCW32L011 UART Polling\r\n";
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    //配置RCC
    RCC_Configuration();

    //配置GPIO
    GPIO_Configuration();

    //配置UART
    UART_Configuration();

    //使能UARTx RC中断
    UART_ITConfig(DEBUG_UARTx, UART_IT_RC, ENABLE);
    UART_SendString(DEBUG_UARTx, "\r\nCW32L011 UART Polling\r\n");
    printf("\r\nCW32L011 UART Polling Example\r\n");
	UART_ClearITPendingBit(DEBUG_UARTx, UART_IT_RC);
    while(1)
    {
		//轮询收发
        TxRxBufferSize = UART_RecvBuf_Polling(DEBUG_UARTx, TxRxBuffer);
        UART_SendBuf_Polling(DEBUG_UARTx, TxRxBuffer, TxRxBufferSize);
    }
}

/**
 * @brief 配置RCC
 *
 */
void RCC_Configuration(void)
{
    //SYSCLK = HSI = 8MHz = HCLK = PCLK
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12);

    //外设时钟使能
    SYSCTRL_AHBPeriphClk_Enable(DEBUG_UART_GPIO_CLK, ENABLE);
    DEBUG_UART_APBClkENx(DEBUG_UART_CLK, ENABLE);
}                            

/**
 * @brief 配置GPIO
 *
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pins = DEBUG_UART_TX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(DEBUG_UART_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = DEBUG_UART_RX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(DEBUG_UART_RX_GPIO_PORT, &GPIO_InitStructure);

    //UART TX RX 复用
    DEBUG_UART_AFTX;
    DEBUG_UART_AFRX;
}

/**
 * @brief 配置UART
 *
 */
void UART_Configuration(void)
{
    UART_InitTypeDef UART_InitStructure = {0};

    UART_InitStructure.UART_BaudRate = DEBUG_UART_BaudRate;
    UART_InitStructure.UART_Over = UART_Over_16;
    UART_InitStructure.UART_Source = UART_Source_PCLK;
    UART_InitStructure.UART_UclkFreq = DEBUG_UART_UclkFreq;
    UART_InitStructure.UART_StartBit = UART_StartBit_FE;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(DEBUG_UARTx, &UART_InitStructure);
}

/**
 * @brief 配置NVIC
 *
 */
void NVIC_Configuration(void)
{
    //优先级，无优先级分组
    NVIC_SetPriority(DEBUG_UART_IRQ, 0);
    //UARTx中断使能
    NVIC_EnableIRQ(DEBUG_UART_IRQ);
}


/**
 * @brief 发送8位数组
 *
 * @param USARTx :USARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、CW_UART3
 * @param TxBuf :待发送的数组
 * @param TxCnt :待发送的数组元素个数
 */
void UART_SendBuf_Polling(UART_TypeDef *UARTx, uint8_t *TxBuf, uint8_t TxCnt)
{
    while (TxCnt)
    {
        UART_SendData_8bit(UARTx, *TxBuf);
        while (UART_GetFlagStatus(UARTx, UART_FLAG_TXE) == RESET);
        TxBuf++;
        TxCnt--;
    }
    while (UART_GetFlagStatus(UARTx, UART_FLAG_TXBUSY) == SET);
}

/**
 * @brief 接收8位数组
 *
 * @param USARTx :USARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、CW_UART3
 * @param RxBuf :接收Buf
 * @return uint8_t :接收的字符个数
 */
uint8_t UART_RecvBuf_Polling(UART_TypeDef *UARTx, uint8_t *RxBuf)
{
    uint8_t RxCnt = 0;
    do
    {
        //等待RC
        while (UART_GetFlagStatus(UARTx, UART_FLAG_RC) == RESET);
        //清RC
        UART_ClearFlag(UARTx, UART_FLAG_RC);
        //ERROR: PE or FE
        if (UART_GetFlagStatus(UARTx, UART_FLAG_PE | UART_FLAG_FE))
        {
            UART_ClearFlag(UARTx, UART_FLAG_PE | UART_FLAG_FE);
            RxCnt = 0x00;
        }
        else
        {
            RxBuf[RxCnt] = UART_ReceiveData_8bit(UARTx);
            RxCnt++;
        }
    }
    while (RxBuf[RxCnt - 1] != '\n');
    return RxCnt;
}


/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

