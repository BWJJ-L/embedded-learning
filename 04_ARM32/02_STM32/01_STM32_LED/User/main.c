#include "stm32f4xx.h"
#include "systick.h"
#include <stdio.h>
/*********************
任务目标:

模板工程

**********************/

#define LED1    GPIOD, GPIO_Pin_8

void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //============================================ 核心板 PB2
    // 1. 时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    // 2. 设置GPIO模式（STM32 标准库把「模式/类型/速度/上下拉」一次性写入结构体再初始化）
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;         // 引脚
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;      // 输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 频率
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;   // 无上拉也无下拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //============================================ 扩展板 LED总开关 PC6
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //============================================ 扩展板 LED灯组 PD8
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // 三极管导通  低电平  PC6
    GPIO_ResetBits(GPIOC, GPIO_Pin_6);
}

int main(void)
{
    // 系统滴答定时器初始化
    systick_config();

    // 引脚初始化配置
    GPIO_Config();

    while(1) {
        GPIO_SetBits(GPIOB, GPIO_Pin_2);     //核心板 PB2，设置高电平亮
        GPIO_ResetBits(LED1);   //扩展板 灯组，设置低电平亮
        delay_1ms(500);

        GPIO_ResetBits(GPIOB, GPIO_Pin_2);   //核心板 PB2，设置低电平灭
        GPIO_SetBits(LED1);     //扩展板 灯组，设置高电平灭
        delay_1ms(500);
    }
}