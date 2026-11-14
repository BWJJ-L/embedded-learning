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

#include "..\inc\main.h"

//I2C从设备地址，按照8 BIT设置，最低位0或者1均可。
#define I2C_SLAVEADDRESS 0xA0                   //EEPROM地址为0XA0，

//主从通信时从机地址设置（在从程序中设置）
#define OwnSlaveAddress0 0x60                   //按照8位设置，最低位设置为0或者1,写入寄存器之前会自动右移一位，再写入寄存器高7位
#define OwnSlaveAddress1 0xb0                   //按照8位设置，最低位设置为0或者1,写入寄存器之前会自动右移一位，再写入寄存器高7位
#define OwnSlaveAddress2 0xfe                   //按照8位设置，最低位设置为0或者1,写入寄存器之前会自动右移一位，再写入寄存器高7位
#define I2C_SCL_PIN               PA09
#define I2C_SDA_PIN               PA10

#define I2C_SCL_SET(pin)          pin##_AFx_I2CSCL()
#define I2C_SDA_SET(pin)          pin##_AFx_I2CSDA()

#define  I2C_SCL_GPIO_PORT       CW_GPIOA
#define  I2C_SCL_GPIO_PIN        GPIO_PIN_9    //如果改动口线则GPIO初始化代码需要做同步修改
#define  I2C_SDA_GPIO_PORT       CW_GPIOA
#define  I2C_SDA_GPIO_PIN        GPIO_PIN_10   //如果改动口线则GPIO初始化代码需要做同步修改


uint8_t u8Senddata[256] = {0};
uint8_t u8Recdata[256] = {0};
uint8_t RecvAddr = 0;

void SYSCTRL_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);


int32_t main(void)
{
    uint16_t Addr = 0;
    uint16_t i = 0;
    I2C_InitTypeDef I2C_InitStruct;

    //时钟初始化
    SYSCTRL_Configuration();
    //IO口初始化
    GPIO_Configuration();

    //I2C初始化
    I2C_InitStruct.PCLK_Freq = 4000000;
    I2C_InitStruct.I2C_SCL_Freq = 100000;
    I2C_InitStruct.I2C_SCL_Source = I2C_SCL_SRC_GPIO;
    I2C_InitStruct.I2C_SDA_Source = I2C_SDA_SRC_GPIO;

    I2C_DeInit();
    I2C_Master_Init( &I2C_InitStruct); //初始化模块

    //中断设置
    NVIC_Configuration();
    //I2C模块开始工作
    I2C_Cmd( ENABLE); //模块使能

    for (i = 0; i < 8; i++)
    {
        u8Senddata[i] = 0xFF;
    }

    for (Addr = 0; Addr < 256; Addr += 8)
    {
        I2C_eepromWriteBytesInt( 0xA0, Addr, u8Senddata, 8);
        while (gEEPROMInstance.EE_State != EE_STATE_IDLE);
        I2C_eepromWaitReady( 0xA0);
    }

    I2C_eepromReadBytesInt( 0xA0, 0x00, u8Recdata, 256);
    while (gEEPROMInstance.EE_State != EE_STATE_IDLE);

    for (i = 0; i < 256; i++)
    {
        if (u8Recdata[i] != 0xFF)
        {
            break;
        }
    }
    if (i != 256)
    {
        PA11_SETHIGH();
        while (1);
    }

    for (Addr = 0; Addr < 256; Addr += 8)
    {
        // 按8个字节一页准备数据
        for (i = 0; i < 8; i++)
        {
            u8Senddata[i] = Addr + i;
        }

        // 写入
        I2C_eepromWriteBytesInt( 0xA0, Addr, u8Senddata, 8);
        while (gEEPROMInstance.EE_State != EE_STATE_IDLE);
        I2C_eepromWaitReady( 0xA0);

        //读出
        I2C_eepromReadBytesInt( 0xA0, Addr, u8Recdata, 8);
        while (gEEPROMInstance.EE_State != EE_STATE_IDLE);

        //比较
        for (i = 0; i < 8; i++)
        {
            if (u8Senddata[i] != u8Recdata[i])
            {
                PA11_SETHIGH();
                while (1);
            }
        }
    }

    while (1)
    {
        PA11_TOG();
        FirmwareDelay(250000);
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void SYSCTRL_Configuration(void)
{
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

    GPIO_InitStructure.Pins = I2C_SCL_GPIO_PIN | I2C_SDA_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init(I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
    PA10_AFx_I2CSDA();
    PA09_AFx_I2CSCL();

    GPIO_InitStructure.Pins = GPIO_PIN_11;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

    PA11_SETLOW();
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

