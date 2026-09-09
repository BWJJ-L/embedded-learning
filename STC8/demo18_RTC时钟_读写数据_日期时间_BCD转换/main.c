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
#include "PCF8563.h"

/*************************************************************************************
RTC时钟

1. 通过I2C外设(协议)读取PCF8563芯片数据(16字节)
    - 开启扩展寄存器访问使能 EAXSFR()
    - 引脚模式: 开漏输出模式 Open 1, Drain 0
    - I2C协议(一对多)
    
2. 引脚
    - SCL: P3.2 时钟线   (Serial Clock)
    - SDA: P3.3 数据线   (Serial Data)
    - INT: P3.7 下降沿
    
读取芯片里的年月日, 时分秒(世纪, 周)

*************************************************************************************/

void    GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	    //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
}

void    UART_config(void) {
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


int main(void) {
    Clock_t c;
    
    EAXSFR();

    GPIO_config();
    UART_config();
    PCF8563_init();

    EA = 1;
    
        /*****************************************
    两个地址:
    dev_addr: 设备地址, 决定了I2C和哪个设备通讯(device address)
        - 设备地址:   0x51 (7bit)
        - 设备读地址：0xA3;  (0x51 << 1) | 1
        - 设备写地址：0xA2;  (0x51 << 1)
    
    mem_addr: 存储地址, 决定了I2C和该设备的哪个寄存器register交互(memory address)
        - 用于指定从哪个寄存器开始读写数据
        - 每次读写数据字节后，寄存器地址自动累加。
    
    u8 *p   : 读写数据缓冲区
    number  : 读写的数据个数
    ******************************************/
    c.years = 2099, c.months = 12, c.days = 31, c.weekdays = 4;
    c.hours = 23, c.minutes = 59, c.seconds = 55;
    
    PCF8563_set_Clock(c);
    
    while (1) {
        PCF8563_get_Clock(&c);
        
        // BCD格式数据, 要用16进制打印
        // 年-月-日
        printf("%04d-%02d-%02d\n", (int)c.years, (int)c.months, (int)c.days);
        // 时:分:秒
        printf("%02d:%02d:%02d\n", (int)c.hours, (int)c.minutes, (int)c.seconds);
        // week
        printf("week->%d\n", (int)c.weekdays);
        
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }
}