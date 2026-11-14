/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2021-04-29
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
#include "main.h"

uint8_t DeviceAddr = 0xB8;
uint8_t TxBuf[9];
uint8_t RxBuf[8];
uint8_t DataAddress = 0x00;


void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
static void I2C_Init(void);
static void VC1_2_Init(void);

int32_t main(void)
{
    uint16_t i;
    
    //时钟初始化
    RCC_Configuration();
    
    // 初始化VC
    VC1_2_Init();
    
    // 初始化I2C
    I2C_Init();
    
    
    {
        // 以下准备待写入的数据
        // 按24C02 8个字节一页写入
        TxBuf[0] = 0x00;    // 写入的地址
        for (i = 1; i < sizeof(TxBuf); i++)
        {
            TxBuf[i] = i;
        }
        
        // 以下为写入数据流程
        // 向地址为0xA0的从机写入数据
        if (I2C_NO_ERROR !=I2C_MasterSendDataToSlave( 0xA0, TxBuf, sizeof(TxBuf)))
        {
            while(1);
        }
        FirmwareDelay(30000);    // 约10ms延时
        
        // 以下为读出数据流程
        if (I2C_NO_ERROR != I2C_MasterSendDataToSlave( 0xA0, &DataAddress, 1))    // 将数据地址设置到DataAddress指定的位置
        {
            while(1);
        }
        if (I2C_NO_ERROR != I2C_MasterRecDataFromSlave(0xA0, RxBuf, sizeof(RxBuf))) // 读取数据
        {
            while(1);
        } 
        
    }
    while(1);
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV2);    // 初始化系统时钟为24MHz
}

/*******************************************************************************
 * @brief  VC1_2 初始化
 * @param  None
 * @retval None
 */
static void VC1_2_Init(void)
{
    VC_InitTypeDef   VC_InitStruct = {0};
    VC_DivTypeDef    VC_DivStruct ={0};
    
    __SYSCTRL_GPIOA_CLK_ENABLE();
    // PA00,PA01配置为模拟输入，作为VC的收入，完成电平转换。
    PA00_ANALOG_ENABLE();   
    PA01_ANALOG_ENABLE();
    
        
   
    
    // 使能外设时钟
    __SYSCTRL_VC_CLK_ENABLE();
    
    // 设置参考源
    VC_DivStruct.VC_DivVref = VC_DivVref_VCAP;    // VCAP 内核电压约1.6V
    VC_DivStruct.VC_DivEn = VC_Div_Enable;        // 使用内部电阻网络分压
    VC_DivStruct.VC_DivValue = 3;                 // VC_REF = Vcore *(1+3)/8 = 1.6/2= 0.8V;
    VC1VC2_DIVInit(&VC_DivStruct);
    
    // 设置VC参数
    VC_InitStruct.VC_FilterClk = VC_FltClk_PCLK;    // 滤波时钟为PCLK
    VC_InitStruct.VC_FilterTime = VC_FilterTime_2N8; // 滤波参数 = CLK/2 N8 -- 0.75us
    VC_InitStruct.VC_Hys = VC_Hys_25mV;     // VC迟滞   = 25mV
    VC_InitStruct.VC_InputN = VC_InputN_DivOut; // VC负端   = VC参考源
    VC_InitStruct.VC_InputP = VC_InputP_Ch0;    // VC1正端  = IN0 (PA00)/VC2正端 = IN0 (PA01)
    VC_InitStruct.VC_Polarity = VC_Polarity_High;    // 正端电压大于负端，输出高
    VC_InitStruct.VC_Resp = VC_Resp_High;    // 响应速度 = 快速 100ns
    VC_InitStruct.VC_Window = VC_Window_Disable;     // 禁止窗口功能
    
    VC1_ChannelInit(&VC_InitStruct);
    VC2_ChannelInit(&VC_InitStruct); 
    
    VC1_EnableChannel();
    VC2_EnableChannel();
    
}

/*******************************************************************************
 * @brief  I2C 初始化
 * @param  None
 * @retval None
 */
static void I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    I2C_InitTypeDef  I2C_InitStruct  = {0};

    // 使能外设时钟
    __SYSCTRL_I2C_CLK_ENABLE();    

    // 配置I2C
    I2C_InitStruct.PCLK_Freq = 24000000;    // PCLK = 24MHz
    I2C_InitStruct.I2C_SCL_Freq = 100000;   // SCL = 100KHz
    I2C_InitStruct.I2C_SCL_Source = I2C_SCL_SRC_VC2_OUT;    // SCLINSRC = VC2_OUT
    I2C_InitStruct.I2C_SDA_Source = I2C_SDA_SRC_VC1_OUT;    // SDAINSRC = VC1_OUT
    
    I2C_Master_Init( &I2C_InitStruct);  
    I2C_Cmd( ENABLE);    
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

