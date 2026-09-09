#include "App.h"
#include "NIXIE.h"

static void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;						//结构定义
	//定时器2做16位自动重装, 中断频率为1000HZ
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000);				//初值
	TIM_InitStructure.TIM_PS        = 0;					//8位预分频器(n+1), 0~255, (注意:并非所有系列都有此寄存器,详情请查看数据手册)
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer2,&TIM_InitStructure);				//初始化Timer2	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer2_Init(ENABLE,NULL);		//中断使能, ENABLE/DISABLE; 无优先级
}

static u8 number[8] = {0, 0, 2, 3, 4, 5, 6, 7};
static u8 num = 00;
static void KEY_scan(u8 row, u8 col) {
    
    if(row == 0 && col == 0) {
        if(num >= 90) {
            num -= 90;
        } else {
            num += 10;
        }
        
    } else if(row == 0 && col == 1) {
        if(num == 99) {
            num = 0;
        } else {
            num += 1;
        }
        
    } else if(row == 1 && col == 0) {
        if(num <= 10) {
            num += 90;
        } else {
            num -= 10;
        }
    } else if(row == 1 && col == 1) {
        if(num == 0) {
            num = 99;
        } else {
            num -= 1;
        }
    }
    
    number[0] = num / 10;
    number[1] = num % 10;
}

void Timer2_callback(){ // 1ms被调一次
    static u8 i = 0;
    
    NIXIE_display(number[i], i); // 1ms (0,1,2,3,4,5,6,7)
    
    i = (i + 1) % 8;
}

void task_8() _task_ TASK_NIXIE {
    NIXIE_init();
    MK_init();
    Timer_config();
    
    while (1) {
        MK_key_state(KEY_scan);
        os_wait2(K_TMO, 1);
        
        APP_I2C_OLED_Refresh(0);
    }
}

void NixieDigital_show() {
    u8 i = 0;
    
    I2C_OLED_ShowString(0, 2, "NIXIE : ", 16);
    
    for(i = 0;i < 8;i++) {
        I2C_OLED_ShowNum(i * 8 + 64, 2, number[i], 1, 16);
    }
}

void TASK_NIXIE_reset(){
    Timer2_Run(DISABLE);
    NIXIE_show(0xFF, 0x00);
    os_delete_task(TASK_NIXIE);
}