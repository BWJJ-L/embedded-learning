#include "App.h"
#include "PCF8563.h"

static void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;      //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_7;       //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
}

static void UART_config(void) {
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

static void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;						//结构定义
	//定时器2做16位自动重装, 中断频率为1000HZ
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000);				//初值
	TIM_InitStructure.TIM_PS        = 0;					//8位预分频器(n+1), 0~255, (注意:并非所有系列都有此寄存器,详情请查看数据手册)
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer3,&TIM_InitStructure);				//初始化Timer2	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer3_Init(ENABLE,NULL);		//中断使能, ENABLE/DISABLE; 无优先级
}

void PCF8563_on_Alarm(void) {
}
void PCF8563_on_Timer(void) {
}

Clock_t c;

void on_uart1_receive() {
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

void Timer3_callback() {
     // 串口接收处理
    if (COM1.RX_TimeOut > 0) {
        //超时计数
        if (--COM1.RX_TimeOut == 0) {
            if (COM1.RX_Cnt > 0) {
                on_uart1_receive();
            }
            COM1.RX_Cnt = 0;
        }
    }
}

void task_7() _task_ TASK_RTC {
    // 时间读取和屏幕刷新不能同时进行， 容易卡死，所以延时一会再启动
    os_wait2(K_TMO, 100);
    
    EAXSFR();

    GPIO_config();
    UART_config();
    Timer_config();
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
    TIME_NUMBER  : 读写的数据个数
    ******************************************/
    
    while (1) {
        APP_I2C_OLED_Refresh(0);
        os_wait2(K_TMO, 200);
    }
}

void RTC_show() {
    char strBuff[32];
    PCF8563_get_Clock(&c);
    
    // BCD格式数据, 要用16进制打印
    // 年-月-日
    sprintf(strBuff, "%04d-%02d-%02d", (int)c.years, (int)c.months, (int)c.days);
    I2C_OLED_ShowString(0, 2, strBuff, 16);
    
    // 时:分:秒 周
    sprintf(strBuff, "%02d:%02d:%02d  W:%d", (int)c.hours, (int)c.minutes, (int)c.seconds, (int)c.weekdays);
    I2C_OLED_ShowString(0, 4, strBuff, 16);
}

void TASK_RTC_reset(){
    Timer3_Run(DISABLE);
    os_delete_task(TASK_RTC);
}