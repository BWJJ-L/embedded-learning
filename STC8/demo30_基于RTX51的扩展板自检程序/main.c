#include "Config.h"
#include "GPIO.h"
#include "delay.h"
#include "UART.h"
#include "Switch.h"
#include "NVIC.h"
#include "App.h"


void GPIO_config(void) {
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
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

// 最多15个任务，就是_task_ 0 到 _task_ 15
// 这里函数名可随意, 建议不要使用start, 会和I2C.h里的Start冲突
void task_0(void) _task_ 0 {
    //初始化
    sys_init();
    //创建任务扫描独立按键
    os_create_task(TASK_KEYS);
    //创建任务SPI oled屏幕刷新任务
    os_create_task(TASK_SPI_OLED);
    //创建任务I2C oled屏幕刷新任务
    os_create_task(TASK_I2C_OLED);
    // 创建任务LED流水灯       必须上来就创建，否则上来LED灯不会亮
    os_create_task(TASK_LED);
    //结束任务0
    os_delete_task(0);
}