#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Delay.h"
#include "Switch.h"

void GPIO_Config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_Pin_3;
    GPIO_InitStructure.Mode = GPIO_OUT_PP;
    GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);

    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);
}

void UART_Config(void) {
    // >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine     COMx_InitStructure;                 //结构定义
    COMx_InitStructure.UART_Mode       = UART_8bit_BRTx;    //模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use    = BRT_Timer1;            //选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate   = 115200ul;          //波特率, 一般 110 ~ 115200
    COMx_InitStructure.Morecommunicate = DISABLE;
    COMx_InitStructure.UART_RxEnable   = ENABLE;                //接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble  = DISABLE;           //波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);     //初始化串口1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE, Priority_1);    //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);     // 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

int main(void) {

    GPIO_Config();

    UART_Config();

    EA = 1;
    
    P53 = 0;

    while (1) {
        delay_ms(1);

        if (COM1.RX_TimeOut > 0) {

            if (--COM1.RX_TimeOut == 0) {

                if (COM1.RX_Cnt > 0) {
                    int i;
                    if ('A' == RX1_Buffer[0]) {
                        P53 = 1;
                    }else if('B' == RX1_Buffer[0]){
                        P53 = 0;
                    }

                    for (i = 0; i < COM1.RX_Cnt; i++) {
                        TX1_write2buff(RX1_Buffer[i]);
                    }
                    
                    COM1.RX_Cnt = 0;
                }
            }
        }

    }

}