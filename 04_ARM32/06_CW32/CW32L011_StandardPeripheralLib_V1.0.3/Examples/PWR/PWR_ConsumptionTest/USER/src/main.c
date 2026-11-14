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

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IWDT_RUNNING              1UL
#define RTC_RUNNING               (1UL << 1)
#define LPTIM_RUNNING             (1UL << 2)
#define UART1_RUNNING             (1UL << 3)

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

volatile uint8_t gKeyStatus = 0;  /* set to 1 after User Button interrupt  */
volatile uint8_t gLPTIMWakeUpIrq = 0;  /* set to 1 after LPTIM interrupt  */
volatile uint8_t gIWDTWakeUpIrq = 0;  /* set to 1 after IWDT alarm interrupt    */
volatile uint8_t gRTCWakeUpIrq = 0;  /* set to 1 after RTC alarm interrupt    */
volatile uint8_t gUARTWakeUpIrq = 0;  /* set to 1 after UART alarm interrupt    */
volatile uint8_t gRecvChar = 0;



/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void DebugUART_Close(void);
void RTC_Configuration(void);
void LPTIM_Configuration(void);
void UART1_Configuration(void);

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
    uint16_t cnt;
    static uint8_t TestState = 0;
    static uint32_t IsPeriphRun = 0;
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    SYSCTRL_MCO_OUT(SYSCTRL_MCO_SRC_HCLK, SYSCTRL_MCO_DIV1);    // PA04输出内核工作频率
    Bsp_Init(4000000);     // 默认上电以4MHz运行，初始化板上的外设
    Bsp_Uart_Init(4000000, 9600);    //改波特率为9600
    printf("\r\nCW32L011 PWR TEST Example\r\n");

    // 96MHz
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI96MHZ);
    Bsp_Uart_Init(96000000, 9600);    // 配置打印串口
    printf("SysClk=HCLK=PCLK=96MHz\r\n");
    // 提供等待时间用于测量在96MHz时，MCU内核功耗8.80mA@3.3V
    cnt = 500;
    while (cnt)   // 按键消抖
    {
        if (!Bsp_Read_Key(&KEY1))    // 按下
        {
            cnt--;
        }
        else
        {
            cnt = 500;
        }
    }
    while (!Bsp_Read_Key(&KEY1)); // 等待释放

    // 48MHz
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI48MHZ);
    Bsp_Uart_Init(48000000, 9600);
    printf("SysClk=HCLK=PCLK=48MHz\r\n");
    // 提供时间用于测量在48MHz时，MCU内核功耗5.05mA@3.3V
    cnt = 500;
    while (cnt)   // 按键消抖
    {
        if (!Bsp_Read_Key(&KEY1))    // 按下
        {
            cnt--;
        }
        else
        {
            cnt = 500;
        }
    }
    while (!Bsp_Read_Key(&KEY1)); // 等待释放

    // 32MHz
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI32MHZ);
    Bsp_Uart_Init(32000000, 9600);
    printf("SysClk=HCLK=PCLK=32MHz\r\n");
    // 提供时间用于测量在32MHz时，MCU内核功耗3.50mA@3.3V
    cnt = 500;
    while (cnt)   // 按键消抖
    {
        if (!Bsp_Read_Key(&KEY1))    // 按下
        {
            cnt--;
        }
        else
        {
            cnt = 500;
        }
    }
    while (!Bsp_Read_Key(&KEY1)); // 等待释放

    // 16MHz
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI16MHZ);
    Bsp_Uart_Init(16000000, 9600);
    printf("SysClk=HCLK=PCLK=16MHz\r\n");
    // 提供时间用于测量在16MHz时，MCU内核功耗2.86mA@3.3V
    cnt = 500;
    while (cnt)   // 按键消抖
    {
        if (!Bsp_Read_Key(&KEY1))    // 按下
        {
            cnt--;
        }
        else
        {
            cnt = 500;
        }
    }
    while (!Bsp_Read_Key(&KEY1)); // 等待释放

    // SYSCLK = 8Hz
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI8MHZ);
    Bsp_Uart_Init(8000000, 9600);
    printf("SysClk=HCLK=PCLK=8MHz\r\n");
    // 提供时间用于测量在8MHz时，MCU内核功耗1.85mA@3.3V
    cnt = 500;
    while (cnt)   // 按键消抖
    {
        if (!Bsp_Read_Key(&KEY1))    // 按下
        {
            cnt--;
        }
        else
        {
            cnt = 500;
        }
    }
    while (!Bsp_Read_Key(&KEY1)); // 等待释放

    // SYSCLK = 4MHz
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI4MHZ);
    Bsp_Uart_Init(4000000, 9600);
    printf("SysClk=HCLK=PCLK=4MHz\r\n");
    // 提供时间用于测量在4MHz时，MCU内核功耗1.47mA@3.3V
    cnt = 500;
    while (cnt)   // 按键消抖
    {
        if (!Bsp_Read_Key(&KEY1))    // 按下
        {
            cnt--;
        }
        else
        {
            cnt = 500;
        }
    }
    while (!Bsp_Read_Key(&KEY1)); // 等待释放

    //deepsleep模式下，测量部分外设的功耗

    while (1)
    {
        switch (TestState)
        {
        default:
        case 0:    // BSP_KEY2_PORT 打开 整体功耗440nA
            // 配置KEY2中断唤醒
            GPIO_InitStructure.IT = GPIO_IT_FALLING;
            GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
            GPIO_InitStructure.Pins = BSP_KEY2_PIN;
            GPIO_Init(BSP_KEY2_PORT, &GPIO_InitStructure);

            NVIC_ClearPendingIRQ(BSP_KEY2_IRQn);
            NVIC_EnableIRQ(BSP_KEY2_IRQn);

            printf("KEY2 interrupt can wake up.\r\n");
            Bsp_Uart_Closed();

            // 关所有外设
            CW_SYSCTRL->APBEN1 = 0x5a5a0000;
            CW_SYSCTRL->APBEN2 = 0x5a5a0000;
            CW_SYSCTRL->AHBEN  = 0x5a5a0000;
            // 仅开KEY2的端口，保证KEY2中断可用
            BSP_KEY2_PERIPH_CLK_ENABLE();
            break;
        case 1:    // BSP_KEY2_PORT + IWDT (IWDT会打开LSI) 功耗1.07uA
            if (!(IsPeriphRun & IWDT_RUNNING))
            {
                IWDT_SetPeriod(IWDT_4_SECS);
                IsPeriphRun |= IWDT_RUNNING;
                printf("IWDT is running now!\r\n");
                Bsp_Uart_Closed();
            }
            break;
        case 2:    // BSP_KEY2_PORT + LPTIM@LSI 功耗1.13uA
            if (!(IsPeriphRun & LPTIM_RUNNING))
            {
                // 先关闭IWDT
                IWDT_Stop();
                SYSCTRL_APBPeriphReset2(SYSCTRL_APB2_PERIPH_IWDT, ENABLE);
                SYSCTRL_APBPeriphReset2(SYSCTRL_APB2_PERIPH_IWDT, DISABLE);
                SYSCTRL_APBPeriphClk_Enable2(SYSCTRL_APB2_PERIPH_IWDT, DISABLE);
                IsPeriphRun &= ~IWDT_RUNNING;

                // 开启LPTIM
                LPTIM_Configuration();
                IsPeriphRun |= LPTIM_RUNNING;
                printf("LPTIM is running now!\r\n");
                Bsp_Uart_Closed();
            }
            else
            {
                Bsp_Uart_Closed();
            }
            break;
        case 3:    // BSP_KEY2_PORT + UART1@LSI 功耗1.19uA
            if (!(IsPeriphRun & UART1_RUNNING))
            {
                // 先关闭LPTIM
                LPTIM_Cmd(DISABLE);
                SYSCTRL_APBPeriphClk_Enable2(SYSCTRL_APB2_PERIPH_LPTIM, DISABLE);
                IsPeriphRun &= ~LPTIM_RUNNING;

                UART1_Configuration();    // UART1 即BSP调试串口，需要重新配置，实现深度休眠下的唤醒
                IsPeriphRun |= UART1_RUNNING;
                printf("UART1 is running now!\r\n");
            }
            else
            {
                UART1_Configuration();
            }
            break;
        case 4:    // BSP_KEY2_PORT + RTC@LSI   功耗1.08uA    RTC的初始化限制只能在上电复位时可配置，故不关闭RTC，进入下一个case。
            if (!(IsPeriphRun & RTC_RUNNING))
            {
                // 先关闭UART1
                SYSCTRL_APBPeriphReset1(SYSCTRL_APB1_PERIPH_UART1, ENABLE);
                SYSCTRL_APBPeriphReset1(SYSCTRL_APB1_PERIPH_UART1, DISABLE);
                SYSCTRL_APBPeriphClk_Enable1(SYSCTRL_APB1_PERIPH_UART1, DISABLE);
                IsPeriphRun &= ~UART1_RUNNING;

                // 开启RTC
                RTC_Configuration();
                IsPeriphRun |= RTC_RUNNING;
                printf("RTC is running now!\r\n");
                Bsp_Uart_Closed();
            }
            else
            {
                Bsp_Uart_Closed();
            }
            break;

        case 5:   // GPIOA + IWDT + RTC + LPTIM + UART +LSI 功耗1.3uA
            if (!(IsPeriphRun & IWDT_RUNNING))
            {
                IWDT_SetPeriod(IWDT_4_SECS);
                IsPeriphRun |= IWDT_RUNNING;
                printf("IWDT is running now!\r\n");
            }
            if (!(IsPeriphRun & RTC_RUNNING))
            {
                // 开启RTC
                RTC_Configuration();
                IsPeriphRun |= RTC_RUNNING;
                printf("RTC is running now!\r\n");
            }
            if (!(IsPeriphRun & LPTIM_RUNNING))
            {
                // 开启LPTIM
                LPTIM_Configuration();

                IsPeriphRun |= LPTIM_RUNNING;
                printf("LPTIM is running now!\r\n");
            }
            if (!(IsPeriphRun & UART1_RUNNING))
            {
                // 开启UART1
                UART1_Configuration();
                IsPeriphRun |= UART1_RUNNING;
                printf("UART1 is running now!\r\n");
            }
            else
            {
                UART1_Configuration();
            }
            IWDT_Refresh();
            break;
        case 6:
            TestState = 0;
            IsPeriphRun = 0;
            while (1);   //等待4s后 IWDT复位
            break;
        }

        BSP_KEY2_PERIPH_CLK_ENABLE();

        //进入deepsleep
        SYSCTRL_GotoDeepSleep();

        //唤醒后配置串口，打印信息
        Bsp_Uart_Init(4000000, 9600);

        if (gRTCWakeUpIrq)
        {
            gRTCWakeUpIrq = 0;
            printf("MCU is waked up by RTC!\r\n");
        }
        if (gLPTIMWakeUpIrq)
        {
            gLPTIMWakeUpIrq = 0;
            printf("MCU is waked up by LPTIM!\r\n");
        }
        if (gUARTWakeUpIrq)
        {
            gUARTWakeUpIrq = 0;
            printf("MCU is waked up by UART!\r\n");
            printf("UART received char: %c\r\n", gRecvChar);
        }

        if (gKeyStatus)
        {
            gKeyStatus = 0;
            printf("MCU is waked up by KEY2!\r\n");
            while (!Bsp_Read_Key(&KEY2)); // 等待释放
            TestState++;     //切换下一个测试状态
        }
    }
}

