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
volatile uint32_t ResetFlagRegister;

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

// 函数声明
void InitSystem(void);  // 系统初始化函数
void LVD_Config(void);
void trigger_fault(void);
void MainLoop(void);    // 主循环函数
void HandleTask1(void); // 任务1处理函数
void HandleTask2(void); // 任务2处理函数

// 主函数
int32_t main(void)
{
    uint8_t i;
    ResetFlagRegister = CW_SYSCTRL->RESETFLAG;
    
    SYSCTRL_ClearResetFlags();
    // 系统初始化
    InitSystem();
    
    for (i = 6; i; i--)
    {
        Bsp_Led_Tog(&LED1);
        SysTickDelay(100);
    }
    Bsp_Led_On(&LED1);
    

    if (ResetFlagRegister & RESETFLAG_POR)
    {
        printf("Power-On Reset or Brown-Out Reset occurred.\n");
    }

    if (ResetFlagRegister & RESETFLAG_LVD)
    {
        printf("LVD Reset occurred.\n");
    }

    if (ResetFlagRegister & RESETFLAG_IWDT)
    {
        printf("IWDT Reset occurred.\n");
    }

    if (ResetFlagRegister & RESETFLAG_RSTB)
    {
        printf("NRST Pin Reset occurred.\n");
    }

    if (ResetFlagRegister & RESETFLAG_LOCKUP)
    {
        printf("LOCKUP Fault Reset occurred.\n");
    }    

    if (ResetFlagRegister & RESETFLAG_SYSRESETREQ)
    {
        printf("SYSRESETREQ Soft Reset occurred.\n");
    }

    // 主循环
    while (1)
    {
        MainLoop();
    }
}

// 系统初始化函数
void InitSystem(void)
{
    // 初始化硬件
    SYSCTRL_LOCKUP_Config(SYSCTRL_LOCKUPEN);    // 是能内核LOCKUP复位
    LVD_Config();
            
    Bsp_Init(4000000);
    InitTick(4000000);
   
    IWDT_SetPeriod(IWDT_2_SECS);    
    
    // 初始化外设
    
    // 初始化全局变量
    printf("Welcome to use CW32L011.\n");
}

void LVD_Config(void)
{    
    LVD_InitTypeDef LVD_InitStruct;
    
    LVD_InitStruct.LVD_Action = LVD_Action_Reset;
    LVD_InitStruct.LVD_FilterClk = LVD_FilterClk_SYSCLK;
    LVD_InitStruct.LVD_FilterTime = LVD_FilterTime_1N2;
    LVD_InitStruct.LVD_Source = LVD_Source_VDDA;
    LVD_InitStruct.LVD_Threshold = LVD_Threshold_3p4V;    
    
    LVD_Init(&LVD_InitStruct);
    LVD_TrigConfig(LVD_TRIG_FALL, ENABLE);  
    LVD_Enable();    
}

// 主循环函数
void MainLoop(void)
{
    //
    HandleTask1();

    //
    HandleTask2();
}

// 任务1处理函数
void HandleTask1(void)
{
    if (ResetFlagRegister & RESETFLAG_IWDT)
    {
        IWDT_Refresh();    // 发生看门狗复位后，开始清看门狗，不再产生看门狗复位        
    }    
}

// 任务2处理函数
void HandleTask2(void)
{   
    if(Bsp_Read_Key(&KEY1) == KEY_STATE_PRESSED)
    {
        NVIC_SystemReset();    // 软件复位
    }
    
    if(Bsp_Read_Key(&KEY2) == KEY_STATE_PRESSED)
    {        
        trigger_fault();    // lockup复位
    }
}

void HardFault_Handler(void) 
{
    // 在 HardFault 处理程序中再次触发一个故障，例如访问非法地址将会产生LOCKUP
    int *ptr = (int*)0x12345678;
    *ptr = 1;
}

void trigger_fault(void) 
{
    volatile uint32_t *ptr = (volatile uint32_t *)0x12345678; // 非法地址
    *ptr = 0xAAAAAAAA; // 写入非法地址  进入HardFault
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

