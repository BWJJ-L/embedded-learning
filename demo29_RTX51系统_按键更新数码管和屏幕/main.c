#include "Config.h"
#include "GPIO.h"
#include "delay.h"
#include "UART.h"
#include "Switch.h"
#include "NVIC.h"
#include "MATRIXKEY.h"
#include "NIXIE.h"

void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;     //结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;      //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
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

void sys_init(void) {
    EAXSFR();

    GPIO_config();
    UART_config();

    EA = 1;
}

//最多15个任务，就是_task_ 0 到 _task_ 15
void task_0(void) _task_ 0 {
    //初始化
    sys_init();
    Nixie_GPIO_INIT();
    MK_init();
    //创建任务1
    os_create_task(1);
    //创建任务2
    os_create_task(2);
    //结束任务0
    os_delete_task(0);
}

u8 numbers[] = {2, 3}; 
void KEY_scan(u8 row, u8 col, u8 state) {
    
}

void task_1(void) _task_ 1 {
    while (1) {
        MK_key_state(KEY_scan);
        
        os_wait2(K_TMO, 2);
    }
}

void task_2(void) _task_ 2 {
    while (1) {
        u8 i = 0;
        for(i = 0;i < 2;i++) {
            Nixie_Display(numbers[i], i);
            os_wait2(K_TMO, 1);
        }
        
        
        os_wait2(K_TMO, 2);
    }
}