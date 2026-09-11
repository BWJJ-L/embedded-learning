#include "Config.h"
#include "GPIO.h"
#include "NVIC.h"
#include "Timer.h"

void GPIO_Config(void){
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_Pin_3;
    GPIO_InitStructure.Mode = GPIO_OUT_PP;
    
    GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);
}

void Timer_Config(void){
    TIM_InitTypeDef     TIM_InitStructure;
    TIM_InitStructure.TIM_Mode = TIM_16BitAutoReload;
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;
    TIM_InitStructure.TIM_ClkOut = DISABLE;
    TIM_InitStructure.TIM_Value = 65536UL - (MAIN_Fosc / 1000UL);
    TIM_InitStructure.TIM_Run = ENABLE;
    
    Timer_Inilize(Timer0,&TIM_InitStructure);
    
    NVIC_Timer0_Init(ENABLE,Priority_0);
}

void Timer0_CallBack(void){
    static int16 counter = 0;
    
    counter++;
    
    if(counter >= 1000){
        counter = 0;
        P53 = !P53;
    }
}

int main(void){
    GPIO_Config();
    
    Timer_Config();
    
    EA = 1;
    
    
    
    
    while(1) ;
}