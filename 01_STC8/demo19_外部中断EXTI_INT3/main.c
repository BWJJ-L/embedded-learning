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
#include "Exti.h"



void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	    //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
    
    GPIO_InitStructure.Pin  = GPIO_Pin_2 | GPIO_Pin_7;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin  = GPIO_Pin_3;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);
}

void UART_config(void) {
    // >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine		COMx_InitStructure;					//结构定义
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/******************** INT配置 ********************/
void Exti_config(void)
{
    EXTI_InitTypeDef	Exti_InitStructure;							//结构定义

//    Exti_InitStructure.EXTI_Mode = EXT_MODE_RiseFall;//中断模式,   EXT_MODE_RiseFall,EXT_MODE_Fall
//    Ext_Inilize(EXT_INT0,&Exti_InitStructure);				//初始化
//    NVIC_INT0_Init(ENABLE,Priority_0);    //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    
    Exti_InitStructure.EXTI_Mode = EXT_MODE_Fall;//中断模式,   EXT_MODE_RiseFall,EXT_MODE_Fall
    Ext_Inilize(EXT_INT0,&Exti_InitStructure);				//初始化
    NVIC_INT3_Init(ENABLE,Priority_0);

}

void exti_int0_call(void) {
//    if(P32){
//        printf("int0 call 抬起 -> Rise上升沿\n");
//    } else {
//        printf("int0 call 按下 -> Fall下降沿\n");
//    }
}

void exti_int3_call(void) {
//    if(P37){
//        printf("int3 call 抬起 -> Rise上升沿\n");
//    } else {
//        printf("int3 call 按下 -> Fall下降沿\n");
//    }
}

int main(void) {
    
    EAXSFR();

    GPIO_config();
    UART_config();
    Exti_config();

    EA = 1;

    while (1) {
        
        if(4 == WakeUpSource) {
            if(!P37) {
                delay_ms(10);
                if(!P37) {
                    printf("按下 -> Fall下降沿\n");
                }
            }
            WakeUpSource = 0;
        }
        
    }
}