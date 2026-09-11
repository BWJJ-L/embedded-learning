#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "Delay.h"
#include "MATRIXKEY.h"
#include "Keys.h"

void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;      //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
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

void KEY_scan(u8 row, u8 col, u8 state) {
    static u8 position;
    position = row * COL_NUM + col + 1;
    if(state) {
        printf("key%d %d行%d列 抬起\n",(int)(position), (int)(row + 1), (int)(col + 1));
    } else {
        printf("key%d %d行%d列 按下\n",(int)(position), (int)(row + 1), (int)(col + 1));
    }
}

u8 Light_P53 = 1;

void Keys_on_keyup(u8 key_index) {
    printf("key%d 抬起\n", (int)key_index);
}
void Keys_on_keydown(u8 key_index) {
    printf("key%d 按下\n", (int)key_index);
    if(key_index == 0) {
        Light_P53 = 1;
    } else if(key_index == 2) {
        Light_P53 = 0;
    }
}

int main(void) {

    EAXSFR();

    GPIO_config();
    UART_config();
    MK_init();
    Keys_init();

    EA = 1;
    
    printf("init commit\n");

    while (1) {
        // 4x4矩阵按钮扫描
        MK_key_state(KEY_scan);
        
        
        // P53按钮和灯复用
        P5_MODE_IN_HIZ(GPIO_Pin_3);
        P5_PULL_UP_ENABLE(GPIO_Pin_3);
        Keys_scan();
        
        P5_MODE_OUT_PP(GPIO_Pin_3);
        P53 = Light_P53;

        delay_ms(10);
    }
}