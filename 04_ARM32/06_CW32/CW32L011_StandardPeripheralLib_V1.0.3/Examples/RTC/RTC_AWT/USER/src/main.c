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
#include "../inc/main.h"
#include "cw32l011_startkit_board.h"


/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void SYSCTRL_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void delay(uint16_t ms);
void RTC_IRQHandlerCallBack(void);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

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
 **
 ******************************************************************************/

/**
 * @brief  RTC
 * @return int32_t
 */
int32_t main(void)
{
    RTC_AWTTypeDef RTC_AWTStruct = {0};  

    /* System Clocks Configuration */
    SYSCTRL_Configuration();
    
    Bsp_Init(8000000);  
    
    printf("RTC AWT Init...\r\n");
    
    RTC_SetClockSource(RTC_RTCCLK_FROM_LSE);    // 使用LSE作为时钟源
    // 设置RTCCLKD的1分频
    RTC_SetRTCCLKD_Prescaler(1-1);    // 注意：实际值需要减1
    
    RTC_AWTStruct.clockSource = RTC_AWTSOURCE_FROM_RTCCLKD;    // 选择RTCCLKD，可不开启RTC
    RTC_AWTStruct.prescaler = RTC_AWTPRS_DIV2;
    RTC_AWTStruct.reloadValue = 32768-1;
    RTC_AWTStruct.enableInterrupt = ENABLE;
    
    RTC_AWTConfig(&RTC_AWTStruct);
    /* NVIC Configuration */
    NVIC_Configuration();
       

    while(1)
    {
       SYSCTRL_GotoDeepSleep();
    }
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void SYSCTRL_Configuration(void)
{
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI8MHZ);
    SYSCTRL_LSI_Enable();
    SYSCTRL_LSE_Enable( SYSCTRL_LSE_MODE_OSC,SYSCTRL_LSE_DRIVER_LEVEL5);
    SYSCTRL_APBPeriphClk_Enable2(SYSCTRL_APB2_PERIPH_RTC, ENABLE);
}


/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(RTC_IRQn);
    __enable_irq();
}

void delay(uint16_t ms)
{
    uint32_t i;
    for( ; ms >0; ms--)
    {
        i = 796;
        while(i--);
    }
}



void RTC_IRQHandlerCallBack(void)
{
    if (RTC_GetITState(RTC_IT_AWTIMER))
    {
        RTC_ClearITPendingBit(RTC_IT_AWTIMER);
        printf("*********AWT requests an interrupt!!!!\r\n");
    }
    
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
