#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "Timer.h"
#include "Delay.h"
#include "STC8H_PWM.h"
#include "I2C.h"
#include "PCF8563.h"
#include "Exti.h"

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
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	    //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
    
    GPIO_InitStructure.Pin  = GPIO_Pin_7;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	    //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
    
    GPIO_InitStructure.Pin  = GPIO_Pin_1;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化
    
    GPIO_InitStructure.Pin  = GPIO_Pin_3;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);//初始化
    
    P01 = 0;
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

void	PWM_config(void)
{
	PWMx_InitDefine		PWMx_InitStructure;

	PWMx_InitStructure.PWM_Mode    		=	CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    		=  (MAIN_Fosc / 1000);	//PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect  = ENO6P;				//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM6, &PWMx_InitStructure);			//初始化PWM,  PWMA,PWMB

	PWMx_InitStructure.PWM_Period   = (MAIN_Fosc / 1000) -1;					//周期时间,   0~65535
	PWMx_InitStructure.PWM_DeadTime = 0;					//死区发生器设置, 0~255
	PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//主输出使能, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//使能计数器, ENABLE,DISABLE
	PWM_Configuration(PWMB, &PWMx_InitStructure);			//初始化PWM通用寄存器,  PWMA,PWMB

	PWM6_SW(PWM6_SW_P01);					//PWM6_SW_P21,PWM6_SW_P54,PWM6_SW_P01,PWM6_SW_P75

	NVIC_PWM_Init(PWMB,DISABLE,Priority_0);
}

/******************** INT配置 ********************/
void Exti_config(void)
{
	EXTI_InitTypeDef	Exti_InitStructure;							//结构定义

	Exti_InitStructure.EXTI_Mode      = EXT_MODE_Fall;//中断模式,   EXT_MODE_RiseFall,EXT_MODE_Fall
	Ext_Inilize(EXT_INT3,&Exti_InitStructure);				//初始化
	NVIC_INT3_Init(ENABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

void PCF8563_on_Alarm(void) {
    printf("Alarm");
    P01 = !P01;
    delay_ms(100);
    P01 = !P01;
    delay_ms(100);
    P01 = !P01;
    delay_ms(100);
    P01 = !P01;
    delay_ms(100);
    P01 = !P01;
    delay_ms(100);
    P01 = !P01;
    delay_ms(100);
}

void PCF8563_on_Timer(void) {
    printf("Timer");
    P53 = !P53;
    delay_ms(100);
    P53 = !P53;
    delay_ms(100);
    P53 = !P53;
    delay_ms(100);
    P53 = !P53;
    delay_ms(100);
    P53 = !P53;
    delay_ms(100);
}

int main(void) {
    Clock_t c;
    Alarm_t a;
    u8 Timer_frequency;
    u8 Timer_countdown;
    
    EAXSFR();

    GPIO_config();
    UART_config();
    PCF8563_init();
    Exti_config();

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
    
    //禁用哪个时间单位，就把那个时间单位置为负数
    a.alarm_minute = 0, a.alarm_hour = 0, a.alarm_day = 28, a.alarm_week = 4;
    PCF8563_set_Alarm(a);
    
    PCF8563_enable_Alarm(ENABLE);
    
    
    Timer_frequency = 0x01; //4.096kHz(0x00) 64Hz(0x01) 1Hz(0x02) 1/60Hz(0x03)
    Timer_countdown = 192;
    PCF8563_set_Timer(Timer_frequency, Timer_countdown);
    PCF8563_enable_Timer(ENABLE);
    
    while (1) {
        PCF8563_get_Clock(&c);
        
        // BCD格式数据, 要用16进制打印
        // 年-月-日
        printf("%04d-%02d-%02d  ", (int)c.years, (int)c.months, (int)c.days);
        // 时:分:秒
        printf("%02d:%02d:%02d  ", (int)c.hours, (int)c.minutes, (int)c.seconds);
        // week
        printf("week->%d\n", (int)c.weekdays);
        
        if(4 == WakeUpSource) {
            WakeUpSource = 0;
            exti_int3_call();
        }
        
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }
}