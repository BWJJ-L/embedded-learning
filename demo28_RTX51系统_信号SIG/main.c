//#include "RTX51TNY.H"
#include "config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"

/**************************************

通过RTX51系统实现多个任务的独立运行(并发, 区别并行)

任务0: 初始化外设, 创建其他任务, 销毁自己
任务1: 循环接收串口消息, 模拟数据变化
        - 更新全局变量
        - 发送信号, 更新界面

任务2: 等待信号, 刷新界面 i2c, spi

***************************************/
void GPIO_config(void) {
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_3;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
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
// P5.3 闪烁
void sys_init() {
    EAXSFR(); // 扩展寄存器访问使能

    GPIO_config();
    UART_config();

    EA = 1;
}

// 这里函数名可随意, 建议不要使用start, 会和I2C.h里的Start冲突
void main_start() _task_ 0 {
    // 初始化引脚及外设
    sys_init();
    // 创建任务 1
    os_create_task(1);
    // 创建任务 2
    os_create_task(2);
    // 结束任务 0
    os_delete_task(0);
}

char global_char = ' ';

// 串口收到数据了
void on_uart1_recv() {
    u8 i = 0;
    // 这里处理收到的数据，做具体的逻辑，可以调用自己的on_uart1_recv
    // for(i=0; i<COM1.RX_Cnt; i++)	{
    //     RX1_Buffer[i]存的是接收的每个字节，写出用 TX1_write2buff
    // }
    
    printf("数据发生变化, 通知屏幕刷新\n");
    // 更新全局变量
    global_char = RX1_Buffer[0];
    
    // 发送信号
    os_send_signal(2);
}

void task_1() _task_ 1 {
    while(1) {
        if(COM1.RX_TimeOut > 0 && --COM1.RX_TimeOut == 0) {
            if(COM1.RX_Cnt > 0) {
                on_uart1_recv();
            }
            COM1.RX_Cnt = 0;
        }

        // 不要处理的太快
        os_wait2(K_TMO, 1); // 5ms
    }
}

// oled屏幕刷新任务
void task_2() _task_ 2 {

    while(1) {
        printf("task2_oled->等待信号\n");
        
        // 阻塞式等待(挂起), 直到收到signal信号, 释放阻塞
        os_wait1(K_SIG);
        
        // os_wait2(K_TMO, 1); // 5ms
        
        printf("task2_oled->oled屏幕刷新: %c!\n", global_char);

    }
}