#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "Timer.h"
#include "Delay.h"
#include "ADC.h"
#include "STC8H_PWM.h"
#include "I2C.h"
#include "DHT11.h"

void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;      //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
}

void UART_config(void) {
    // >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine     COMx_InitStructure;                 //结构定义
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx; //模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;         //选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;           //波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;             //接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;            //波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);     //初始化串口1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE, Priority_1);    //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);     // 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

void on_uart1_receive(void) {
    int i;
    int8 result;
    float humidity;
    float temperature;
    
    for (i = 0; i < COM1.RX_Cnt; i++) {
        // RX1_Buffer[i]存的是接收的每个字节，写出用 TX1_write2buff
        TX1_write2buff(RX1_Buffer[i]);
    }
    
    result = DHT11_get_info(&humidity, &temperature);
    
    if(result == SUCCESS) {
        printf("湿度：%.2f%%  温度：%.2f℃\n",humidity,temperature);
    }
}

int main(void) {

    EAXSFR();

    GPIO_config();
    UART_config();
    DHT11_init();

    EA = 1;

    while (1) {

        if (COM1.RX_TimeOut > 0) {
            //超时计数
            if (--COM1.RX_TimeOut == 0) {
                if (COM1.RX_Cnt > 0) {
                    on_uart1_receive();
                }
                COM1.RX_Cnt = 0;
            }
        }

        // 不要处理的太快
        delay_ms(1);

    }
}