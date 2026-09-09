#include "Config.h"
#include "GPIO.h"
#include "NVIC.h"
#include "UART.h"
#include "Switch.h"
#include "ADC.h"
#include "Timer.h"
#include "STC8H_PWM.h"
#include "Delay.h"

#define LED_SW  P45
#define LED1    P27
#define LED2    P26
#define LED3    P15
#define LED4    P14
#define LED5    P23
#define LED6    P22
#define LED7    P21
#define LED8    P20

void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;     //结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_5;       //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//初始化

    // P20,P21,P22,P23,P26,P27
    GPIO_InitStructure.Pin  = GPIO_Pin_LOW | GPIO_Pin_6 | GPIO_Pin_7;       //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure);//初始化

    // P14, P15
    GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;      //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);//初始化

    // 初始化P30, P31
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;      //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
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

int main() {
    int8 i = 0;  //[-128, 127] 或使用int等有符号数

    // 引脚初始化
    GPIO_config();
    // 串口初始化
    UART_config();
    // 开启全局中断
    EA = 1;

    //1. 导通总开关: 拉低P45
    P45 = 0;

    //2. 熄灭所有灯
    LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;


    while (1) {

//        从右 -> 左
        for (i = 0; i < 4; i++) {

            switch (i) {
                case 0:
                    LED2 = 0;
                    delay_ms(250);
                    break;
                case 1:
                    LED4 = 0;
                    delay_ms(250);
                    break;
                case 2:
                    LED6 = 0;
                    delay_ms(250);
                    break;
                case 3:
                    LED8 = 0;
                    delay_ms(250);
                    break;
                default:
                    break;
            }


        }

        LED2  = LED4 =  LED6 =  LED8 = 1;
        delay_ms(250);
    }
}
