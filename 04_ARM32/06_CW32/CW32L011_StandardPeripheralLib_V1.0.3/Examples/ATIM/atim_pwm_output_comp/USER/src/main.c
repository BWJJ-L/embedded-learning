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
void ATIM_IRQHandlerCallBack(void);
void ATIM_Configuration(void);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


int32_t main(void)
{  
    /* System Clocks Configuration */
    SYSCTRL_Configuration();

    /* GPIO Configuration */
    GPIO_Configuration();
    
    ATIM_Configuration();

    /* NVIC Configuration */
    NVIC_Configuration();    

    while (1)
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
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12);
    __SYSCTRL_ATIM_CLK_ENABLE();
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
    
    // PA05 ATIM_CH1
    PA05_AFx_ATIMCH1();		
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pins = GPIO_PIN_5;    
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
    
    // PA07 ATIM_CH1N
    PA07_AFx_ATIMCH1N();		
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pins = GPIO_PIN_7;    
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);  
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(ATIM_IRQn);
    __enable_irq();
}


void ATIM_Configuration(void)
{
    ATIM_InitTypeDef ATIM_InitStruct = {0};
    ATIM_OCInitTypeDef ATIM_OCInitStruct = {0};
    
    // 对ATIM进行计数的基本设置   
    ATIM_InitStruct.BufferState = DISABLE;     //不启用ARR的缓存功能    
    ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_ALIGN_MODE_CENTER_BOTH;    // 
    ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;
    ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;    
    ATIM_InitStruct.Prescaler = 7; //8分频，计数时钟1MHz
    ATIM_InitStruct.ReloadValue = 99;                // 溢出周期100us
    ATIM_InitStruct.RepetitionCounter = 0;
    ATIM_Init(&ATIM_InitStruct);
    
    ATIM_OCInitStruct.BufferState = DISABLE;
    ATIM_OCInitStruct.OCComplement = ENABLE;
    ATIM_OCInitStruct.OCFastMode = DISABLE;    
    ATIM_OCInitStruct.OCInterruptState = ENABLE;
    ATIM_OCInitStruct.OCMode = ATIM_OCMODE_PWM1;    
    ATIM_OCInitStruct.OCPolarity = ATIM_OCPOLARITY_NONINVERT;
    ATIM_OC1Init(&ATIM_OCInitStruct);

//    ATIM_ITConfig(ATIM_IT_UIE, ENABLE);    
    ATIM_SetCompare1(50-1);
    
    ATIM_SetPWMDeadtime(20, 40, ENABLE);    // 前死区为20个单位，后死区为40个单位，死区计算见用户手册
    ATIM_CH1Config(ENABLE);
    ATIM_CtrlPWMOutputs(ENABLE);
    ATIM_Cmd(ENABLE);    
}
void ATIM_IRQHandlerCallBack(void)
{
    
    
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
