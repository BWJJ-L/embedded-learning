#include "main.h"
#include "systick.h"
#include <stdio.h>

void GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOB时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    // 配置PB4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      // 输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 高速
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   // 无上下拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


int main(void) {
	systick_config(); 
    
	GPIO_Config();
    
    while(1) {
        GPIO_ToggleBits(GPIOB, GPIO_Pin_4); // 翻转PB4状态
        delay_1ms(500);                     // 延时500ms
    }
}
