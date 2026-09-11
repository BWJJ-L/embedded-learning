#include "Motors.h"

void Motors_init(){
    //马达引脚初始化
    P1_MODE_OUT_PP(GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
    P2_MODE_OUT_PP(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
    
    // 全部先设置为低
    LF_P = LF_N = RF_P = RF_N = LB_P = LB_N = RB_P = RB_N = 0;
}

static char speedDuty(char speed) {
    // 传入-100 -> 返回0
    // 传入 0   -> 返回50
    // 传入 100 -> 返回100
    return speed / 2 + 50;
}

#define PERIOD (MAIN_Fosc / 1000)
static void	PWM_config(MotorSpeed ms)
{
    PWMx_InitDefine		PWMx_InitStructure;
		
	// 配置PWM1
    // 右后轮
	PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    		= (speedDuty(ms.RB_Speed) / 100.00) * PERIOD;	//PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect	= ((-10 <= ms.RB_Speed && ms.RB_Speed <= 10) ? 0 : (ENO1P | ENO1N));	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM1, &PWMx_InitStructure);			//初始化PWM
	// 配置PWM2
    // 左后轮
	PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    		= (speedDuty(ms.LB_Speed) / 100.00) * PERIOD;	//PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect	= ((-10 <= ms.LB_Speed && ms.LB_Speed <= 10) ? 0 : (ENO2P | ENO2N));	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM2, &PWMx_InitStructure);			//初始化PWM
	// 配置PWM3
    //右前轮
    //占空比: 0% - 50%   逆时针转
    //       50% - 0%    顺时针转
	PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    		= (speedDuty(ms.RF_Speed) / 100.00) * PERIOD;    //PWM占空比时间, 0~Period
	//PWMx_InitStructure.PWM_EnoSelect    = ENO3P | ENO3N;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	//在50%压差左右就关闭通道输出
    PWMx_InitStructure.PWM_EnoSelect    = ((-10 <= ms.RF_Speed && ms.RF_Speed <= 10) ? 0 : (ENO3P | ENO3N));
    PWM_Configuration(PWM3, &PWMx_InitStructure);
	// 配置PWM4
    // 左前轮
    PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    		= (speedDuty(ms.LF_Speed) / 100.00) * PERIOD;	//PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect	= ((-10 <= ms.LF_Speed && ms.LF_Speed <= 10) ? 0 : (ENO4P | ENO4N));	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM4, &PWMx_InitStructure);

	// 配置PWMA
    PWMx_InitStructure.PWM_Period   = PERIOD - 1;			//周期时间,   0~65535
    PWMx_InitStructure.PWM_DeadTime = 0;					//死区发生器设置, 0~255
    PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//主输出使能, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//使能计数器, ENABLE,DISABLE
    PWM_Configuration(PWMA, &PWMx_InitStructure);			//初始化PWM通用寄存器,  PWMA,PWMB

	// 切换PWM通道
	PWM1_SW(PWM1_SW_P20_P21);			//PWM1_SW_P10_P11,PWM1_SW_P20_P21,PWM1_SW_P60_P61
	PWM2_SW(PWM2_SW_P22_P23);			//PWM2_SW_P12_P13,PWM2_SW_P22_P23,PWM2_SW_P62_P63
	PWM3_SW(PWM3_SW_P14_P15);			//PWM3_SW_P14_P15,PWM3_SW_P24_P25,PWM3_SW_P64_P65
	PWM4_SW(PWM4_SW_P16_P17);			//PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

	// 初始化PWMA的中断
    NVIC_PWM_Init(PWMA,DISABLE,Priority_0);
}

// speed：速度 0~100  mode： LEFT_M左前 , MID_M前进 , RIGHT_M右前
void Motors_forward(char speed, MotorMode mode) {
    MotorSpeed ms = {0};
    
    if(LEFT_M == mode) {
        ms.LF_Speed = 0;
        ms.RF_Speed = speed;
        ms.LB_Speed = speed;
        ms.RB_Speed = 0;
    } else if(RIGHT_M == mode) {
        ms.LF_Speed = speed;
        ms.RF_Speed = 0;
        ms.LB_Speed = 0;
        ms.RB_Speed = speed;
    } else {
        ms.LF_Speed = speed;
        ms.RF_Speed = speed;
        ms.LB_Speed = speed;
        ms.RB_Speed = speed;
    }
    
    PWM_config(ms);
}

// speed：速度 0~100  mode： LEFT_M左后 , MID_M后退 , RIGHT_M右后
void Motors_backward(char speed , MotorMode mode) {
    MotorSpeed ms = {0};
    
    if(LEFT_M == mode) {
        ms.LF_Speed = -speed;
        ms.RF_Speed = 0;
        ms.LB_Speed = 0;
        ms.RB_Speed = -speed;
    } else if(RIGHT_M == mode) {
        ms.LF_Speed = 0;
        ms.RF_Speed = -speed;
        ms.LB_Speed = -speed;
        ms.RB_Speed = 0;
    } else {
        ms.LF_Speed = -speed;
        ms.RF_Speed = -speed;
        ms.LB_Speed = -speed;
        ms.RB_Speed = -speed;
    }
    
    PWM_config(ms);
}

// speed：速度 0~100  mode： LEFT_M左平移 ，RIGHT_M右平移
void Motors_translate(char speed , MotorMode mode) {
    MotorSpeed ms = {0};
    
    if(LEFT_M == mode) {
        ms.LF_Speed = -speed;
        ms.RF_Speed = speed;
        ms.LB_Speed = speed;
        ms.RB_Speed = -speed;
    } else if(RIGHT_M == mode) {
        ms.LF_Speed = speed;
        ms.RF_Speed = -speed;
        ms.LB_Speed = -speed;
        ms.RB_Speed = speed;
    }
    
    PWM_config(ms);
}

// 顺时针 (Clockwise): 想象一个时钟，指针从12点走向1点、2点、3点。在时钟的上半部分，指针是向右移动的。所以“向右转”就是顺时针。
// 逆时针 (Counter-clockwise): 与时钟指针相反的方向，从12点走向11点、10点。在时钟的上半部分，指针是向左移动的。所以“向左转”就是逆时针。
// speed：速度 0~100  mode： LEFT_M向左旋转(逆时针) , RIGHT_M向右旋转(顺时针)
void Motors_around(char speed , MotorMode mode) {
    MotorSpeed ms = {0};
    
    if(LEFT_M == mode) {
        ms.LF_Speed = speed;
        ms.RF_Speed = -speed;
        ms.LB_Speed = speed;
        ms.RB_Speed = -speed;
    } else if(RIGHT_M == mode) {
        ms.LF_Speed = -speed;
        ms.RF_Speed = speed;
        ms.LB_Speed = -speed;
        ms.RB_Speed = speed;
    }
    
    PWM_config(ms);
}

// speed：速度 0~100  mode： LEFT_M左转 , RIGHT_M右转
void Motors_turn(char speed ,  MotorMode mode) {
    MotorSpeed ms = {0};
    
    if(LEFT_M == mode) {
        ms.LF_Speed = speed;
        ms.RF_Speed = speed * 2;
        ms.LB_Speed = speed;
        ms.RB_Speed = speed * 2;
    } else if(RIGHT_M == mode) {
        ms.LF_Speed = speed * 2;
        ms.RF_Speed = speed;
        ms.LB_Speed = speed * 2;
        ms.RB_Speed = speed;
    }
    
    PWM_config(ms);
}

void Motors_stop(){
    MotorSpeed ms = {0};
    PWM_config(ms);
}