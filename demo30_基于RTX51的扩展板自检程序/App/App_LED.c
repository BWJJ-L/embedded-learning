#include "App.h"

#define LED_SW  P45
#define LED1    P27
#define LED2    P26
#define LED3    P15
#define LED4    P14
#define LED5    P23
#define LED6    P22
#define LED7    P21
#define LED8    P20

static void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;     //结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_5;       //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//初始化

    // P20,P21,P22,P23,P26,P27
    GPIO_InitStructure.Pin  = GPIO_Pin_LOW | GPIO_Pin_6 | GPIO_Pin_7;       //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure);//初始化

    // P14, P15
    GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;      //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);//初始化
}

static int LED_cnt = 0;

void task_5() _task_ TASK_LED {
    int8 i = 0;  //[-128, 127] 或使用int等有符号数

    // 引脚初始化
    GPIO_config();

    //1. 导通总开关: 拉低P45
    LED_SW = 0;

    //2. 熄灭所有灯
    LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;


    while (1) {
//      从右 -> 左
        for (i = 0; i < 8; i++) { // 0,1,2,3,4,5,6,7,8
            switch (i) {
                case 0:LED1 = 0; break;
                case 1:LED2 = 0; break;
                case 2:LED3 = 0; break;
                case 3:LED4 = 0; break;
                case 4:LED5 = 0; break;
                case 5:LED6 = 0; break;
                case 6:LED7 = 0; break;
                case 7:LED8 = 0; break;
                default: break;
            }
            
            LED_cnt = i;
            APP_I2C_OLED_Refresh(0);
            
            os_wait2(K_TMO, 25);
        }
        
        LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;
        os_wait2(K_TMO, 255);
        
//      从左 -> 右
        for(i = 7; i >= 0; i--){ // 0,1,2,3,4,5,6,7,8
            switch (i) {
                case 0:LED1 = 0; break;
                case 1:LED2 = 0; break;
                case 2:LED3 = 0; break;
                case 3:LED4 = 0; break;
                case 4:LED5 = 0; break;
                case 5:LED6 = 0; break;
                case 6:LED7 = 0; break;
                case 7:LED8 = 0; break;
                default: break;
            }
            LED_cnt = i;
            APP_I2C_OLED_Refresh(0);
            
            os_wait2(K_TMO, 25);
        }
        
        LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;
        os_wait2(K_TMO, 255);
        
    }
}

//切换任务时，需要结束LED任务
void TASK_LED_reset() {
    LED_SW = 1;
    os_delete_task(TASK_LED);
}
//LED模块需要在I2C屏幕上显示的内容
void LED_show() {
    char strbuff[20];
    sprintf(strbuff, "LED_CNT : %d", LED_cnt);
    I2C_OLED_ShowString(0, 2, strbuff, 16);
}