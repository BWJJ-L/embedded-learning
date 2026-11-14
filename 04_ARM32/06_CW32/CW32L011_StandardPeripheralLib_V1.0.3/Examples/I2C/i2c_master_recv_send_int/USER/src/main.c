/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2024-08-07
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

#include "..\inc\main.h"

#define I2C_SLAVEADDRESS        0x3C

#define  I2C_SCL_GPIO_PORT       CW_GPIOA
#define  I2C_SCL_GPIO_PIN        GPIO_PIN_9    //如果改动口线则GPIO初始化代码需要做同步修改
#define  I2C_SDA_GPIO_PORT       CW_GPIOA
#define  I2C_SDA_GPIO_PIN        GPIO_PIN_10   //如果改动口线则GPIO初始化代码需要做同步修改



void SYSCTRL_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void I2C_Configuration(void);
void I2C_IRQHandlerCallback(void);

uint8_t u8Senddata[16] = {0x00};
uint8_t u8Recdata[16] = {0x00};
uint32_t u8SendLen = 16;
uint32_t u8RecvLen = 16;
uint8_t u8TransferMode = 0;


int32_t main(void)
{
    uint8_t i;
    //时钟初始化
    SYSCTRL_Configuration();

    //IO口初始化
    GPIO_Configuration();

    I2C_Configuration();
    NVIC_Configuration();

    while (1)
    {
        // 准备发送的数据
        for (i = 0; i < 16; i++)
        {
            u8Senddata[i] = ~i;
        }
        // 启动发送
        u8TransferMode = 0;    // 0:发送， 1:接收 传入中断
        u8SendLen = 16;     // 发送的数据长度
        I2C_GenerateSTART( ENABLE);   // 启动

        // 等待中断传输完成
        while (u8TransferMode != 3);

        // 接收
        u8TransferMode = 1;
        u8RecvLen = 16;
        I2C_GenerateSTART( ENABLE);   // 启动

        // 等待中断传输完成
        while (u8TransferMode != 3);
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void SYSCTRL_Configuration(void)
{
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI16MHZ);
    __SYSCTRL_GPIOA_CLK_ENABLE();
    __SYSCTRL_I2C_CLK_ENABLE();
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure  = {0};

    PA10_AFx_I2CSDA();
    PA09_AFx_I2CSCL();
    GPIO_InitStructure.Pins = I2C_SCL_GPIO_PIN | I2C_SDA_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init(I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
}

void I2C_Configuration(void)
{

    I2C_InitTypeDef I2C_InitStruct = {0};

    //I2C初始化
    I2C_InitStruct.PCLK_Freq    = 16000000;
    I2C_InitStruct.I2C_SCL_Freq = 1000000;
    I2C_InitStruct.I2C_SCL_Source = I2C_SCL_SRC_GPIO;
    I2C_InitStruct.I2C_SDA_Source = I2C_SDA_SRC_GPIO;

    I2C_DeInit();
    I2C_Master_Init(&I2C_InitStruct);//初始化模块
    I2C_AcknowledgeConfig(ENABLE);//ACK打开
    I2C_Cmd( ENABLE);//模块使能
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(I2C_IRQn);
    __enable_irq();
}

void I2C_IRQHandlerCallback(void)
{
    uint8_t u8State;
    static uint8_t* pBuffPtr;
    static uint32_t XferCount;

    u8State = CW_I2C->STAT;
    if (u8TransferMode == 0)
    {
        switch (u8State)
        {
        case 0x08:    //发送完START信号
        case 0x10:    // 已发送重复起始信号
            pBuffPtr = u8Senddata;
            XferCount = u8SendLen;
            CW_I2C->CR_f.STA = 0;
            CW_I2C->DR = I2C_SLAVEADDRESS & 0xFE;    // 从机地址，按8位格式,最后一位为读写控制
            break;
        case 0x18:   //发送完SLA+W信号,并收到ACK
        case 0x28:   //发送完1字节数据,并收到ACK
            if (XferCount > 0)
            {
                // 数据尚未发送完成
                CW_I2C->DR = *pBuffPtr;
                pBuffPtr++;
                XferCount--;
            }
            else    // 数据已发送完成
            {
                CW_I2C->CR_f.STO = 1;
                CW_I2C->CR_f.SI = 0;
                u8TransferMode = 3;    // 完成
                return;
            }
            break;
        case 0x20:    //发送完SLA+W后从机返回NACK
        case 0x30:    //已发送数据字节，接收 NACK
            CW_I2C->CR_f.STO = 1;
            CW_I2C->CR_f.SI = 0;
            u8TransferMode = 3;    // 完成
            return;
        default:
            break;
        }
        CW_I2C->CR_f.SI = 0;    // 清中断标志
    }
    else if (u8TransferMode == 1)
    {
        switch (u8State)
        {
        case 0x08:   //发送完START信号
        case 0x10:   // 已发送重复起始信号
            pBuffPtr = u8Recdata;
            XferCount = u8RecvLen;
            CW_I2C->CR_f.STA = 0;
            CW_I2C->DR = I2C_SLAVEADDRESS | 0x01;    // 从机地址，按8位格式,最后一位为读写控制
            break;
        case 0x40:   //发送完SLA+R信号，开始接收数据
            CW_I2C->CR_f.AA = 1;
            break;
        case 0x50:   //接收完一字节数据，在接收最后1字节数据之前设置AA=0;
            *pBuffPtr = CW_I2C->DR;
            pBuffPtr++;
            XferCount--;
            if (XferCount == 1)
            {
                CW_I2C->CR_f.AA = 0;
            }
            break;
        case 0x58:   //接收到一个数据字节，且NACK已回应
            *pBuffPtr = CW_I2C->DR;
            XferCount--;
            CW_I2C->CR_f.STO = 1;
            CW_I2C->CR_f.SI = 0;
            u8TransferMode = 3;    // 完成
            return;
        case 0x38:   //主机在发送 SLA+W 阶段或者发送数据阶段丢失仲载  或者  主机在发送 SLA+R 阶段或者回应 NACK 阶段丢失仲裁
        case 0x48:   //发送完SLA+R后从机返回NACK
            CW_I2C->CR_f.STO = 1;
            CW_I2C->CR_f.SI = 0;
            u8TransferMode = 3;    // 完成
            return ;
        default:
            break;
        }
        CW_I2C->CR_f.SI = 0;
    }
    else
    {
        CW_I2C->CR_f.SI = 0;
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
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

