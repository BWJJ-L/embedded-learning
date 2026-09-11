#include "App.h"

#define MOTOR       P01

#define PREQ		1000

#define PERIOD      (MAIN_Fosc / PREQ)

static void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;     //结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_5;       //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_HighZ;   //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin  = GPIO_Pin_1;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化
}
/******************* AD配置函数 *******************/
static void    ADC_config(void) {
    ADC_InitTypeDef     ADC_InitStructure;      //结构定义

    ADC_InitStructure.ADC_SMPduty   = 31;       //ADC 模拟信号采样时间控制, 0~31（注意： SMPDUTY 一定不能设置小于 10）
    ADC_InitStructure.ADC_CsSetup   = 0;        //ADC 通道选择时间控制 0(默认),1
    ADC_InitStructure.ADC_CsHold    = 1;        //ADC 通道选择保持时间控制 0,1(默认),2,3
    ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X16T;       //设置 ADC 工作时钟频率 ADC_SPEED_2X1T~ADC_SPEED_2X16T
    ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;  //ADC结果调整,  ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
    ADC_Inilize(&ADC_InitStructure);        //初始化
    ADC_PowerControl(ENABLE);               //ADC电源开关, ENABLE或DISABLE
    NVIC_ADC_Init(DISABLE, Priority_0);     //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

static void	PWM_config(void)
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

static u16 adc; // 电位器ADC原始值, 范围0~4095

void task_6() _task_ TASK_Motor {
             
    PWMx_Duty pwm_duty;
    
    EAXSFR();

    GPIO_config();
    PWM_config();
    ADC_config();

    EA = 1;

    pwm_duty.PWM6_Duty = 0;
    UpdatePwm(PWM6, &pwm_duty);

    while (1) {
        adc = Get_ADCResult(ADC_CH13);    // 读P0.5上的电位器电压

        // 24000乘4095约9828万, 超出16位上限, 先按32位计算再截回16位
        pwm_duty.PWM6_Duty = PERIOD * adc / 2740;
        UpdatePwm(PWM6, &pwm_duty);

        printf("ADC: %d, duty: %d\n", (int)adc, (int)pwm_duty.PWM6_Duty);   // u16占两格, 正好配%u

        APP_I2C_OLED_Refresh(0);
        
        os_wait2(K_TMO, 40);
    }
}

void MOTOR_show() {
    char strbuff[30];
    sprintf(strbuff, "Duty:%6.2f%%",((adc / 2730.00) * 100));
    I2C_OLED_ShowString(0, 2, strbuff, 16);
}

void TASK_MOTOR_reset() {
    PWMB_CC6E_Disable();    //关闭PWM6的输入捕获/比较输出
    os_delete_task(TASK_Motor);
}