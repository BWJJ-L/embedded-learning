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
#include "Exti.h"
#include "oled.h"
#include "bmp.h"

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

3. 设置Timer运行频率 & 启用Timer
4. 设置Timer计数值n
5. 设置cs2, TIE置1启用, 清理TF标记
6. 启用外部中断, 判断并清理Timer的TF标记

*************************************************************************************/

void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;      //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_7;       //指定要初始化的IO,
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

void PCF8563_on_Alarm(void) {
    printf("Alarm");
}

void PCF8563_on_Timer(void) {
    printf("Timer");
}

void on_uart1_receive() {
    Clock_t c;
    int i;
    // 这里处理收到的数据，做具体的逻辑，可以调用自己的on_uart1_recv
    for (i = 0; i < COM1.RX_Cnt; i++)    {
        // RX1_Buffer[i]存的是接收的每个字节，写出用 TX1_write2buff
        TX1_write2buff(RX1_Buffer[i]);
    }

    c.years = RX1_Buffer[2] * 100 + RX1_Buffer[3];
    c.months = RX1_Buffer[4];
    c.days = RX1_Buffer[5];
    c.weekdays = RX1_Buffer[6];
    c.hours = RX1_Buffer[7];
    c.minutes = RX1_Buffer[8];
    c.seconds = RX1_Buffer[9];
    PCF8563_set_Clock(c);
}

int main(void) {
    Clock_t c;
    char strBuff[32];
    int a = 1006;
    u16 cnt;

    EAXSFR();

    GPIO_config();
    UART_config();
    PCF8563_init();

    OLED_Init();//初始化OLED
    OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示

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
    TIME_NUMBER  : 读写的数据个数
    ******************************************/
    c.years = 2026, c.months = 8, c.days = 27, c.weekdays = 3;
    c.hours = 23, c.minutes = 59, c.seconds = 55;
    PCF8563_set_Clock(c);
    
    
    while (1) {

        if (++cnt > 1000) {
            cnt = 0;
            
            PCF8563_get_Clock(&c);

            // BCD格式数据, 要用16进制打印
            // 年-月-日
            sprintf(strBuff, "%04d-%02d-%02d", (int)c.years, (int)c.months, (int)c.days);
            OLED_ShowString(0, 0, strBuff, 16);

            // 时:分:秒 周
            sprintf(strBuff, "%02d:%02d:%02d  W:%d\n", (int)c.hours, (int)c.minutes, (int)c.seconds, (int)c.weekdays);
            OLED_ShowString(0, 2, strBuff, 16);


            sprintf(strBuff, "%d", a--);
            OLED_ShowString(0, 6, "    ", 16); // 先擦除,不擦除会导致最后一位的0不会消失
            OLED_ShowString(0, 6, strBuff, 16); // 再写入
        }

        // --------------------------------------------------------------------- 串口接收处理
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