/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2022-7-12
 *
 * @copyright Copyright (c) 2021
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
#include "..\inc\main.h"
#include "cw32l011_systick.h"
#include "cw32l011_sysctrl.h"
#include "cw32l011_gpio.h"
#include "cw32l011_adc.h"
#include "cw32l011_gtim.h"
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
static void ADC_Config(void);
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
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{    
	float AVCCvalue,Temp;
    uint16_t BGR_Code, Temp_Code;
    
    
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI48MHZ);
    
    Bsp_Init(48000000);    
    printf("Debug serial port initialization is complete.");
    ADC_Config();
    
    while(1)
    {
        ADC_SoftwareStartConvCmd(ENABLE); //启动ADC转换
        while (!(CW_ADC->ISR & ADC_ISR_EOS_Msk))    // 查询等待ADC转换完成
        {
            
        }
        ADC_ClearITPendingBit(ADC_IT_EOS);
        BGR_Code = ADC_GetConversionValue(ADC_RESULT0);
        Temp_Code = ADC_GetConversionValue(ADC_RESULT1);   
        
		AVCCvalue = ADC_BgrResult2Avcc(BGR_Code);
		printf("The AVCC voltage is %3.4f\r\n", AVCCvalue);
		
		Temp =  ADC_GetTs(AVCCvalue,Temp_Code);
		printf("The Tempreture is %3.4f\r\n", Temp);       
        
    }

}

/*******************************************************************************
 * @brief  ADC 配置函数
 * @param  None
 * @retval None
 *
 */
static void ADC_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};
    
    __SYSCTRL_ADC_CLK_ENABLE();    // 使能ADC外设配置时钟    
    
    // 配置 ADC，单次转换模式
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div8;    // ADC clk = 48/8= 6MHz
    ADC_InitStructure.ADC_ConvertMode = ADC_ConvertMode_Once;    // 单次采样
    ADC_InitStructure.ADC_BgrEn = TRUE;
    ADC_InitStructure.ADC_TempSensorEn = TRUE;
    ADC_InitStructure.ADC_SQREns = ADC_SqrEns0to1;     // 2通道
    ADC_InitStructure.ADC_IN0.ADC_InputChannel = ADC_InputVref1P2;
    ADC_InitStructure.ADC_IN0.ADC_SampTime = ADC_SampTime262Clk;    // 置采样过程的持续时间不小于 40μs。40/1/6MHz=240;
    ADC_InitStructure.ADC_IN1.ADC_InputChannel = ADC_InputTs;
    ADC_InitStructure.ADC_IN1.ADC_SampTime = ADC_SampTime262Clk;    

    ADC_Init(&ADC_InitStructure);
    //ADC使能
    ADC_Enable();
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
  * @return None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

