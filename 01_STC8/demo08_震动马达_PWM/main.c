#include "Config.h"
#include "Delay.h"
#include "GPIO.h"
#include "STC8H_PWM.h"
#include "NVIC.h"
#include "Switch.h"

#define MOTOR P01

#define PREQ			1000

#define PERIOD (MAIN_Fosc / PREQ)

void GPIO_config(void) {
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_1;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化
}
void	PWM_config(void)
{
	PWMx_InitDefine		PWMx_InitStructure;

	PWMx_InitStructure.PWM_Mode    		=	CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    		=  PERIOD;	//PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect  = ENO6P;				//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM6, &PWMx_InitStructure);			//初始化PWM,  PWMA,PWMB

	PWMx_InitStructure.PWM_Period   = PERIOD - 1;					//周期时间,   0~65535
	PWMx_InitStructure.PWM_DeadTime = 0;					//死区发生器设置, 0~255
	PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//主输出使能, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//使能计数器, ENABLE,DISABLE
	PWM_Configuration(PWMB, &PWMx_InitStructure);			//初始化PWM通用寄存器,  PWMA,PWMB

	PWM6_SW(PWM6_SW_P01);					//PWM6_SW_P21,PWM6_SW_P54,PWM6_SW_P01,PWM6_SW_P75

	NVIC_PWM_Init(PWMB,DISABLE,Priority_0);
}

void main(){
    int i = 0,dir = 1;
    
	PWMx_Duty duty;
	u8 duty_percent = 0; // 0 - 100
	
	EAXSFR();
	
	GPIO_config();
	PWM_config();
	
	duty.PWM6_Duty = 0;
	UpdatePwm(PWM6, &duty);
	while(1){
		delay_ms(10);
		
		// 设置占空比
        duty_percent += dir;
        
        //低速转高速-》再转成低速
//        if(100 == duty_percent) dir = -1;
//        else if(0 == duty_percent) dir = 1;
        //高速转低速
        if(duty_percent > 0) dir = -1;
        else if(0 == duty_percent) duty_percent = 100;
        
        
		duty.PWM6_Duty = PERIOD * duty_percent / 100;
		UpdatePwm(PWM6, &duty);
        
        delay_ms(20);
        
	}
}