#include "Config.h"
#include "GPIO.h"
#include "Delay.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h" // 配置外设引脚

/*************************************************************************************
模板代码:

1. 导通总开关: 拉低P45

从右 -> 左
从左 -> 右

*************************************************************************************/

#define LED_SW  P45
#define LED1    P27
#define LED2    P26
#define LED3    P15
#define LED4    P14
#define LED5    P23
#define LED6    P22
#define LED7    P21
#define LED8    P20

void GPIO_config(void) {
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_5;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;	    //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//初始化
    
    // P20,P21,P22,P23,P26,P27
    GPIO_InitStructure.Pin  = GPIO_Pin_LOW | GPIO_Pin_6 | GPIO_Pin_7;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;	    //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure);//初始化
    
    // P14, P15
    GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;	    //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);//初始化
    
    // 初始化P30, P31
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	    //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
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


void on_uart1_recv(){
    int i;
    // 这里处理收到的数据，做具体的逻辑，可以调用自己的on_uart1_recv
    for(i=0; i<COM1.RX_Cnt; i++)	{
        // RX1_Buffer[i]存的是接收的每个字节，写出用 TX1_write2buff
        TX1_write2buff(RX1_Buffer[i]);
    }
}

int main() {
    int8 i = 0;  //[-128, 127] 或使用int等有符号数
    
    // 引脚初始化
    GPIO_config();
    // 串口初始化
    UART_config();
    // 开启全局中断
    EA = 1;
    
    //1. 导通总开关: 拉低P45
    P45 = 0;
    
    //2. 熄灭所有灯
    LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;
   

    while(1) {
        
//        从右 -> 左
        for(i = 0; i < 8; i++){ // 0,1,2,3,4,5,6,7,8
            
            LED1 = (i == 0) ? 0 : 1; // i是当前位置, 亮, 否则 灭
            LED2 = (i == 1) ? 0 : 1; // i是当前位置, 亮, 否则 灭
            LED3 = (i == 2) ? 0 : 1; // i是当前位置, 亮, 否则 灭
            LED4 = (i == 3) ? 0 : 1; // i是当前位置, 亮, 否则 灭
            LED5 = (i == 4) ? 0 : 1; // i是当前位置, 亮, 否则 灭
            LED6 = (i == 5) ? 0 : 1; // i是当前位置, 亮, 否则 灭
            LED7 = (i == 6) ? 0 : 1; // i是当前位置, 亮, 否则 灭
            LED8 = (i == 7) ? 0 : 1; // i是当前位置, 亮, 否则 灭
                         
            delay_ms(250);
        }
        
        LED2  = LED4 =  LED6 =  LED8 = 1;
        delay_ms(250);
        
////        从左 -> 右
//        for(i = 7; i >= 0; i--){ // 0,1,2,3,4,5,6,7,8
//            printf("i = %d\n", (int)i);
//            
//            LED1 = (i == 0) ? 0 : 1; // i是当前位置, 亮, 否则 灭
//            LED2 = (i == 1) ? 0 : 1; // i是当前位置, 亮, 否则 灭
//            LED3 = (i == 2) ? 0 : 1; // i是当前位置, 亮, 否则 灭
//            LED4 = (i == 3) ? 0 : 1; // i是当前位置, 亮, 否则 灭
//            LED5 = (i == 4) ? 0 : 1; // i是当前位置, 亮, 否则 灭
//            LED6 = (i == 5) ? 0 : 1; // i是当前位置, 亮, 否则 灭
//            LED7 = (i == 6) ? 0 : 1; // i是当前位置, 亮, 否则 灭
//            LED8 = (i == 7) ? 0 : 1; // i是当前位置, 亮, 否则 灭
//                         
//            delay_ms(250);
//        }
    }
}