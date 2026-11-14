#include "board.h"

#define BOARD_HCLKFREQ	8000000		// 系统时钟频率

/******************************************************************
 * 函 数 名 称：board_init
 * 函 数 说 明：初始化开发板
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void board_init(void)
{
	// 时钟HSIOSC
	// 系统时钟设置为HSIOSC时钟12分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
    //SYSCLK = HSI = 8MHz = HCLK = PCLK
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12);
	
	//使用sysTick作为1ms
    InitTick(BOARD_HCLKFREQ);

}

/******************************************************************
 * 函 数 名 称：delay_us
 * 函 数 说 明：利用滴答定时器延时多少us
 * 函 数 形 参：__us：延时的时长（单位us）
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void delay_us(unsigned long __us) 
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 38;

    // 计算需要的时钟数 = 延迟微秒数 * 每微秒的时钟数
    ticks = __us * (BOARD_HCLKFREQ / 1000000);

    // 获取当前的SysTick值
    told = SysTick->VAL;

    while (1)
    {
        // 重复刷新获取当前的SysTick值
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += SysTick->LOAD - tnow + told;

            told = tnow;

            // 如果达到了需要的时钟数，就退出循环
            if (tcnt >= ticks)
                break;
        }
    }
}
//搭配滴答定时器实现的精确ms延时
void delay_ms(unsigned long ms) 
{
	delay_us( ms * 1000 );
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
