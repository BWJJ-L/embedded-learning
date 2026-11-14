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
#include "cw32l011_sysctrl.h"
#include "cw32l011_gpio.h"

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
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample toggle MCO
 **
 ******************************************************************************/
int32_t main(void)
{
    uint32_t cnt;
    SYSCTRL_HSI_Enable( HSIOSC_TO_HSI24MHZ );   //24M
    __SYSCTRL_GPIOA_CLK_ENABLE();
    PA15_DIGTAL_ENABLE();
    PA15_DIR_INPUT();
    
    while(1)
    {    
    
        /********************************************************************
        1. MCO toggle HCLK Test. MCO_CLK = HCLK
         *******************************************************************/   
        SYSCTRL_MCO_OUT( SYSCTRL_MCO_SRC_HCLK, SYSCTRL_MCO_DIV1 );
        cnt = 500;
        while(cnt)    // 按键消抖
        {
            if (!PA15_GETVALUE())
            {
                cnt--;
            }
            else 
            {
                cnt = 500;
            }                
        }
        while(!PA15_GETVALUE());
        
        
        /********************************************************************
        2. MCO toggle PCLK Test. MCO_CLK = PCLK / 2
         *******************************************************************/
        SYSCTRL_MCO_OUT( SYSCTRL_MCO_SRC_PCLK, SYSCTRL_MCO_DIV2 );
        cnt = 500;
        while(cnt)
        {
            if (!PA15_GETVALUE())
            {
                cnt--;
            }
            else 
            {
                cnt = 500;
            }                
        }
        while(!PA15_GETVALUE());


        /********************************************************************
        3. MCO toggle HSI Test.  MCO_CLK = HSIOSC / 8
         *******************************************************************/
        SYSCTRL_MCO_OUT( SYSCTRL_MCO_SRC_HSI , SYSCTRL_MCO_DIV8 );
        cnt = 500;
        while(cnt)
        {
            if (!PA15_GETVALUE())
            {
                cnt--;
            }
            else 
            {
                cnt = 500;
            }                
        }
        while(!PA15_GETVALUE());


        /********************************************************************
        4. MCO toggle LSI Test.  MCO_CLK = LSI / 64
         *******************************************************************/
        SYSCTRL_LSI_Enable();
        SYSCTRL_MCO_OUT( SYSCTRL_MCO_SRC_LSI , SYSCTRL_MCO_DIV64 );
        cnt = 500;
        while(cnt)    
        {
            if (!PA15_GETVALUE())
            {
                cnt--;
            }
            else 
            {
                cnt = 500;
            }                
        }
        while(!PA15_GETVALUE());

        /********************************************************************
        5. MCO toggle HSE Test. MCO_CLK = HSE / 128
         *******************************************************************/
        SYSCTRL_HSE_Enable( SYSCTRL_HSE_MODE_OSC, 16000000, SYSCTRL_HSE_DRIVER_LEVEL0, SYSCTRL_HSE_FLT_CLOSE);
        SYSCTRL_MCO_OUT( SYSCTRL_MCO_SRC_HSE , SYSCTRL_MCO_DIV128 );
        cnt = 500;
        while(cnt)
        {
            if (!PA15_GETVALUE())
            {
                cnt--;
            }
            else 
            {
                cnt = 500;
            }                
        }
        while(!PA15_GETVALUE());

        /********************************************************************
        6. MCO toggle LSE Test. MCO_CLK = LSE / 256
         *******************************************************************/
        SYSCTRL_LSE_Enable(SYSCTRL_LSE_MODE_OSC ,SYSCTRL_LSE_DRIVER_LEVEL2);
        SYSCTRL_MCO_OUT( SYSCTRL_MCO_SRC_LSE , SYSCTRL_MCO_DIV256 );
        cnt = 500;
        while(cnt)
        {
            if (!PA15_GETVALUE())
            {
                cnt--;
            }
            else 
            {
                cnt = 500;
            }                
        }
        while(!PA15_GETVALUE());
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

