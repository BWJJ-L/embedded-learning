#include "BUZZER.h"

u16 code Hz[] = {523, 587, 659, 698, 784, 880, 988, 1047};

static void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;     //结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_0;       //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化
}

void PWM_config() {
    PWMx_InitDefine		PWMx_InitStructure;
	
    // 配置PWM5
    PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty   	 	= 0;  //(u16)(Period * 0.05f);	//PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect    = ENO5P;			//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM5, &PWMx_InitStructure);			//初始化PWM,  PWMA,PWMB

    // 配置PWMB
    PWMx_InitStructure.PWM_Period   = MAIN_Fosc / 1000UL;//Period - 1;			//周期时间,   0~65535
    PWMx_InitStructure.PWM_DeadTime = 0;					//死区发生器设置, 0~255
    PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//主输出使能, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//使能计数器, ENABLE,DISABLE
    PWM_Configuration(PWMB, &PWMx_InitStructure);			//初始化PWM通用寄存器,  PWMA,PWMB

    // 切换PWM通道
    PWM5_SW(PWM5_SW_P00);					//PWM5_SW_P20,PWM5_SW_P17,PWM5_SW_P00,PWM5_SW_P74

    // 初始化PWMB的中断
    NVIC_PWM_Init(PWMB,DISABLE,Priority_0);
}

void Buzzer_Init(void) {
    EAXSFR();
    GPIO_config();
    PWM_config();
}

void Buzzer_Play(u16 Hz_Value) {
    u16 Period = MAIN_Fosc / Hz_Value;
    u16 duty = Period * 0.05f;
    
    PWMB_Duty5(duty);
    
    PWMB_AutoReload(Period - 1);
    
    PWMB_CC5E_Enable();
}

void Buzzer_Beep(u8 tone) {
    Buzzer_Play(Hz[tone-1]);
}

void Buzzer_Stop() {
    PWMB_CC5E_Disable();
}