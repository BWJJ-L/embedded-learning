#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "STC8H_PWM.h"

#include "Light.h"
#include "Key.h"
#include "Battery.h"
#include "Buzzer.h"
#include "Ultrasonic.h"
#include "Motors.h"


#define TASK_LIGHT  1
#define TASK_KEY    2


void GPIO_config(void) {
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
}

void UART_config(void) {
	// >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine		COMx_InitStructure;					//结构定义
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx; 	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		    //初始化串口1 UART1,UART2,UART3,UART4

  	NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

void sys_init() {
    EAXSFR();
    
    GPIO_config();
    UART_config();
    
    //硬件驱动初始化
//    Light_init();
    Key_init();
    Battery_init();
    Buzzer_init();
    Ultrasonic_init();
    Motors_init();
    
    EA = 1;
}

void main_task() _task_ 0 {
    sys_init();
    
    os_create_task(TASK_LIGHT);
    
    os_create_task(TASK_KEY);
    
    os_delete_task(0);
}

void task_1() _task_ TASK_LIGHT {
    while(1) {
        Light_on(LEFT);
        os_wait2(K_TMO, 200);
        Light_off(LEFT);
        os_wait2(K_TMO, 200);
        
        Light_on(RIGHT);
        os_wait2(K_TMO, 200);
        Light_off(RIGHT);
        os_wait2(K_TMO, 200);
        
        Light_on(ALL);
        os_wait2(K_TMO, 200);
        Light_off(ALL);
        os_wait2(K_TMO, 200);
    }
}

u8 flag = 0;
char speed = 20;
void Key_on_keydown() {
    float vol;
    float distance;
    char ret;

    
    printf("DOWN\n");
    
    
    //测电压
    vol = Battery_get_voltage();
    printf("vol : %.2f\n",vol);
    
    
    //蜂鸣器-1首两只老虎
//    Buzzer_demo_2tiger();
    //蜂鸣器-哔哔哔
//    Buzzer_Alarm();
    
    
    //超声波测距
    ret = Ultrasonic_get_distance(&distance);
    if(!ret) {
        printf("距离：%.2f\n", distance);
    } else {
        printf("错误码：%d\n",(int)ret);
    }
    
    
    //DC电机驱动
        
//    switch(flag) {                          //左前   直前   右前
//        case 0:
//            printf("----方向1\n");
//            Motors_forward(speed, LEFT_M);
//            break;
//        case 1:
//            printf("----方向2\n");
//            Motors_forward(speed, MID_M);
//            break;
//        case 2:
//            printf("----方向3\n");
//            Motors_forward(speed, RIGHT_M);
//        default: break;
//    }
        
//    switch(flag) {                          // 左退   直退   右退
//        case 0:
//            printf("----方向1\n");
//            Motors_backward(speed, LEFT_M);
//            break;
//        case 1:
//            printf("----方向2\n");
//            Motors_backward(speed, MID_M);
//            break;
//        case 2:
//            printf("----方向3\n");
//            Motors_backward(speed, RIGHT_M);
//        default: break;
//    }
        
//    switch(flag) {                          // 向左平移  向右平移
//        case 0:
//            printf("----方向1\n");
//            Motors_translate(speed, LEFT_M);
//            break;
//        case 1:
//            printf("----方向2\n");
//            Motors_translate(speed, RIGHT_M);
//            break;
//        default: break;
//    }
        
    switch(flag) {                        // 顺时针原地旋转    逆时针原地旋转
        case 0:
            printf("----方向1\n");
            Motors_around(speed, LEFT_M);
            break;
        case 1:
            printf("----方向2\n");
            Motors_around(speed, RIGHT_M);
            break;
        default: break;
    }
    
//    switch(flag) {                    //  向左转  向右转
//        case 0:
//            printf("----方向1\n");
//            Motors_turn(speed, LEFT_M);
//            break;
//        case 1:
//            printf("----方向2\n");
//            Motors_turn(speed, RIGHT_M);
//            break;
//        default: break;
//    }
    
    flag++;
    
    if(flag > 2) flag = 0;
}
void Key_on_keyup(){
    printf("UP");
}

void task_2() _task_ TASK_KEY {
    while(1) {
        Key_scan(Key_on_keydown, NULL);
        os_wait2(K_TMO, 2);
    }
}