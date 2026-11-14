/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    interrupts_cw32l011.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 CW.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "..\inc\main.h"
#include "interrupts_cw32l011.h"
#include "cw32l011_gpio.h"
#include "cw32l011_adc.h"
#include "cw32l011_lptim.h"
#include "cw32l011_uart.h"
#include "cw32l011_iwdt.h"
#include "cw32l011_rtc.h"
#include "cw32l011_sysctrl.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */


/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
/* USER CODE END TD */


/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */


/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */


/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */


/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */
extern volatile uint8_t gFlagIrq;
extern uint16_t gCntEoc;
extern volatile uint8_t gKeyStatus ; 
extern volatile uint8_t gLPTIMWakeUpIrq ; 
extern volatile uint8_t gRTCWakeUpIrq;
extern volatile uint8_t gUARTWakeUpIrq;
extern volatile uint8_t gRecvChar;


/* USER CODE END EV */


/******************************************************************************/
/*           Cortex-M0P Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn */

    /* USER CODE END NonMaskableInt_IRQn */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn */

    /* USER CODE END HardFault_IRQn */
    while (1)
    {
        /* USER CODE BEGIN W1_HardFault_IRQn */

        /* USER CODE END W1_HardFault_IRQn */
    }
}


/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
    /* USER CODE BEGIN SVCall_IRQn */

    /* USER CODE END SVCall_IRQn */
}


/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
    /* USER CODE BEGIN PendSV_IRQn */

    /* USER CODE END PendSV_IRQn */
}


/******************************************************************************/
/* CW32 Peripheral Interrupt Handlers                                         */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_cw32xxxx.s).                     */
/******************************************************************************/

/**
 * @brief This funcation handles WDT
 */
void WDT_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles LVD
 */
void LVD_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles RTC
 */
void RTC_IRQHandler(void)
{
    /* USER CODE BEGIN */
    if (CW_RTC->ISR & RTC_ISR_INTERVAL_Msk)
    {
        CW_RTC->ICR = 0x00;
        gRTCWakeUpIrq = 1;
    }
    /* USER CODE END */
}

/**
 * @brief This funcation handles FLASHRAM
 */
void FLASHRAM_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles SYSCTRL
 */
void SYSCTRL_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOA
 */
extern volatile uint8_t gKeyStatus;
void GPIOA_IRQHandler(void)
{
    /* USER CODE BEGIN */
    if (BSP_KEY2_PORT->ISR & BSP_KEY2_PIN)
    {
        BSP_KEY2_PORT->ICR &= ~BSP_KEY2_PIN;
        gKeyStatus = 1;
    }  
    else
    {
        BSP_KEY2_PORT->ICR = 0;
    }
 
    /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOB
 */
void GPIOB_IRQHandler(void)
{
    /* USER CODE BEGIN */
    
    /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOC
 */
void GPIOC_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles ADC
 */
void ADC_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles ATIM
 */
void ATIM_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles VC1
 */
void VC1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles VC2
 */
void VC2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GTIM1
 */
void GTIM1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GTIM2
 */
void GTIM2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles LPTIM
 */
void LPTIM_IRQHandler(void)
{
    /* USER CODE BEGIN */
	if (CW_LPTIM->ISR & LPTIM_ISR_ARRM_Msk)
    {        
        CW_LPTIM->ICR_f.ARRM = 0;
        gLPTIMWakeUpIrq=1;
    }
    /* USER CODE END */
}

/**
 * @brief This funcation handles BTIM1
 */
void BTIM1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles BTIM2
 */
void BTIM2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles BTIM3
 */
void BTIM3_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles I2C
 */
void I2C_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles SPI1
 */
void SPI1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles UART1
 */
void UART1_IRQHandler(void)
{
    /* USER CODE BEGIN */
    if (CW_UART1->ISR_f.RC)
    {
        CW_UART1->ICR_f.RC = 0;
        gUARTWakeUpIrq = 1;
        gRecvChar = CW_UART1->RDR;        
    }

    /* USER CODE END */
}

/**
 * @brief This funcation handles UART2
 */
void UART2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles UART3
 */
void UART3_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles FAULT
 */
void CLKFAULT_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}



/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT CW *****END OF FILE****/
