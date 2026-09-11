#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "Delay.h"
#include "NVIC.h"
#include "Switch.h"
#include "STC8H_PWM.h"

#define LED_SW	    P45
#define LED1		P27
#define LED2		P26
#define LED3		P15
#define LED4		P14
#define LED5		P23
#define LED6		P22
#define LED7		P21
#define LED8		P20

void GPIO_config(void) {
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
    // LED_SW
    GPIO_InitStructure.Pin  = GPIO_Pin_5;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//初始化
    
    GPIO_InitStructure.Pin  = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);
}

void UART_config(void){
    COMx_InitDefine COMx_InitStructure;
    COMx_InitStructure.UART_Mode = UART_8bit_BRTx;
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;
    COMx_InitStructure.UART_BaudRate = 115200ul;
    COMx_InitStructure.UART_RxEnable  = ENABLE;
    COMx_InitStructure.BaudRateDouble = DISABLE;
    UART_Configuration(UART1, &COMx_InitStructure);
    
    NVIC_UART1_Init(ENABLE,Priority_1);
    
    UART1_SW(UART1_SW_P30_P31);
}

#define FREQ    1000

#define PERIOD (MAIN_Fosc / FREQ)

PWMx_Duty dutyA;

void PWM_config(void){
    PWMx_InitDefine PWMx_InitStructure;
    
    PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1;
    PWMx_InitStructure.PWM_Duty = dutyA.PWM1_Duty;
    PWMx_InitStructure.PWM_EnoSelect = ENO1P | ENO1N;
    PWM_Configuration(PWM1,&PWMx_InitStructure);
    
    PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1;
    PWMx_InitStructure.PWM_Duty = dutyA.PWM2_Duty;
    PWMx_InitStructure.PWM_EnoSelect = ENO2P | ENO2N;
    PWM_Configuration(PWM2,&PWMx_InitStructure);
    
    PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1;
    PWMx_InitStructure.PWM_Duty = dutyA.PWM3_Duty;
    PWMx_InitStructure.PWM_EnoSelect = ENO3P | ENO3N;
    PWM_Configuration(PWM3,&PWMx_InitStructure);
    
    PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1;
    PWMx_InitStructure.PWM_Duty = dutyA.PWM4_Duty;
    PWMx_InitStructure.PWM_EnoSelect = ENO4P | ENO4N;
    PWM_Configuration(PWM4,&PWMx_InitStructure);
    
    PWMx_InitStructure.PWM_Period        = PERIOD - 1;
    PWMx_InitStructure.PWM_DeadTime      = 0;
    PWMx_InitStructure.PWM_CEN_Enable    = ENABLE;
    PWMx_InitStructure.PWM_MainOutEnable = ENABLE;
    
    PWM_Configuration(PWMA,&PWMx_InitStructure);
    
    PWM1_SW(PWM1_SW_P20_P21);
    PWM2_SW(PWM2_SW_P22_P23);
    PWM3_SW(PWM3_SW_P14_P15);
    PWM4_SW(PWM4_SW_P26_P27);
    
    NVIC_PWM_Init(PWMA,DISABLE,Priority_0);
    
}

int duty_Percent = 0;
void on_uart1_recv(void){
    
    if(0x00 == RX1_Buffer[0]){
        duty_Percent++;
    }else if(0x01 == RX1_Buffer[0]){
        duty_Percent--;
    }
    
    if(duty_Percent >= 100){
        duty_Percent = 100;
    }else if(duty_Percent <= 0){
        duty_Percent = 0;
    }
    
    dutyA.PWM1_Duty = PERIOD * duty_Percent / 100;
    dutyA.PWM2_Duty = PERIOD * duty_Percent / 100;
    dutyA.PWM3_Duty = PERIOD * duty_Percent / 100;
    dutyA.PWM4_Duty = PERIOD * duty_Percent / 100;
    
    UpdatePwm(PWMA,&dutyA);
}

int main(void){
    EAXSFR();
    
    GPIO_config();
    
    UART_config();
    
    PWM_config();
    
    EA = 1;
    
    LED_SW = 0;
    
    while(1) { // duty -> 100% -> 50% -> 25% -> 10% -> 0%
        // duty_percent = [0, 1, 2.... 98, 99, 100, 0, 1, 2.... 98, 99, 100]
        // duty_percent = [0, 1, 2.... 98, 99, 100, 99, 98, ..... 2, 1, 0, 1....]
        
        if(COM1.RX_TimeOut > 0) {
            //超时计数
            if(--COM1.RX_TimeOut == 0) {
                if(COM1.RX_Cnt > 0) {
                    on_uart1_recv();
                }
                COM1.RX_Cnt = 0;
            }
        }

        // 不要处理的太快
        delay_ms(1);
        
    }
}