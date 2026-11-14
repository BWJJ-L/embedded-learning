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
#include "..\inc\main.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
 uint32_t Period =0 ,Width =0 ;
 uint8_t flagComplete = 0;
/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void SYSCTRL_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void PWM_InputConfig(void);

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
 * @brief  使用GTIM1的CH1通道对PWM输入信号进行测量，同一通道输入信号被两路捕获寄存器进行捕获。
 * @return int32_t
 */
int32_t main(void)
{
    /* System Clocks Configuration */
    SYSCTRL_Configuration();    
    
    Bsp_Init(96000000);

    /* GPIO Configuration */
    GPIO_Configuration();

    PWM_InputConfig();

    /* NVIC Configuration */
    NVIC_Configuration();
    
    printf("Please connect the PWM signal to pin PA06.\r\n");

    while(1)
    {
        if (flagComplete)
        {
            flagComplete = 0;
            printf("The positive pulse width of the PWM signal is %dus, and the period is %dus.\r\n", Width,Period);
        }
       
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void SYSCTRL_Configuration(void)
{   
    /* 0. HSI使能并校准 */
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1);

    /* 1. 设置HCLK和PCLK的分频系数　*/
    SYSCTRL_HCLKPRS_Config(SYSCTRL_HCLK_DIV1);
    SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV1);
  
    SYSCTRL_SystemCoreClockUpdate(96000000);    
    SYSCTRL_MCO_OUT(SYSCTRL_MCO_SRC_HCLK,SYSCTRL_MCO_DIV8);    //PA04输出12MHz波形    
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    // PA06: GTIM1CH1
    __SYSCTRL_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pins = GPIO_PIN_6 ;
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
    PA06_AFx_GTIM1CH1();
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(GTIM1_IRQn);
    __enable_irq();
}

void PWM_InputConfig(void)
{
    GTIM_InitTypeDef GTIM_InitStruct = {0};
    GTIM_ICModeCfgTypeDef GTIM_ICModeCfgStruct = {0};
	GTIM_SlaverCfgTypeDef GTIM_SlaverCfgStruct = {0};
    __SYSCTRL_GTIM1_CLK_ENABLE();
				
    GTIM_InitStruct.AlignMode = GTIM_ALIGN_MODE_EDGE;         //边沿对齐
    GTIM_InitStruct.ARRBuffState = GTIM_ARR_BUFF_EN;          //使能缓存寄存器
    GTIM_InitStruct.Direction = GTIM_DIRECTION_UP;            //向上计数
    GTIM_InitStruct.EventOption = GTIM_EVENT_NORMAL;
    GTIM_InitStruct.Prescaler = 96-1;                            // 96分频
    GTIM_InitStruct.PulseMode = GTIM_PULSE_MODE_DIS;
    GTIM_InitStruct.ReloadValue = 0xFFFF;
    GTIM_InitStruct.UpdateOption = GTIM_UPDATE_EN;        
    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);

    GTIM_ICModeCfgStruct.ICMap = GTIM_IC1_MAP_TI1;                //IC1 映射到 TI1 上
    GTIM_ICModeCfgStruct.ICPrescaler = GTIM_IC_PRESCALER_DIV1;
    GTIM_ICModeCfgStruct.ICFilter = GTIM_IC_FILTER_PCLK_NONE ;
    GTIM_ICModeCfgStruct.ICPolarity = GTIM_ICPolarity_Rising ;    //检测 IC1 上升沿
    GTIM_IC1ModeCfg(CW_GTIM1, &GTIM_ICModeCfgStruct);
	GTIM_IC1Cmd(CW_GTIM1, ENABLE);
				
    GTIM_ICModeCfgStruct.ICMap = GTIM_IC2_MAP_TI1;                //IC2 映射到 TI1 上
    GTIM_ICModeCfgStruct.ICPrescaler = GTIM_IC_PRESCALER_DIV1;
    GTIM_ICModeCfgStruct.ICFilter = GTIM_IC_FILTER_PCLK_NONE ;
    GTIM_ICModeCfgStruct.ICPolarity = GTIM_ICPolarity_Falling;    //检测 IC2下降沿
    GTIM_IC2ModeCfg(CW_GTIM1, &GTIM_ICModeCfgStruct);
	GTIM_IC2Cmd(CW_GTIM1, ENABLE);

	GTIM_SlaverCfgStruct.SlaverMode = GTIM_SLAVER_MODE_RESET;     //GTIM 工作于复位模式
    GTIM_SlaverCfgStruct.TrigInput = GTIM_TRIG_INPUT_TI1FP1;      //TRGI 输入为 TI1FP1
	GTIM_SlaverModeCfg(CW_GTIM1, &GTIM_SlaverCfgStruct);
				
	GTIM_ITConfig(CW_GTIM1, GTIM_IT_CC1 | GTIM_IT_CC2 | GTIM_IT_UPDATA, ENABLE);	
	GTIM_Cmd(CW_GTIM1, ENABLE);
}

void GTIM1_IRQHandlerCallback(void)
{
    /* USER CODE BEGIN */
    if (GTIM_GetITStatus(CW_GTIM1, GTIM_IT_UPDATA))
    {					 
        GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_UPDATA);
    }
    if (GTIM_GetITStatus(CW_GTIM1, GTIM_IT_CC1))
    {

		Period = CW_GTIM1->CCR1;    
        GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_CC1);
    }
    if (GTIM_GetITStatus(CW_GTIM1, GTIM_IT_CC2))
    {
		Width = CW_GTIM1->CCR2;
        GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_CC2);
        flagComplete = 1;
    }  
    /* USER CODE END */
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
