#include "Config.h"
#include "Delay.h"
#include "UART.h"
#include "GPIO.h"
#include "STC8H_PWM.h"
#include "NVIC.h"
#include "Switch.h"

void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;     //结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_2;       //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化
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

#define PRESCALER   10

#define FREQ        50

#define PERIOD  (MAIN_Fosc / PRESCALER / FREQ)

PWMx_Duty   dutyB;

void    PWM_config(void) {
    PWMx_InitDefine     PWMx_InitStructure;

    // 配置PWM7
    PWMx_InitStructure.PWM_Mode         = CCMRn_PWM_MODE1;  //模式,     CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty         = dutyB.PWM7_Duty;  //PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect    = ENO7P;            //输出通道选择, ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM7, &PWMx_InitStructure);           //初始化PWM,  PWMA,PWMB

    // 配置PWMB
    PWMx_InitStructure.PWM_Period        = PERIOD - 1;          //周期时间,   0~65535
    PWMx_InitStructure.PWM_DeadTime      = 0;                   //死区发生器设置, 0~255
    PWMx_InitStructure.PWM_MainOutEnable = ENABLE;          //主输出使能, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable    = ENABLE;          //使能计数器, ENABLE,DISABLE
    PWM_Configuration(PWMB, &PWMx_InitStructure);           //初始化PWM通用寄存器,  PWMA,PWMB

    // 配置预分频系数
    PWMB_Prescaler(PRESCALER - 1); // 配置预分频系数

    // 切换PWM通道
    PWM7_SW(PWM7_SW_P02);                   //PWM7_SW_P22,PWM7_SW_P33,PWM7_SW_P02,PWM7_SW_P76

    // 初始化PWMB的中断
    NVIC_PWM_Init(PWMB, DISABLE, Priority_0);
}

void Serve_set_angle(float angle) {
    u16 duty = 2000 * angle / 180 + 500;

    dutyB.PWM7_Duty = PERIOD * duty / 20000;
    UpdatePwm(PWM7, &dutyB);
}


int percent = 0;

void on_uart1_receive(void) {
    int direction = 0;

    if (COM1.RX_TimeOut > 0) {

        if (--COM1.RX_TimeOut == 0) {

            if (COM1.RX_Cnt > 0) {
                if (0x00 == RX1_Buffer[0])  direction = -10;
                else if (0x01 == RX1_Buffer[0])  direction = 10;

                percent += direction;
                if (percent >= 180)  percent = 180;
                else if (percent <= 0)  percent = 0;

                Serve_set_angle(percent);
            }
            COM1.RX_Cnt = 0;
        }
    }
}

int main(void) {

    EAXSFR();

    GPIO_config();
    UART_config();
    PWM_config();

    EA = 1;

    while (1) {
        on_uart1_receive();

        delay_ms(20);
    }

}