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
uint16_t valueAdc;
uint32_t valueAdcAcc;
volatile uint8_t gFlagIrq;
uint16_t gCntEoc = 0;
uint8_t cntSample;
float fTsDegree;

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
    uint16_t cnt = 0;
    uint32_t sum = 0;
    uint16_t average;
	float AVCCvalue,tmp;
    uint16_t ADC_ResultBuff[8];
    
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI48MHZ);
    
    Bsp_Init(48000000);    
    printf("Debug serial port initialization is complete.");
    ADC_Config();
    
    while(1)
    {
        ADC_SoftwareStartConvCmd(ENABLE); //启动下一次ADC转换
        while (!(CW_ADC->ISR & ADC_ISR_EOS_Msk))
        {
            
        }
        ADC_ClearITPendingBit(ADC_IT_EOS);
        
        for (cnt = 0; cnt < sizeof(ADC_ResultBuff)/sizeof(ADC_ResultBuff[0]); cnt++)
        {
            ADC_ResultBuff[cnt] = ADC_GetConversionValue(cnt);
            
        }
        sum = 0;
        for (cnt = 0; cnt < 7; cnt++)
        {
            sum += ADC_ResultBuff[cnt];            
        }
        
        average = sum / cnt;
		AVCCvalue = ADC_BgrResult2Avcc(average);
		printf("The AVCC voltage is %3.4f\r\n", AVCCvalue);
		
		tmp =  ADC_ResultByBgr(ADC_ResultBuff[7], average);
		printf("The PA05 voltage is %3.4f\r\n", tmp);
        
        
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
    __SYSCTRL_GPIOA_CLK_ENABLE();    // 使能GPIOA外设配置时钟
    
    //set PA05 as AIN5 INPUT
    PA05_ANALOG_ENABLE();
    
    // 配置 ADC，单次转换模式
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div8;    // ADC clk = 48/8= 6MHz
    ADC_InitStructure.ADC_ConvertMode = ADC_ConvertMode_Once;    // 单次采样
    ADC_InitStructure.ADC_BgrEn = TRUE;
    ADC_InitStructure.ADC_TempSensorEn = FALSE;
    ADC_InitStructure.ADC_SQREns = ADC_SqrEns0to7;     // 8通道
    ADC_InitStructure.ADC_IN0.ADC_InputChannel = ADC_InputVref1P2;
    ADC_InitStructure.ADC_IN0.ADC_SampTime = ADC_SampTime262Clk;    // 置采样过程的持续时间不小于 40μs。40/1/6MHz=240;
    ADC_InitStructure.ADC_IN1.ADC_InputChannel = ADC_InputVref1P2;
    ADC_InitStructure.ADC_IN1.ADC_SampTime = ADC_SampTime262Clk;
    ADC_InitStructure.ADC_IN2.ADC_InputChannel = ADC_InputVref1P2;
    ADC_InitStructure.ADC_IN2.ADC_SampTime = ADC_SampTime262Clk;
    ADC_InitStructure.ADC_IN3.ADC_InputChannel = ADC_InputVref1P2;
    ADC_InitStructure.ADC_IN3.ADC_SampTime = ADC_SampTime262Clk;
    ADC_InitStructure.ADC_IN4.ADC_InputChannel = ADC_InputVref1P2;
    ADC_InitStructure.ADC_IN4.ADC_SampTime = ADC_SampTime262Clk;
    ADC_InitStructure.ADC_IN5.ADC_InputChannel = ADC_InputVref1P2;
    ADC_InitStructure.ADC_IN5.ADC_SampTime = ADC_SampTime262Clk;
    ADC_InitStructure.ADC_IN6.ADC_InputChannel = ADC_InputVref1P2;
    ADC_InitStructure.ADC_IN6.ADC_SampTime = ADC_SampTime262Clk;
    ADC_InitStructure.ADC_IN7.ADC_InputChannel = ADC_InputCH5;
    ADC_InitStructure.ADC_IN7.ADC_SampTime = ADC_SampTime9Clk;

    ADC_Init(&ADC_InitStructure);

    ADC_ClearITPendingAll(); 
    ADC_ITConfig(ADC_IT_EOS, ENABLE);    

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

