#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "Delay.h"
#include "ADC.h"
#include "STC8H_PWM.h"
#include "Timer.h"

/****************
独立按键: 获取按键的按下和抬起事件

KEY1: P51
KEY2: P52
KEY3: P53
KEY4: P54

****************/
#define BUZZER_GPIO     P00
#define KEY1    P51
#define KEY2    P52
#define KEY3    P53
#define KEY4    P54

void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;     //结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;      //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
    
    	GPIO_InitStructure.Pin  = GPIO_Pin_1;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化
    
        GPIO_InitStructure.Pin  = GPIO_Pin_0;       //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化

    // 准双向
    P5_MODE_IO_PU(GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);

    P0_MODE_OUT_PP(GPIO_Pin_1);
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

/******************* AD配置函数 *******************/
void    ADC_config(void) {
    ADC_InitTypeDef     ADC_InitStructure;      //结构定义

    ADC_InitStructure.ADC_SMPduty   = 31;       //ADC 模拟信号采样时间控制, 0~31（注意： SMPDUTY 一定不能设置小于 10）
    ADC_InitStructure.ADC_CsSetup   = 0;        //ADC 通道选择时间控制 0(默认),1
    ADC_InitStructure.ADC_CsHold    = 1;        //ADC 通道选择保持时间控制 0,1(默认),2,3
    ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X1T;       //设置 ADC 工作时钟频率 ADC_SPEED_2X1T~ADC_SPEED_2X16T
    ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;  //ADC结果调整,  ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
    ADC_Inilize(&ADC_InitStructure);        //初始化
    ADC_PowerControl(ENABLE);               //ADC电源开关, ENABLE或DISABLE
    NVIC_ADC_Init(DISABLE, Priority_0);     //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

#define DOWN 0
#define UP   1

u8 state = 0x0F;

#define IS_KEY_DOWN(k)  (((state >> k) & 0x01) == DOWN)
#define IS_KEY_UP(k)    (((state >> k) & 0x01) == UP)

#define SET_KEY_DOWN(k) (state &= ~(1 << k))
#define SET_KEY_UP(k)   (state |=  (1 << k))

#define LAST_KEY_SATE(k)   ((state >> k) & 0x01)

u8 get_Key_Value(u8 key) {
    switch (key) {
        case 0:
            return KEY1;
        case 1:
            return KEY2;
        case 2:
            return KEY3;
        case 3:
            return KEY4;
        default:
            return 0;
    }
}

u16 hz[] = {33, 37, 41, 44, 49, 55, 62, 65};

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

int main() {
        u8 HzIdx = 0;
    u8 len = sizeof(hz) / sizeof(hz);

    EAXSFR(); // 扩展寄存器使能

    GPIO_config();
    UART_config();

    EA = 1;

    P01 = 0;

    while (1) {
        if(P51 == 0) {
            P01 = 1;
        }else if (P51 == 1){
            P01 = 0;
        }
        
        if (P52 == 0) {

            Timer_config(hz[HzIdx]);

            if (++HzIdx >= len)  HzIdx = 0;
            
            delay_ms(100);

        } else if (P52 == 1) {
            BUZZER_GPIO = 0;
        }


        }
    }