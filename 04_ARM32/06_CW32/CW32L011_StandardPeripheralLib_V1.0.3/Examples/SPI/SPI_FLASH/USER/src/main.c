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
#include "cw32l011_startkit_board.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define  BufferSize                     (ARRAY_SZ(TxBuffer) - 1)

#define  FLASH_WriteAddress             0x00000
#define  FLASH_ReadAddress              FLASH_WriteAddress
#define  FLASH_SectorToEraseAddress     FLASH_WriteAddress
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
void UART_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t TxBuffer[] = "CW32L011 SPI FLASH Example";
uint8_t RxBuffer[BufferSize];

uint8_t DeviceID = 0;
uint16_t ManufactDeviceID = 0;
uint32_t JedecID = 0;
uint8_t UniqueID[8];

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
    
    Bsp_Init(24000000);
    Bsp_Led_Off(&LED2);

    
    //配置SPI
    SPI_FLASH_Init();

    printf("\r\nCW32L011 SPI FLASH Example\r\n");

    DeviceID = SPI_FLASH_DeviceID();
    ManufactDeviceID = SPI_FLASH_ManufactDeviceID();
    JedecID = SPI_FLASH_JedecID();
    SPI_FLASH_UniqueID(UniqueID);

    printf("\r\nDeviceID = 0x%X\r\nManufactDeviceID = 0x%X\r\nJedecID = 0x%X", DeviceID, ManufactDeviceID, JedecID);
    printf("\r\nUniqueID = 0x ");
    for(uint8_t i = 0; i<8; i++)
    {
        printf("%X ", UniqueID[i]);
    }

    /* Check JEDEC ID */
    if(JedecID == sJedecID)
    {
        printf("\r\n\nFLASH Detected\r\n");;

        //擦除扇区 4KB
        SPI_FLASH_SectorErase(FLASH_SectorToEraseAddress);

        //写数据
        SPI_FLASH_BufferWrite(TxBuffer, FLASH_WriteAddress, BufferSize);
        printf("\r\nThe data that has been written is:%s\r\n", TxBuffer);

        //读数据
        SPI_FLASH_BufferRead(RxBuffer, FLASH_ReadAddress, BufferSize);
        printf("\r\nThe data that has been read out is:%s\r\n", RxBuffer);

        //检查写入的数据与读出的数据是否一致
        TransferStatus = Buffercmp(TxBuffer, RxBuffer, BufferSize);

        //擦除扇区 4KB
        SPI_FLASH_SectorErase(FLASH_SectorToEraseAddress);

        //读数据
        SPI_FLASH_BufferRead(RxBuffer, FLASH_ReadAddress, BufferSize);

        if(TransferStatus == PASSED)
        {
            Bsp_Led_On(&LED2);
            printf("\r\nFLASH Success\r\n");
        }
        else
        {            
            printf("\r\nFLASH Error 1\r\n");
        }

        for(uint8_t j = 0; j < BufferSize; j++)
        {
            if(RxBuffer[j] != 0xFF)
            {               
                printf("\r\nFLASH Error 2\r\n");
            }
        }
    }
    else// (FlashID ！= sFLASH_ID)
    {        
        printf("\r\nFLASH Error 3\r\n");
    }

    while(1)
    {

    }
}

/**
 * @brief 配置RCC
 *
 */
void RCC_Configuration(void)
{
    //SYSCLK = HSI = 24MHz = HCLK = PCLK
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI24MHZ);
}


/**
 * @brief Compares two buffers
 *
 * @param pBuffer1 :buffers to be compared
 * @param pBuffer2 :buffers to be compared
 * @param BufferLength :buffer's length
 * @return TestStatus
 *     @arg PASSED: pBuffer1 identical to pBuffer2
 *     @arg FAILED: pBuffer1 differs from pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while(BufferLength--)
    {
        if(*pBuffer1 != *pBuffer2)
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

