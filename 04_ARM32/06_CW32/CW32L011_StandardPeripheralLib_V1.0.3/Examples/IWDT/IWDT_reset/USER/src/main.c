/**
 * @file main.c
 * @author WHXY 
 * @version 0.1
 * @date 2022-7-12
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
#define IWDT_DURATION        250    // 250ms
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
 * @brief
 * @return int32_t
 */
int32_t main(void)
{
    
    /* System Clocks Configuration */
    SYSCTRL_Configuration();

    /* GPIO Configuration */
    GPIO_Configuration();
    
    // 如果程序发生的看门狗复位，LED2常亮，并停止运行，需要手动NRST复位。
    if (SYSCTRL_GetRstFlag(SYSCTRL_RESETFLAG_IWDT ))
    {
        PA11_SETHIGH();
        SYSCTRL_ClearRstFlag(SYSCTRL_RESETFLAG_IWDT);
        while(1);
    }

    SysTick_Config(SystemCoreClock / 1000 * 250);    // 250ms
    
    IWDT_SetPeriod(IWDT_2_SECS); 
    
    NVIC_Configuration();

    while(1)
    {

    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void SYSCTRL_Configuration(void)
{    
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV2);

    SYSCTRL_HCLKPRS_Config(SYSCTRL_HCLK_DIV1);
    SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV1);
   
    SYSCTRL_SystemCoreClockUpdate(48000000);

    __SYSCTRL_IWDT_CLK_ENABLE(); 
    __SYSCTRL_GPIOB_CLK_ENABLE();
    __SYSCTRL_GPIOA_CLK_ENABLE();
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.IT = GPIO_IT_FALLING;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pins = GPIO_PIN_15;
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
				
	GPIO_InitStruct.Pins = GPIO_PIN_11  ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);

    PA11_SETLOW();
        
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
//  NVIC_EnableIRQ(WDT_IRQn);
    NVIC_SetPriority(GPIOA_IRQn, 0);
    NVIC_SetPriority(SysTick_IRQn, 1);
    NVIC_EnableIRQ(GPIOA_IRQn);
    __enable_irq();
}

void GPIOA_IRQHandlerCallback(void)
{
    PA11_SETLOW();
}

void SysTick_HandlerCallback(void)
{
	PA11_TOG();
    IWDT_Refresh();
}

void WDT_IRQHandlerCallback(void)
{

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