void RTC_Configuration(void)
{
    RTC_InitTypeDef RTC_InitStruct = {0};

    RTC_InitStruct.DateStruct.Day = 1;
    RTC_InitStruct.DateStruct.Month = RTC_Month_January;
    RTC_InitStruct.DateStruct.Year = 24;
    RTC_InitStruct.DateStruct.Week = RTC_Weekday_Sunday;
    RTC_InitStruct.RTC_ClockSource = RTC_RTCCLK_FROM_LSI;
    RTC_InitStruct.TimeStruct.AMPM = RTC_H12_AM;
    RTC_InitStruct.TimeStruct.H24 = RTC_HOUR12;
    RTC_InitStruct.TimeStruct.Hour = 1;
    RTC_InitStruct.TimeStruct.Minute = 0;
    RTC_InitStruct.TimeStruct.Second = 0;

    SYSCTRL_LSI_Enable();    // 使用LSI作为RTC的时钟源，必须在配置RTC前准备好时钟
    RTC_Init(&RTC_InitStruct);
    RTC_SetInterval(RTC_INTERVAL_EVERY_1S);    // 1s间隔一次产生中断
    RTC_ITConfig(RTC_IT_INTERVAL, ENABLE);
    RTC_ClearITPendingBit(RTC_IT_ALL);

    NVIC_EnableIRQ(RTC_IRQn);
    RTC_Cmd(ENABLE);
}


