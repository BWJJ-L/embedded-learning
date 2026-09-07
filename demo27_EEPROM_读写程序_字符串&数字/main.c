#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "Delay.h"
#include "EEPROM.h"
#include <string.h>

extern void delay_X_ms(u16 ms);

void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	    //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
}

void UART_config(void) {
    // >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine     COMx_InitStructure;                 //结构定义
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx; //模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;         //选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;           //波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;             //接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;            //波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);     //初始化串口1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE, Priority_1);    //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);     // 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

char strBuff[20] = {0};

int main(void) {
    u16 address = 0x0001;
    char* str = "Hello Wrold!";
    u8 len = strlen(str);
    
    EAXSFR();

    GPIO_config();
    UART_config();

    EA = 1;
    
    delay_X_ms(2000);
    
    printf("Init Complete\n");
    
    EEPROM_read_n(address, str, len);
    printf("写入前 : strBuff->%s\n",strBuff);
    
    
    EEPROM_SectorErase(address);
    EEPROM_write_n(address, str, len);
    EEPROM_read_n(address, strBuff, len);
    
    printf("写入后 : strBuff->%s\n",strBuff);
    printf("写入后 : str->%s\n",str);
    
    if(!strcmp(str, strBuff)) {
        printf("对比成功");
    } else {
        printf("对比失败");
    }

    while (1) {
        
    }
}