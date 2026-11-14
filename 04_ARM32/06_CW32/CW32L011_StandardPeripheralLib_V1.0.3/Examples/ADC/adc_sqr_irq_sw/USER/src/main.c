/**
 * @file   main.c
 * @author WHXY
 * @version 0.1
 * @date
 * @brief
 *
 * @copyright All rights reserved.
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
 */
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "..\inc\main.h"
#include "cw32l011_systick.h"
#include "cw32l011_sysctrl.h"
#include "cw32l011_gpio.h"
#include "cw32l011_adc.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

uint16_t gAdcResult[8];
volatile uint8_t gFlagIrq;



/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

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
 * @brief ADC I/O初始化
 *
 */
void ADC_PortInit(void)
{
    //打开GPIO时钟
    __SYSCTRL_GPIOA_CLK_ENABLE();
    __SYSCTRL_GPIOB_CLK_ENABLE();
    //打开ADC时钟
    __SYSCTRL_ADC_CLK_ENABLE();

    //set PA00 as AIN0 INPUT
    PA00_ANALOG_ENABLE();
    //set PA01 as AIN1 INPUT
    PA01_ANALOG_ENABLE();
    //set PA02 as AIN2 INPUT
    PA02_ANALOG_ENABLE();
    //set PA03 as AIN2 INPUT
    PA03_ANALOG_ENABLE();
    //set PA04 as AIN2 INPUT
    PA04_ANALOG_ENABLE();
    //set PA05 as AIN5 INPUT
    PA05_ANALOG_ENABLE();
    //set PA06 as AIN6 INPUT
    PA06_ANALOG_ENABLE();
    //set PA07 as AIN7 INPUT
    PA07_ANALOG_ENABLE();    
}

/**
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};

    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI8MHZ);

    //LED初始化
    LED_Init();
    InitTick(8000000);

    //配置ADC输入端口
    ADC_PortInit();

    //ADC序列转换模式配置
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div1;    // PCLK/1
    ADC_InitStructure.ADC_ConvertMode = ADC_ConvertMode_Continuous;
    ADC_InitStructure.ADC_IN0.ADC_InputChannel = ADC_InputCH0;
    ADC_InitStructure.ADC_IN0.ADC_SampTime = ADC_SampTime6Clk;
    ADC_InitStructure.ADC_IN1.ADC_InputChannel = ADC_InputCH1;
    ADC_InitStructure.ADC_IN1.ADC_SampTime = ADC_SampTime7Clk;
    ADC_InitStructure.ADC_IN2.ADC_InputChannel = ADC_InputCH2;
    ADC_InitStructure.ADC_IN2.ADC_SampTime = ADC_SampTime9Clk;
    ADC_InitStructure.ADC_IN3.ADC_InputChannel = ADC_InputCH3;
    ADC_InitStructure.ADC_IN3.ADC_SampTime = ADC_SampTime12Clk;
    ADC_InitStructure.ADC_IN4.ADC_InputChannel = ADC_InputCH4;
    ADC_InitStructure.ADC_IN4.ADC_SampTime = ADC_SampTime18Clk;
    ADC_InitStructure.ADC_IN5.ADC_InputChannel = ADC_InputCH5;
    ADC_InitStructure.ADC_IN5.ADC_SampTime = ADC_SampTime18Clk;
    ADC_InitStructure.ADC_IN6.ADC_InputChannel = ADC_InputCH6;
    ADC_InitStructure.ADC_IN6.ADC_SampTime = ADC_SampTime18Clk;
    ADC_InitStructure.ADC_IN7.ADC_InputChannel = ADC_InputCH7;
    ADC_InitStructure.ADC_IN7.ADC_SampTime = ADC_SampTime18Clk;
    ADC_InitStructure.ADC_SQREns = ADC_SqrEns0to7;

    ADC_Init(&ADC_InitStructure);

    ADC_ClearITPendingAll();
    ADC_ITConfig(ADC_IT_EOS, ENABLE);
    NVIC_EnableIRQ(ADC_IRQn);

    //ADC使能
    ADC_Enable();
    ADC_SoftwareStartConvCmd(ENABLE);

    while (1)
    {
        /* 序列通道连续采样模式 */
        if (gFlagIrq & ADC_ISR_EOS_Msk)
        {
            ADC_SoftwareStartConvCmd(DISABLE);
            PA11_SETHIGH();
            ADC_GetSqr0Result(gAdcResult);
            ADC_GetSqr1Result(&gAdcResult[1]);
            ADC_GetSqr2Result(&gAdcResult[2]);
            ADC_GetSqr3Result(&gAdcResult[3]);
            ADC_GetSqr4Result(&gAdcResult[4]);
            ADC_GetSqr5Result(&gAdcResult[5]);
            ADC_GetSqr6Result(&gAdcResult[6]);
            ADC_GetSqr7Result(&gAdcResult[7]);

            gFlagIrq = 0u;
            ADC_SoftwareStartConvCmd(ENABLE);
            PA11_SETLOW();
        }
    }
}

/**
 * @brief LED I/O口初始化
 *
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };

    //打开GPIO时钟
    __SYSCTRL_GPIOA_CLK_ENABLE();

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.Pins = GPIO_PIN_11;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    //LEDs are off.
    PA11_SETLOW();
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
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

