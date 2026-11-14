/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2022-01-12
 *
 * @copyright Copyright (c) 2022
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
#include "main.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define  SPI_MASTER //主机模式
//#define  SPI_SLAVE  //从机模式

#define  HCLKFREQ                      16000000

//SPI GPIO
#define  SPI_SCK_GPIO_CLK              SYSCTRL_AHB_PERIPH_GPIOA
#define  SPI_SCK_GPIO_PORT             CW_GPIOA
#define  SPI_SCK_GPIO_PIN              GPIO_PIN_5
#define  SPI_SCK_AF()                  PA05_AFx_SPI1SCK()

#define  SPI_MISO_GPIO_CLK             SYSCTRL_AHB_PERIPH_GPIOA
#define  SPI_MISO_GPIO_PORT            CW_GPIOA
#define  SPI_MISO_GPIO_PIN             GPIO_PIN_6
#define  SPI_MISO_AF()                 PA06_AFx_SPI1MISO()

#define  SPI_MOSI_GPIO_CLK             SYSCTRL_AHB_PERIPH_GPIOA
#define  SPI_MOSI_GPIO_PORT            CW_GPIOA
#define  SPI_MOSI_GPIO_PIN             GPIO_PIN_7
#define  SPI_MOSI_AF()                 PA07_AFx_SPI1MOSI()

#define  SPI_CS_GPIO_CLK               SYSCTRL_AHB_PERIPH_GPIOB
#define  SPI_CS_GPIO_PORT              CW_GPIOA
#define  SPI_CS_GPIO_PIN               GPIO_PIN_4
#define  SPI_CS_AF()                   PA04_AFx_SPI1NCS()

//CS LOW or HIGH
#define  SPI_CS_LOW()     		         PA04_SETLOW()
#define  SPI_CS_HIGH()    		         PA04_SETHIGH()


#define  BufferSize                    ARRAY_SZ(TxBuffer1)
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void SPI_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t TxBuffer1[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                       0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                       0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                       0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
                       0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23
                      };
uint8_t TxBuffer2[] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
                       0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E,
                       0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
                       0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C,
                       0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83
                      };
uint8_t RxBuffer1[BufferSize];
uint8_t RxBuffer2[BufferSize];
uint8_t TxRxCounter = 0;
volatile TestStatus TransferStatus = FAILED;
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
 ******************************************************************************/
int32_t main(void)
{
    //配置RCC
    RCC_Configuration();

    //配置GPIO
    GPIO_Configuration();


    //配置SPI
    SPI_Configuration();

    //初始化SysTick
    InitTick(HCLKFREQ);

    /**************************主机 <-> 从机**********************/
#ifdef   SPI_MASTER
    //拉低CS，选中SPI从机
    SPI_CS_LOW();
    while (TxRxCounter < BufferSize)
    {
        //发送
        while (SPI_GetFlagStatus(CW_SPI, SPI_FLAG_TXE) == RESET);
        SPI_SendData(CW_SPI, TxBuffer1[TxRxCounter]);

        //接收
        while (SPI_GetFlagStatus(CW_SPI, SPI_FLAG_RXNE) == RESET);
        RxBuffer1[TxRxCounter++] = SPI_ReceiveData(CW_SPI);

        //等待从机将数据先写入DR寄存器，防止数据漏发
        SysTickDelay(10);
    }
    //拉高CS，释放SPI从机
    SPI_CS_HIGH();

    //检查收发数据一致性
    TransferStatus = Buffercmp(TxBuffer2, RxBuffer1, BufferSize);
#endif

#ifdef SPI_SLAVE
    //清空发送缓冲区和移位寄存器
    SPI_FlushSendBuff(CW_SPI);

    while (TxRxCounter < BufferSize)
    {
        //发送
        while (SPI_GetFlagStatus(CW_SPI, SPI_FLAG_TXE) == RESET);
        SPI_SendData(CW_SPI, TxBuffer2[TxRxCounter]);

        //接收
        while (SPI_GetFlagStatus(CW_SPI, SPI_FLAG_RXNE) == RESET);
        RxBuffer2[TxRxCounter++] = SPI_ReceiveData(CW_SPI);
    }
    //检查收发数据一致性
    TransferStatus = Buffercmp(TxBuffer1, RxBuffer2, BufferSize);
#endif

    if (TransferStatus == PASSED) //PASSED
    {
        //LED2亮
        PA11_SETHIGH();
    }
    else //FAILED
    {
        //LED2灭
        PA11_SETLOW();
    }
    while (1)
    {
    }
}

/**
 * @brief 配置RCC
 *
 */
void RCC_Configuration(void)
{
    //SYSCLK = HSI = 8MHz = HCLK = PCLK
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12);

    SYSCTRL_AHBPeriphClk_Enable(SPI_SCK_GPIO_CLK | SPI_MISO_GPIO_CLK | SPI_MOSI_GPIO_CLK | SPI_CS_GPIO_CLK, ENABLE);
    SYSCTRL_APBPeriphClk_Enable1(SYSCTRL_APB1_PERIPH_SPI1, ENABLE);

}

/**
 * @brief 配置GPIO
 *
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

#ifdef SPI_MASTER
    //推挽输出 SCK MOSI CS
    GPIO_InitStructure.Pins = SPI_SCK_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = SPI_MOSI_GPIO_PIN;
    GPIO_Init(SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = SPI_CS_GPIO_PIN;
    GPIO_Init(SPI_CS_GPIO_PORT, &GPIO_InitStructure);

    //浮空输入 MISO
    GPIO_InitStructure.Pins = SPI_MISO_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_Init(SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
    //SPI SCK MOSI MISO 复用

    //拉高CS
    SPI_CS_HIGH();
#endif

#ifdef SPI_SLAVE
    //SPI CS复用

    //推挽输出 MISO
    GPIO_InitStructure.Pins = SPI_MISO_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

    //浮空输入 SCK MOSI CS
    GPIO_InitStructure.Pins = SPI_SCK_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_Init(SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = SPI_MOSI_GPIO_PIN;
    GPIO_Init(SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = SPI_CS_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(SPI_CS_GPIO_PORT, &GPIO_InitStructure);
    SPI_CS_AF();
#endif
    SPI_SCK_AF();
    SPI_MISO_AF();
    SPI_MOSI_AF();

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.Pins = GPIO_PIN_11;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    //LED灭
    PA11_SETLOW();
}

/**
 * @brief 配置UART
 *
 */
void SPI_Configuration(void)
{
    SPI_InitTypeDef SPI_InitStructure = {0};

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    // 双线全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                         // 主机模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     // 帧数据长度为8bit
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                           // 时钟空闲电平为高
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                          // 第二个边沿采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             // 片选信号由SSI寄存器控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;    // 波特率为PCLK的8分频
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                    // 最高有效位 MSB 收发在前
    SPI_InitStructure.SPI_Speed = SPI_Speed_Low;                          // 低速SPI

#ifdef SPI_SLAVE
    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;                          // 从机模式
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;                             // SPI_CS引脚电平决定本机是否被选中
#endif

    SPI_Init(CW_SPI, &SPI_InitStructure);
    SPI_Cmd(CW_SPI, ENABLE);
}

/**
 * @brief Compares two buffers
 *
 * @param pBuffer1 :buffers to be compared
 * @param pBuffer2 :buffers to be compared
 * @param BufferLength :buffer's length
 * @return TestStatus
 * @arg PASSED: pBuffer1 identical to pBuffer2
 * @arg FAILED: pBuffer1 differs from pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
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

