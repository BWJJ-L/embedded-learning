#include "Config.h"
#include "GPIO.h"
#include "NVIC.h"
#include "UART.h"
#include "Switch.h"
#include "ADC.h"
#include "Timer.h"
#include "STC8H_PWM.h"
#include "Delay.h"

#define MOTOR P01

#define PREQ            1000

#define PERIOD (MAIN_Fosc / PREQ)

void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;     //结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_1;       //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_1;       //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);//初始化
}

void UART_config(void) {
    COMx_InitDefine     COMx_InitStructure;
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;
    COMx_InitStructure.UART_BaudRate  = 115200ul;
    COMx_InitStructure.UART_RxEnable  = ENABLE;
    COMx_InitStructure.BaudRateDouble = DISABLE;
    UART_Configuration(UART1, &COMx_InitStructure);

    NVIC_UART1_Init(ENABLE, Priority_1);
    UART1_SW(UART1_SW_P30_P31);
}

void    PWM_config(void) {
    PWMx_InitDefine     PWMx_InitStructure;

    PWMx_InitStructure.PWM_Mode         =   CCMRn_PWM_MODE1;    //模式,     CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty         =   PERIOD; //PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect  = ENO6P;              //输出通道选择, ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM6, &PWMx_InitStructure);           //初始化PWM,  PWMA,PWMB

    PWMx_InitStructure.PWM_Period   = PERIOD - 1;                   //周期时间,   0~65535
    PWMx_InitStructure.PWM_DeadTime = 0;                    //死区发生器设置, 0~255
    PWMx_InitStructure.PWM_MainOutEnable = ENABLE;          //主输出使能, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;           //使能计数器, ENABLE,DISABLE
    PWM_Configuration(PWMB, &PWMx_InitStructure);           //初始化PWM通用寄存器,  PWMA,PWMB

    PWM6_SW(PWM6_SW_P01);                   //PWM6_SW_P21,PWM6_SW_P54,PWM6_SW_P01,PWM6_SW_P75

    NVIC_PWM_Init(PWMB, DISABLE, Priority_0);
}

void main() {
    int i = 0, dir = -1;
    PWMx_Duty duty;
    u8 duty_percent = 100;

    EAXSFR();

    GPIO_config();
    PWM_config();


    // 0 - 100
    duty.PWM6_Duty = PERIOD * duty_percent / 100;
    UpdatePwm(PWM6, &duty);
    delay_X_ms(2000);

    while (duty_percent > 20) {
        duty_percent += dir;
        duty.PWM6_Duty = PERIOD * duty_percent / 100;
        UpdatePwm(PWM6, &duty);
        delay_ms(7);
    }
    delay_X_ms(1000);
    
    PWMB_CC6E_Disable();

    while (1) {

    }
}