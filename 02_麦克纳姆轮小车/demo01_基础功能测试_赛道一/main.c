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
#include "Track.h"


#define TASK_LIGHT  1
#define TASK_KEY    2
#define TASK_TRACK  3
#define TASK_UART1  4
#define TASK_UART2  5


void GPIO_config(void) {
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
    
    P1_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);
}

void UART_config(void) {
	// >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine		COMx_InitStructure;					//结构定义
    //================UART 1==========================
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx; 	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		    //初始化串口1 UART1,UART2,UART3,UART4

  	NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
    
    
    //================UART 2==========================
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx; 	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART2, &COMx_InitStructure);		    //初始化串口1 UART1,UART2,UART3,UART4

  	NVIC_UART2_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART2_SW(UART2_SW_P10_P11);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

void sys_init() {
    EAXSFR();
    
    GPIO_config();
    UART_config();
    
    //硬件驱动初始化
    Light_init();
    Key_init();
    Battery_init();
    Buzzer_init();
    Ultrasonic_init();
    Motors_init();
    Track_init();
    
    EA = 1;
}

void main_task() _task_ 0 {
    sys_init();
    
//    os_create_task(TASK_LIGHT);
    
    os_create_task(TASK_KEY);
    
    os_create_task(TASK_UART1);
    os_create_task(TASK_UART2);
    
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

//    switch(flag) {                        // 顺时针原地旋转    逆时针原地旋转
//        case 0:
//            printf("----方向1\n");
//            Motors_around(speed, LEFT_M);
//            break;
//        case 1:
//            printf("----方向2\n");
//            Motors_around(speed, RIGHT_M);
//            break;
//        default: break;
//    }

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
    
    
    switch(flag){
        case 0:
            os_create_task(TASK_TRACK);
            break;
        case 1:
            os_delete_task(TASK_TRACK);
            Motors_stop();
            break;
        default: break;
    }
    
    flag++;
    
    if(flag > 1) flag = 0;
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

void task_3() _task_ TASK_TRACK {
    char speed = 20;
    int pos = 0;
    
    while(1){
        pos = Track_get_position();
        if(pos == -64){
            Motors_turn(speed+10, LEFT_M);
        } else if(pos == -48){
            Motors_turn(speed+15, LEFT_M);
        } else if(pos == -32){
            Motors_turn(speed+15, LEFT_M);
        } else if(pos == -21){
            Motors_turn(speed+15, LEFT_M);
        } else if(pos == -16){
            Motors_forward(speed+30, MID_M);
        } else if(pos == -10){
            Motors_forward(speed+30, MID_M);
        } else if(pos == -8){
            Motors_forward(speed+30, MID_M);
        } else if(pos == 0){
            Motors_forward(speed+30, MID_M);
        } else if(pos == 8){
            Motors_forward(speed+30, MID_M);
        } else if(pos == 10){
            Motors_forward(speed+30, MID_M);
        } else if(pos == 16){
            Motors_forward(speed+30, MID_M);
        } else if(pos == 21){
            Motors_turn(speed+15, RIGHT_M);
        } else if(pos == 32){
            Motors_turn(speed+15, RIGHT_M);
        } else if(pos == 48){
            Motors_turn(speed+15, RIGHT_M);
        } else if(pos == 64){
            Motors_turn(speed+10, RIGHT_M);
        }
        
//        os_wait2(K_TMO, 100);
    }
}

void task_4() _task_ TASK_UART1 {
    u8 i;
    while(1) {
        if(COM1.RX_TimeOut > 0) {
            //超时计数
            if(--COM1.RX_TimeOut == 0) {
                if(COM1.RX_Cnt > 0) {
                    // 这里处理收到的数据，做具体的逻辑，可以调用自己的on_uart1_recv
                    for(i=0; i<COM1.RX_Cnt; i++)	{
                        // RX1_Buffer[i]存的是接收的每个字节，写出用 TX1_write2buff
                        TX2_write2buff(RX1_Buffer[i]);// 写一个byte
                }
            }
            COM1.RX_Cnt = 0;
        }
    }

    // 不要处理的太快
    os_wait2(K_TMO, 1);
    }
}

/*****************************************
 0	1   2  3  4  5  6  7  数组索引
 帧头   x  y  A  B  C  D
DD 77  EF 50 01 00 01 00  数据示例

帧头 01：DD 77
方向 23：EF 50
按钮 4567: ABCD  按下01 抬起00

A: 蜂鸣器/车灯
B: 左旋转: 按下开始转,抬起停止转
C: 右旋转: 按下开始转,抬起停止转
D: 开启/关闭巡线

1. A键: 蜂鸣器/车灯 (边缘触发：只在按下的瞬间执行一次)
2. D键: 开启/关闭巡线 (边缘触发)
3. 互斥锁：如果开启了巡线，屏蔽手动驾驶，直接退出
4. 运动控制 (无巡线时生效)
    B/C键: 旋转 (电平触发：按住持续生效)
    摇杆控制: 只有在没有按下旋转按键时，摇杆才生效
*****************************************/

void do_work_app(){
    u8 *buf = RX2_Buffer;
    char x, y;
    
    static u8 led_flag = 0;
    static u8 is_turning = 0;
    static u8 is_tracking = 0;
    
    u8 cur_A = buf[4];
	u8 cur_B = buf[5];
	u8 cur_C = buf[6];
	u8 cur_D = buf[7];
    
    
    if(buf[0] != 0xDD || buf[1] != 0x77){
        return ;
    }
    
    //1. A键: 蜂鸣器/车灯 (边缘触发：只在按下的瞬间执行一次)
    if (cur_A == 1) { 
//		Buzzer_alarm();
		if (led_flag == 0) { // 原来是灭，需要开灯
			Light_on(ALL);
		} else { // 原来是亮的，需关灯
			Light_off(ALL);
		}
		led_flag = !led_flag; // 标志位反转
	}
    
    //2. D键: 开启/关闭巡线 (边缘触发)
    if(cur_D){
        if(is_tracking == 0){
            os_create_task(TASK_TRACK);
        } else {
            os_delete_task(TASK_TRACK);
            Motors_stop();
        }
        is_tracking = !is_tracking;
    }
    
    //3. 互斥锁：如果开启了巡线，屏蔽手动驾驶，直接退出
    if(is_tracking) return;
    
    //4. 运动控制 (无巡线时生效)
    //B/C键: 旋转 (电平触发：按住持续生效)
    if(cur_B && !is_turning){
        Motors_around(50, LEFT_M);
        is_turning = !is_turning;   //必须加上标志位反转
    }else if(cur_C && !is_turning){
        Motors_around(50, RIGHT_M);
        is_turning = !is_turning;   //必须加上标志位反转
    }
    if(!cur_B && !cur_C && is_turning){ //正在转向且已经抬起按钮，才停止马达
        is_turning = !is_turning;
        Motors_stop();
    }
    
    //摇杆控制: 只有在没有按下旋转按键时，摇杆才生效
    if(!cur_A && !cur_B && !cur_C && !cur_D){
        x = buf[2], y = buf[3];
        UART_move(x, y);
    }
}


void task_5() _task_ TASK_UART2 {
    while(1) {
        if(COM2.RX_TimeOut > 0) {
            //超时计数
            if(--COM2.RX_TimeOut == 0) {
                if(COM2.RX_Cnt > 0) {
                    
                    do_work_app();
                    
                    
//                    // 这里处理收到的数据，做具体的逻辑，可以调用自己的on_uart1_recv
//                    for(i=0; i<COM2.RX_Cnt; i++)	{
//                        // RX1_Buffer[i]存的是接收的每个字节，写出用 TX1_write2buff
//                        TX1_write2buff(RX2_Buffer[i]);// 写一个byte
//                    }
            }
            COM2.RX_Cnt = 0;
        }
    }

    // 不要处理的太快
    os_wait2(K_TMO, 1);
    }
}

