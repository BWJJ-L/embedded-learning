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
uint16_t valueAdc;
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
    //打开ADC时钟
    __SYSCTRL_ADC_CLK_ENABLE();

    //set PA02 as AIN2 INPUT
    PA02_ANALOG_ENABLE();
}

/**
 * @brief LED I/O初始化
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
/**
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{   
    uint32_t TicksCount;
    ADC_WatchdogTypeDef ADC_WdtStructure = {0};
    
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI8MHZ); //配置系统时钟为HSI 8MHz
    InitTick(8000000ul); //初始化SysTick

    //配置ADC测试IO
    ADC_PortInit();

    //LED初始化
    LED_Init();

    
    //ADC模拟看门狗通道初始化
    ADC_WdtStructure.ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div8;
    ADC_WdtStructure.ADC_InitStruct.ADC_ConvertMode = ADC_ConvertMode_Continuous;
    ADC_WdtStructure.ADC_InitStruct.ADC_SQREns = ADC_SqrEns0to0;
    ADC_WdtStructure.ADC_InitStruct.ADC_IN0.ADC_InputChannel = ADC_InputCH2;
    ADC_WdtStructure.ADC_InitStruct.ADC_IN0.ADC_SampTime = ADC_SampTime390Clk;
    ADC_WdtStructure.ADC_WatchdogCHx = ADC_WATCHDOG_IN2;    // 模拟看门狗监控的是输入通道
    ADC_WdtStructure.ADC_WatchdogOverHighIrq = ENABLE;
    ADC_WdtStructure.ADC_WatchdogUnderLowIrq = ENABLE;
    ADC_WdtStructure.ADC_WatchdogVth = 0x700;
    ADC_WdtStructure.ADC_WatchdogVtl = 0x600;
    
    ADC_WatchdogInit(&ADC_WdtStructure);
    ADC_ITConfig(ADC_IT_AWDH|ADC_IT_AWDL|ADC_IT_EOC, ENABLE);
    ADC_ClearITPendingAll();
    
    NVIC_EnableIRQ(ADC_IRQn);
    //ADC使能
    ADC_Enable();
    ADC_SoftwareStartConvCmd(ENABLE);
    
    TicksCount = GetTick();

    while (1)
    {
        
        if ((gFlagIrq & ADC_ISR_EOC_Msk) && !(gFlagIrq & (ADC_ISR_AWDH_Msk | ADC_ISR_AWDL_Msk)))
        {
            gFlagIrq &= ~ADC_ISR_EOC_Msk;
            valueAdc = ADC_GetConversionValue(ADC_RESULT_0);
            if (GetTick() - TicksCount >=1000)
            {
                PA11_TOG();
                TicksCount = GetTick();
            }
        }
        if (gFlagIrq & ADC_ISR_AWDH_Msk)
        {
            PA11_SETHIGH();
            gFlagIrq &= ~ADC_ISR_AWDH_Msk;
            continue;            
        }
        else if (gFlagIrq & ADC_ISR_AWDL_Msk)
        {
            PA11_SETLOW();
            gFlagIrq &= ~ADC_ISR_AWDL_Msk;
            continue;
        }
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

