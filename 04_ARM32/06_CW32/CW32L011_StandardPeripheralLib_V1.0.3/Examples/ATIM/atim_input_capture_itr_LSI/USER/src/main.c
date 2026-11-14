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
void ATIM_Configuration(void);
void BTIM_Configuration(void);
void NVIC_Configuration(void);
void ATIM_IRQHandlerCallBack(void);
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
static uint16_t Period =0 ,Width =0 , PeriodX = 0, PeriodY = 0, WidthX = 0, WidthY = 0, Flag = 0;
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  
 *         
 * @return int32_t
 */
int32_t main(void)
{
    uint32_t CurrentFreq;
    /* System Clocks Configuration */
    SYSCTRL_Configuration(); 
    
    Bsp_Init(48000000);

    /* GPIO Configuration */
    GPIO_Configuration();  
    
    ATIM_Configuration();

    /* NVIC Configuration */
    NVIC_Configuration();    
   
    ATIM_Cmd(ENABLE); 

    while(1)
    {       
        if (Flag)
        {
            Flag = 0;
            printf("Period = %.4fus, Width = %.4fus\r\n", Period/48.0, Width/48.0);
            
        }            
        
        /* 中断服务程序见ATIM_IRQHandlerCallBack() */
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void SYSCTRL_Configuration(void)
{
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI48MHZ);    //  配置时钟为48MHz
    InitTick(48000000);
    
    SYSCTRL_LSI_Enable();
    SYSCTRL_MCO_OUT(SYSCTRL_MCO_SRC_LSI, SYSCTRL_MCO_DIV1);
    
    __SYSCTRL_ATIM_CLK_ENABLE();
    __SYSCTRL_GPIOA_CLK_ENABLE();
    __SYSCTRL_GPIOB_CLK_ENABLE();
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
        
}

void ATIM_Configuration(void)
{
    ATIM_InitTypeDef ATIM_InitStruct = {0};
    ATIM_ICInitTypeDef ATIM_ICInitStruct = {0};
    
    ATIM_InitStruct.BufferState = ENABLE;               //使能缓存寄存器    
    ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_ALIGN_MODE_EDGE;    //边沿对齐
    ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;        //向上计数；
    ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;    //连续运行模式   
    ATIM_InitStruct.Prescaler = 0;    // 1分频
    ATIM_InitStruct.ReloadValue = 0xFFFF;                 // 重载周期
    ATIM_InitStruct.RepetitionCounter = 0;              // 重复周期0

    ATIM_Init(&ATIM_InitStruct);
    
    ATIM_ICInitStruct.ICFilter = ATIM_ICFILTER_NONE;
    ATIM_ICInitStruct.ICMap = ATIM_IC_MAP_TIx;     // ICx映射到TIx上
    ATIM_ICInitStruct.ICPolarity = ATIM_ICPOLOARITY_RISING;
    ATIM_ICInitStruct.ICPrescaler = ATIM_ICPRESCALER_DIV1;
    
    ATIM_IC1Init(&ATIM_ICInitStruct);
    ATIM_RemapTimerInput(ATIM_CH1, ATIM_TI_REMAP_LSI_OUT);
    
    ATIM_ICInitStruct.ICFilter = ATIM_ICFILTER_NONE;
    ATIM_ICInitStruct.ICMap = ATIM_IC_MAP_TIy;    // ICx映射到TIy上
    ATIM_ICInitStruct.ICPolarity = ATIM_ICPOLOARITY_FALLING;
    ATIM_ICInitStruct.ICPrescaler = ATIM_ICPRESCALER_DIV1;
    ATIM_IC2Init(&ATIM_ICInitStruct); 

    ATIM_ITConfig(ATIM_IT_CC1IE|ATIM_IT_CC2IE, ENABLE);
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

void ATIM_IRQHandlerCallBack(void)
{
    static uint8_t step = 0;
    //if (ATIM_GetITStatus(ATIM_STATE_CC1IF))
    if (CW_ATIM->ISR & ATIM_ISR_CC1IF_Msk)
    {
        step++;
        if (step == 2)
        {
            Flag = 1;
            step = 0;
        }
        PeriodX = CW_ATIM->CCR1;
        Period = PeriodX - PeriodY;
        PeriodY = PeriodX;        
        ATIM_ClearITPendingBit(ATIM_STATE_CC1IF);
       
        
    }
    if (ATIM_GetITStatus(ATIM_STATE_CC2IF))
    {
        ATIM_ClearITPendingBit(ATIM_STATE_CC2IF);
        Width = CW_ATIM->CCR2 -CW_ATIM->CCR1;
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

