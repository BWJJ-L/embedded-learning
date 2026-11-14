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

#define I2C_SLAVEADDRESS          0x3C
#define I2C_SLAVEADDRESS_OTHER1   0xAA
#define I2C_SLAVEADDRESS_OTHER2   0x96

#define  I2C_SCL_GPIO_PORT       CW_GPIOA
#define  I2C_SCL_GPIO_PIN        GPIO_PIN_9    //如果改动口线则GPIO初始化代码需要做同步修改
#define  I2C_SDA_GPIO_PORT       CW_GPIOA
#define  I2C_SDA_GPIO_PIN        GPIO_PIN_10   //如果改动口线则GPIO初始化代码需要做同步修改


uint8_t u8Senddata[16] = {0x55};
uint8_t u8Recdata[16] = {0x00};
uint8_t u8SendLen = 16;
uint32_t u32RecvLen = 16;

void SYSCTRL_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

void I2C_Configuration(void);
int32_t main(void)
{
    //时钟初始化
    SYSCTRL_Configuration();

    //IO口初始化
    GPIO_Configuration();

    I2C_Configuration();

    while (1)
    {
        I2C_SlaveRecDataFromMaster(u8Recdata, &u32RecvLen); //等主机写数据,MCU作为从机接收主机发送的数据
        I2C_SlaveSendDataToMaster(u8Recdata, u8SendLen); //等待主机寻址，MCU作为从机给发送数据
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
    GPIO_InitTypeDef GPIO_InitStructure;

    PA10_AFx_I2CSDA();
    PA09_AFx_I2CSCL();
    GPIO_InitStructure.Pins = I2C_SCL_GPIO_PIN | I2C_SDA_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init(I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
}

void I2C_Configuration(void)
{
    I2C_InitTypeDef I2C_InitStruct;

    //I2C初始化
    I2C_InitStruct.PCLK_Freq = 16000000;
    I2C_InitStruct.I2C_SCL_Freq = 1000000;
    I2C_InitStruct.I2C_OwnSlaveAddr0 = I2C_SLAVEADDRESS;         //TEST ADDRESS0
    I2C_InitStruct.I2C_OwnSlaveAddr1 = I2C_SLAVEADDRESS_OTHER1;  //TEST ADDRESS1
    I2C_InitStruct.I2C_OwnSlaveAddr2 = I2C_SLAVEADDRESS_OTHER2;  //TEST ADDRESS2

    I2C_InitStruct.I2C_OwnGc = DISABLE;

    I2C_DeInit();
    I2C_Slave_Init(&I2C_InitStruct);//初始化模块
    CW_I2C->CR_f.FLT = 1;
    I2C_Cmd( ENABLE);//模块使能
    I2C_AcknowledgeConfig(ENABLE);//ACK打开
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
//    __disable_irq();
//    NVIC_EnableIRQ(I2C_IRQn);
//    __enable_irq();
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

