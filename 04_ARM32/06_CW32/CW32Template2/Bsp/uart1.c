#include "uart1.h"

/**
 * @brief 配置RCC
 *
 */
static void RCC_Configuration(void)
{
	// board_init() 已经调用了
    //SYSCLK = HSI = 8MHz = HCLK = PCLK
//    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12);

    //外设时钟使能
    SYSCTRL_AHBPeriphClk_Enable(UART1_GPIO_CLK, ENABLE);
    UART1_APBClkENx(UART1_CLK, ENABLE);
}                            

/**
 * @brief 配置GPIO
 *
 */
static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pins = UART1_TX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(UART1_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = UART1_RX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(UART1_RX_GPIO_PORT, &GPIO_InitStructure);

    //UART TX RX 复用
    UART1_AFTX;
    UART1_AFRX;
}

/**
 * @brief 配置UART
 *
 */
static void UART_Configuration(void)
{
    UART_InitTypeDef UART_InitStructure = {0};

    UART_InitStructure.UART_BaudRate = UART1_BaudRate;
    UART_InitStructure.UART_Over = UART_Over_16;
    UART_InitStructure.UART_Source = UART_Source_PCLK;
    UART_InitStructure.UART_UclkFreq = UART1_UclkFreq;
    UART_InitStructure.UART_StartBit = UART_StartBit_FE;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(UART1, &UART_InitStructure);
}

/**
 * @brief 配置NVIC
 *
 */
void NVIC_Configuration(void)
{
    //优先级，无优先级分组
    NVIC_SetPriority(UART1_IRQ, 0);
    //UARTx中断使能
    NVIC_EnableIRQ(UART1_IRQ);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f) {
    // 发送一个字节
    UART_SendData_8bit(CW_UART1, (uint8_t)ch);
    // 等待发送完成
    while( RESET == UART_GetFlagStatus(CW_UART1, UART_FLAG_TXE) ){}

    return ch;
}

void uart1_init() {
	RCC_Configuration(); 	// 配置RCC
	GPIO_Configuration(); 	// 配置GPIO
	UART_Configuration();   // 配置串口
	NVIC_Configuration();	 //配置NVIC
	
	//使能UARTx RC中断  UART_IT_RC:    接收完成中断
    UART_ITConfig(UART1, UART_IT_RC, ENABLE);
	UART_ClearITPendingBit(UART1, UART_IT_RC);
}

void uart1_sendstring(char *string){ // 串口1发送字符串
	UART_SendString(UART1, string);
}