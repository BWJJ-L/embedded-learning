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
 #define encoderlines      1024
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
void GTIM_Configuration(void);
void NVIC_Configuration(void);
void ATIM_IRQHandlerCallBack(void);
void GTIM_IRQHandlerCallBack(void);
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
 int icount=0;
 uint16_t circle_count1;
 uint16_t circle_count2;
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

    /* GPIO Configuration */
    GPIO_Configuration();

    /* GTIM Configuration */
	GTIM_Configuration();
    
    /*ATIM Configuration */
    ATIM_Configuration();

    /* NVIC Configuration */
    NVIC_Configuration();    
   
    ATIM_Cmd(ENABLE);    

    while(1)
					
    {
        if(circle_count1>=circle_count2)
        { 
            float forwardspeed=(circle_count1-circle_count2)*100.0f/encoderlines;  //计算单位时间对应的脉冲数，再由编码器线数获得转速
        }   
        if(circle_count1<circle_count2)
        { 
            float reversespeed=(circle_count1-circle_count2)*100.0f/encoderlines;
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
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI8MHZ);    //  配置PCLK为8MHz
    InitTick(8000000);
    __SYSCTRL_GTIM1_CLK_ENABLE();
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
    GPIO_InitTypeDef GPIO_InitStruct = {0};   
    
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pins = GPIO_PIN_5| GPIO_PIN_9;
    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
    
    PA05_AFx_ATIMCH1();
    PA09_AFx_ATIMCH2();  
}                            

void ATIM_Configuration(void)
{
    ATIM_ICInitTypeDef ATIM_ICInitStruct = {0};
    ATIM_ICInitStruct.ICFilter = ATIM_ICFILTER_NONE;
    ATIM_IC1Init(&ATIM_ICInitStruct);
    ATIM_RemapTimerInput(ATIM_CH1, ATIM_TI_REMAP_ATIM_CHx);
    ATIM_RemapTimerInput(ATIM_CH2, ATIM_TI_REMAP_ATIM_CHx);
	ATIM_ICInitStruct.ICFilter = ATIM_ICFILTER_NONE;  
    ATIM_IC2Init(&ATIM_ICInitStruct);
	ATIM_SlaverModeConfig(0x03);        //正交编码计数模式 -x4 模式
	ATIM_SetReloadValue(999);
    ATIM_ITConfig(ATIM_IT_UIE, ENABLE);
}

void GTIM_Configuration(void)
{                                                                   
    GTIM_InitTypeDef GTIM_InitStruct = {0};
    
    GTIM_InitStruct.AlignMode = GTIM_ALIGN_MODE_EDGE;
    GTIM_InitStruct.ARRBuffState = GTIM_ARR_BUFF_EN;
    GTIM_InitStruct.Direction = GTIM_DIRECTION_UP;
    GTIM_InitStruct.EventOption = GTIM_EVENT_NORMAL;
    GTIM_InitStruct.Prescaler = 800 - 1;
    GTIM_InitStruct.PulseMode = GTIM_PULSE_MODE_DIS;
    GTIM_InitStruct.ReloadValue = 100 -1;                   //定时器计数10ms溢出一次并触发中断
    GTIM_InitStruct.UpdateOption = GTIM_UPDATE_EN;
    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);
    GTIM_ITConfig(CW_GTIM1,GTIM_IT_UPDATA, ENABLE);	
    GTIM_Cmd(CW_GTIM1, ENABLE);
    GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_UPDATA);
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
    NVIC_EnableIRQ(GTIM1_IRQn);
    __enable_irq();
}

void ATIM_IRQHandlerCallBack(void)
{
    if (ATIM_GetITStatus(ATIM_IT_UIE))
    {
         ATIM_ClearITPendingBit(ATIM_IT_UIE);//清空中断标志位       
    }                                                    
}

void GTIM_IRQHandlerCallBack(void)
{
    if ((GTIM_GetITStatus(CW_GTIM1, GTIM_IT_UPDATA))&( icount==0))
	{
        icount++;
		circle_count1 =CW_ATIM->CNT;
		GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_UPDATA);//清空中断标志位
	} 
    if ((GTIM_GetITStatus(CW_GTIM1, GTIM_IT_UPDATA))&( icount==1))
	{
        icount=0;
		circle_count2 =CW_ATIM->CNT;
		GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_UPDATA);//清空中断标志位
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

