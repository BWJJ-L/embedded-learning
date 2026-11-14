/**
 * @file    main.c
 * @brief   该文件是程序的入口点，包含主函数`main`以及一些辅助函数的实现
 * @author  
 * @date    
 * @version 
 *
 * 代码许可和免责信息
 * 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由
 * 此生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯
 * 源半导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明
 * 示或暗含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵
 * 权的保证或条件。
 * 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
 * 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带
 * 的或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金
 * 额的损失。
 * 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些
 * 或全部上述排除或限制可能并不适用于您。
 **/
 
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "..\inc\main.h"
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


/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

// 主函数
int main(void)
{
    Bsp_Init(4000000);
    
    Bsp_Led_On(&LED1);
    Bsp_Led_On(&LED2);
    printf("Welcome to use CW32L011.\n");
    
    while(1)
    {
        if (Bsp_Read_Key(&KEY1))
        {
            Bsp_Led_On(&LED1);
        }
        else
        {
            Bsp_Led_Off(&LED1);
        }
        
        if (Bsp_Read_Key(&KEY2))
        {
            Bsp_Led_On(&LED2);
        }
        else
        {
            Bsp_Led_Off(&LED2);
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

