/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2021-03-12  1.0  xiebin First version for Device Driver Library of Module.
 **
 ******************************************************************************/
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
#include "cw32l011_vc.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint16_t  gFlagIrq = FALSE;
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
void VC_PortInit(void);
void LED_Init(void);
/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 **
 **
 ******************************************************************************/
int32_t main(void)
{
    VC_InitTypeDef VC_InitStruct;
    VC_BlankTypeDef VC_BlankStruct;

    //配置测试IO口
    VC_PortInit();
    //VC通道初始化
    VC_InitStruct.VC_InputP = VC_InputP_Ch0;
    VC_InitStruct.VC_InputN = VC_InputN_Bgr1P2;
    VC_InitStruct.VC_Hys = VC_Hys_25mV;
    VC_InitStruct.VC_Resp = VC_Resp_High;
    VC_InitStruct.VC_FilterClk = VC_FltClk_PCLK;
    VC_InitStruct.VC_FilterTime = VC_FilterTime_4N6;
    VC_InitStruct.VC_Window = VC_Window_Disable;
    VC_InitStruct.VC_Polarity = VC_Polarity_High;
    VC1_ChannelInit(&VC_InitStruct);
    VC1VC2_INNInit(&VC_InitStruct);//负端输入信号初始化;
    //VC Blank窗口初始化
    VC1VC2_BlankInit(&VC_BlankStruct);
    VC_BlankStruct.VC_BlankLevel = VC_BlankLevel_high ;
    VC_BlankStruct.VC_BlankTime = VC_BlankTime_512Clk ;
    VC_BlankStruct.VC_BlankCh5 = VC_BlankCh4_Enable ;
    VC1_BlankCfg(&VC_BlankStruct);
    //VC中断设置
    VC1_ITConfig(VC_IT_HIGH, ENABLE);
    VC1_EnableIrq(VC_INT_PRIORITY);
    VC1_ClearIrq();
    VC1_EnableChannel();
    while (1)
    {
        if (gFlagIrq)
        {
            gFlagIrq = FALSE;
            FirmwareDelay(500000);
            VC_EnableNvic(VC1_IRQn, VC_INT_PRIORITY);
        }
    }
}

/**
 * @brief VC I/O初始化
 *
 */
void VC_PortInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    //打开VC时钟
    __SYSCTRL_VC_CLK_ENABLE();
    //set PA11 as output
    GPIO_InitStruct.Pins = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
    //set PA11 as VC output
    PA11_AFx_VC1OUT();

    //set PA00 as VC1_CH0 P-INPUT
    PA00_ANALOG_ENABLE();
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
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif
