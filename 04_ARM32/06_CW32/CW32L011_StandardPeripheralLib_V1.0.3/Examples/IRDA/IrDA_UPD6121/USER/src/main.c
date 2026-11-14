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
#include "../inc/main.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define NEC_LEAD_HIGH_TIME 9000  // 引导码高电平时间（单位：us）
#define NEC_LEAD_LOW_TIME 4500   // 引导码低电平时间（单位：us）
#define NEC_BIT_HIGH_TIME 560    // 每个比特的高电平时间（单位：us）
#define NEC_BIT_LOW_TIME_0 560   // 逻辑0的低电平时间（单位：us）
#define NEC_BIT_LOW_TIME_1 1680  // 逻辑1的低电平时间（单位：us）
#define NEC_CARRIER_FREQ 38000   // 载波频率（单位：Hz）
 
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void SYSCTRL_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

static void GTIM1_Config(void);
static void GTIM2_Config(void);

void IrDA_Send(const uint8_t *pData, uint16_t len);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

uint32_t DelayTime;
static uint8_t DataBuffer[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief
 * @return int32_t
 */
int32_t main(void)
{
    /* System Clocks Configuration */
    SYSCTRL_Configuration();      // HSI时钟源，PCLK=HCLK=48MHz
  
    /* GPIO Configuration */
    GPIO_Configuration();

    GTIM1_Config();
    GTIM2_Config();

    CW_IRMOD->CR_f.MOD = 0;  // 选择模式： GTIM1_CH1 & GTIM2_CH1 & IRSW

    /* NVIC Configuration */
    NVIC_Configuration();
    
    while(1)
    {        
        IrDA_Send(DataBuffer, 10);
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void SYSCTRL_Configuration(void)
{    
    __SYSCTRL_FLASH_CLK_ENABLE();
    FLASH_SetLatency(FLASH_Latency_2);
   
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI48MHZ);

    /* 1. 设置HCLK和PCLK的分频系数　*/
    SYSCTRL_HCLKPRS_Config(SYSCTRL_HCLK_DIV1);
    SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV1);
   
    SYSCTRL_SystemCoreClockUpdate(48000000);
}

/**
  * @brief  Configure the GPIO Pins.
  *  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __SYSCTRL_GPIOA_CLK_ENABLE();
    
    PA08_AFx_IROUT();
    PA06_AFx_GTIM1CH1();
    PA00_AFx_GTIM2CH1();    

    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pins = GPIO_PIN_8 | GPIO_PIN_6 | GPIO_PIN_0;

    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
}


/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();

    __enable_irq();
}

/**
 * @brief GTIM1设置为PWM1，产生38KHz的载波
 *
 */
static void GTIM1_Config(void)
{
    GTIM_InitTypeDef GTIM_InitStruct = {0};
    GTIM_OCModeCfgTypeDef GTIM_OCModeCfgStruct;

    __SYSCTRL_GTIM1_CLK_ENABLE();
    
    GTIM_InitStruct.AlignMode     = GTIM_ALIGN_MODE_EDGE;
    GTIM_InitStruct.ARRBuffState  = GTIM_ARR_BUFF_DIS;
    GTIM_InitStruct.Direction     = GTIM_DIRECTION_UP;
    GTIM_InitStruct.EventOption   = GTIM_EVENT_NORMAL;
    GTIM_InitStruct.Prescaler     = 2;    // 48MHz/(Prescaler+1) = 16MHz
    GTIM_InitStruct.PulseMode     = GTIM_PULSE_MODE_DIS;
    GTIM_InitStruct.ReloadValue   = 420;    // 38kHz  ReloadValue = 16000/38 -1
    GTIM_InitStruct.UpdateOption  = GTIM_UPDATE_DIS;
    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);
    
    GTIM_OCModeCfgStruct.FastMode     = ENABLE;
    GTIM_OCModeCfgStruct.OCMode       = GTIM_OC_MODE_PWM1;
    GTIM_OCModeCfgStruct.OCPolarity   = GTIM_OC_POLAR_NONINVERT;
    GTIM_OCModeCfgStruct.PreloadState = ENABLE;
    GTIM_OC1ModeCfg(CW_GTIM1, &GTIM_OCModeCfgStruct);
    
    GTIM_SetCompare1(CW_GTIM1, (GTIM_InitStruct.ReloadValue / 16) * 3);
    GTIM_OC1Cmd(CW_GTIM1, ENABLE);
}

/**
 * @brief GTIM2设置为PWM2，控制高低电平的时常。
 *
 */
static void GTIM2_Config(void)
{
    GTIM_InitTypeDef GTIM_InitStruct = {0};
    GTIM_OCModeCfgTypeDef GTIM_OCModeCfgStruct;

    __SYSCTRL_GTIM2_CLK_ENABLE();
    
    GTIM_InitStruct.AlignMode     = GTIM_ALIGN_MODE_EDGE;
    GTIM_InitStruct.ARRBuffState  = GTIM_ARR_BUFF_DIS;
    GTIM_InitStruct.Direction     = GTIM_DIRECTION_UP;
    GTIM_InitStruct.EventOption   = GTIM_EVENT_NORMAL;
    GTIM_InitStruct.Prescaler     = 47;    // 48MHz/(Prescaler+1) = 1MHz
    GTIM_InitStruct.PulseMode     = GTIM_PULSE_MODE_EN;    // 单次模式
    GTIM_InitStruct.ReloadValue   = NEC_LEAD_HIGH_TIME + NEC_LEAD_LOW_TIME ;    //  实际工作时会动态修改
    GTIM_InitStruct.UpdateOption  = GTIM_UPDATE_EN;
    GTIM_TimeBaseInit(CW_GTIM2, &GTIM_InitStruct);
    
    GTIM_OCModeCfgStruct.FastMode     = DISABLE;
    GTIM_OCModeCfgStruct.OCMode       = GTIM_OC_MODE_PWM1;
    GTIM_OCModeCfgStruct.OCPolarity   = GTIM_OC_POLAR_NONINVERT;
    GTIM_OCModeCfgStruct.PreloadState = DISABLE;
    GTIM_OC1ModeCfg(CW_GTIM2, &GTIM_OCModeCfgStruct);    
    
    GTIM_SetCompare1(CW_GTIM2, NEC_LEAD_HIGH_TIME);
    GTIM_OC1Cmd(CW_GTIM2, ENABLE);
    GTIM_Cmd(CW_GTIM2, ENABLE);
}



void IrDA_Send(const uint8_t *pData, uint16_t len)
{
    uint8_t i;
    
    CW_IRMOD->CR_f.IRSW = 1;    // 软件开关打开

    // 发送引导码
    CW_GTIM2->CNT = 0;
    CW_GTIM2->CCR1 = NEC_LEAD_HIGH_TIME;
    CW_GTIM2->ARR = NEC_LEAD_HIGH_TIME + NEC_LEAD_LOW_TIME;
        
    CW_GTIM2->ICR = 0x0000;
    
    GTIM_Cmd(CW_GTIM1, ENABLE);
    GTIM_Cmd(CW_GTIM2, ENABLE);
    while(!CW_GTIM2->ISR_f.UIF);  
    
    while (len--)
    {
        for (i = 0; i < 8; i++)
        {
            if (*pData & (1 << i))
            {                
                CW_GTIM2->ICR = 0;               
                CW_GTIM2->CCR1 = NEC_BIT_HIGH_TIME;
                CW_GTIM2->ARR = NEC_BIT_HIGH_TIME+NEC_BIT_LOW_TIME_1;
                CW_GTIM2->CR1_f.CEN = 1;
                while(!CW_GTIM2->ISR_f.UIF);                
            }
            else
            {                
                CW_GTIM2->ICR = 0;                
                CW_GTIM2->CCR1 = NEC_BIT_HIGH_TIME;
                CW_GTIM2->ARR = NEC_BIT_HIGH_TIME+NEC_BIT_LOW_TIME_0;
                CW_GTIM2->CR1_f.CEN = 1;
                while(!CW_GTIM2->ISR_f.UIF);                                 
            }
        }
    }
    
    CW_GTIM1->CR1_f.CEN = 0;
    CW_GTIM2->CR1_f.CEN = 0;
    CW_IRMOD->CR_f.IRSW = 0; 
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

