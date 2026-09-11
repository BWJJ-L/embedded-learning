#include "Config.h"
#include "GPIO.h"
#include "NVIC.h"
#include "UART.h"
#include "Switch.h"
#include "Timer.h"
#include "STC8H_PWM.h"
#include "Delay.h"

#define BUZZER_GPIO     P00

void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;     //结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_0;       //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);

    GPIO_InitStructure.Pin  = GPIO_Pin_1;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);

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

u16 hz[] = {33, 37, 41, 44, 49, 55, 62, 65};

// 哆啦A梦 - 主歌 + 高潮乐段（完整版）
u16 code notes[] = {
    // 主歌 A
    392, 523, 523, 659, 880, 659, 784, 784, 880, 784, 659, 698, 659, 587,
    // 主歌 B-开放
    440, 587, 587, 698, 988, 988, 880, 784, 698, 698, 659, 880, 988, 523, 587,
    // 主歌 A'
    392, 523, 523, 659, 880, 659, 784, 784, 880, 784, 659, 698, 659, 587,
    // 主歌 B-收束
    440, 587, 587, 698, 988, 988, 880, 784, 698, 698, 659, 988, 587, 523,
    // ---- 高潮乐段(ANANAN) ----
    880, 880, 784, 698, 784, 880, 784, 587, 659, 698, 587, 784,
    880, 784, 587, 659, 698, 587, 784,
    880, 784, 698, 587, 587, 988, 880, 784, 880, 784,
    784, 880, 659, 587, 523,
    880, 784, 698, 587, 587, 988, 880, 784, 880, 784,
    784, 880, 659, 587, 523,
    523, 587, 659, 698, 659, 698, 659, 587,
    659, 587, 659, 587, 523,
    880, 880, 494, 523, 587, 440, 440, 494, 523, 587, 523
};

u8 code durations[] = {
    // 主歌 A
    3, 1, 3, 1, 3, 1, 4, 3, 1, 3, 1, 3, 1, 4,
    // 主歌 B-开放
    3, 1, 3, 1, 3, 1, 3, 1, 2, 2, 3, 1, 2, 4, 2,
    // 主歌 A'
    3, 1, 3, 1, 3, 1, 4, 3, 1, 3, 1, 3, 1, 4,
    // 主歌 B-收束
    3, 1, 3, 1, 3, 1, 3, 1, 2, 2, 3, 1, 4, 4,
    // ---- 高潮乐段 ----
    2, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 3,
    2, 1, 1, 1, 1, 1, 3,
    2, 1, 1, 1, 1, 1, 1, 1, 1, 3,
    1, 1, 1, 1, 3,
    2, 1, 1, 1, 1, 1, 1, 1, 1, 3,
    1, 1, 1, 1, 3,
    1, 1, 1, 1, 1, 1, 1, 3,
    1, 1, 1, 1, 3,
    2, 2, 2, 1, 1, 2, 2, 2, 1, 1, 4
};

void Timer_config(u16 hz_Output) {
    TIM_InitTypeDef     TIM_InitStructure;                      //结构定义
    //定时器0做16位自动重装, 中断频率为1000HZ
    TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;  //指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;     //指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut    = DISABLE;              //是否输出高速脉冲, ENABLE或DISABLE
    TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / hz_Output / 2);        //初值,
    TIM_InitStructure.TIM_Run       = ENABLE;               //是否初始化后启动定时器, ENABLE或DISABLE
    Timer_Inilize(Timer0, &TIM_InitStructure);              //初始化Timer0    Timer0,Timer1,Timer2,Timer3,Timer4
    NVIC_Timer0_Init(ENABLE, Priority_0);       //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}



void Call_Back_Buzzer(void) {
//    static cnt = 0;
//    
//    
//    BUZZER_GPIO = 0;

//    if (cnt == 1) {
//        BUZZER_GPIO = 1;
//        cnt = 0;
//    }

//    cnt++;
    
    BUZZER_GPIO = !BUZZER_GPIO;
}

int main(void) {
    u8 HzIdx = 0;
    u8 len = sizeof(notes) / sizeof(notes[0]);

    EAXSFR();

    GPIO_config();
    UART_config();

    EA = 1;


    while (1) {
        if (P51 == 0) {

            Timer_config(hz[HzIdx]);

            if (++HzIdx >= len)  HzIdx = 0;
            
            delay_ms(100);

        } else if (P51 == 1) {
            BUZZER_GPIO = 0;
        }
    }
}