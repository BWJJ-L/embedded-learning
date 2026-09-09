#include "Config.h"
#include "GPIO.h"
#include "NVIC.h"
#include "UART.h"
#include "Switch.h"
#include "Timer.h"
#include "STC8H_PWM.h"
#include "Delay.h"

#include "BUZZER.h"

#define BUZZER_GPIO     P00

void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);

    GPIO_InitStructure.Pin  = GPIO_Pin_1;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);
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

int main(void) {
    u8 tone = 1;

    EAXSFR();

    GPIO_config();
    UART_config();
    
    Buzzer_Init();

    EA = 1;

    while (1) {
        if (P51 == 0) {
            
            Buzzer_Beep(tone);
            
            printf("tone->%bu\r\n",tone);
            
            if(++tone > 8) tone = 1;
            
            delay_ms(250);
            
            Buzzer_Stop();
            delay_ms(250);
            
        } else if (P51 == 1) {
            Buzzer_Stop();
        }
    }
}