void LPTIM_Configuration(void)
{
    LPTIM_InitTypeDef LPTIM_InitStruct = {0};

    SYSCTRL_LSI_Enable();    // 使用LSI作为时钟源

    LPTIM_InitStruct.LPTIM_ClockSource = LPTIM_CLOCK_SOURCE_MCLK;
    LPTIM_InitStruct.LPTIM_CounterMode = LPTIM_COUNTER_MODE_TIME;
    LPTIM_InitStruct.LPTIM_Period = 32800 - 1;
    LPTIM_InitStruct.LPTIM_Prescaler = LPTIM_PRS_DIV2;
    LPTIM_Init(&LPTIM_InitStruct);

    LPTIM_InternalClockConfig(LPTIM_ICLK_LSI);

    LPTIM_ITConfig(LPTIM_IT_ARRM, ENABLE);
    CW_LPTIM->ICR = 0x00;
    NVIC_EnableIRQ(LPTIM_IRQn);

    LPTIM_Cmd(ENABLE);
    LPTIM_SelectOnePulseMode(LPTIM_OPERATION_REPETITIVE);
}


void UART1_Configuration(void)
{
    UART_InitTypeDef UART_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pins = GPIO_PIN_2;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = GPIO_PIN_3;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    PA02_AFx_UART1TXD();
    PA03_AFx_UART1RXD();

    UART_InitStructure.UART_BaudRate = 9600;        // 休眠模式下可以使用的波特率
    UART_InitStructure.UART_Over = UART_Over_sp;    // 使用LSI必须实现特殊采样率
    UART_InitStructure.UART_Source = UART_Source_LSI;
    UART_InitStructure.UART_UclkFreq = 32800;
    UART_InitStructure.UART_StartBit = UART_StartBit_FE;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(CW_UART1, &UART_InitStructure);

    UART_ITConfig(CW_UART1, UART_IT_RC, ENABLE);
    UART_ClearFlag(CW_UART1, UART_IT_RC);
    NVIC_EnableIRQ(UART1_IRQn);
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

