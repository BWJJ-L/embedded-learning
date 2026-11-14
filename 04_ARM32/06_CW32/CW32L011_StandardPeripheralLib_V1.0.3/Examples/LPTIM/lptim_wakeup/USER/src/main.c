/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2021-04-22
 *
 * @copyright Copyright (c) 2021
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
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void LPTIM_Configuration(void);
void LowPowerSetup(void);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
extern uint16_t Key1Count;

/**
 * @brief
 * @return int32_t
 */
int32_t main(void)
{
    RCC_Configuration();    
    GPIO_Configuration();
    LPTIM_Configuration();

    NVIC_Configuration();

    while (1)
    {
        PA08_SETHIGH();
        SYSCTRL_GotoDeepSleep();
        PA08_SETLOW();
        SYSCTRL_GotoDeepSleep();
    }
}

void LPTIM_Configuration(void)
{
    LPTIM_InitTypeDef LPTIM_InitStruct = {0};
    __SYSCTRL_LPTIM_CLK_ENABLE();

    LPTIM_InitStruct.LPTIM_ClockSource = LPTIM_CLOCK_SOURCE_MCLK;
    LPTIM_InitStruct.LPTIM_CounterMode = LPTIM_COUNTER_MODE_TIME;
    LPTIM_InitStruct.LPTIM_Period = 32800;
    LPTIM_InitStruct.LPTIM_Prescaler = LPTIM_PRS_DIV1;

    LPTIM_Init(&LPTIM_InitStruct);

    SYSCTRL_LSI_Enable();
    LPTIM_InternalClockConfig(LPTIM_ICLK_LSI);

    LPTIM_ITConfig(LPTIM_IT_ARRM, ENABLE);
    CW_LPTIM->ICR = 0x00;

    LPTIM_Cmd(ENABLE);
    LPTIM_SelectOnePulseMode(LPTIM_OPERATION_REPETITIVE);
}


void LowPowerSetup(void)
{

}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    /* 1. 设置HCLK和PCLK的分频系数　*/
    SYSCTRL_HCLKPRS_Config(SYSCTRL_HCLK_DIV1);
    SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV1);
    SYSCTRL_SystemCoreClockUpdate(4000000);
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};   

    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pins = GPIO_PIN_11|GPIO_PIN_8;
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
    
    PA11_SETHIGH();
    PA08_SETHIGH();
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(LPTIM_IRQn);
    __enable_irq();
}





/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef USE_FULL_ASSERT
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
