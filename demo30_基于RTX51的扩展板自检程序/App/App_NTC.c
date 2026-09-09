#include "App.h"
#include "NTC.h"

static int global_temp;

void task_4(void) _task_ TASK_NTC {
    NTC_init();
    
    while (1) {
        global_temp = NTC_get_temperature();
        
        printf("temp -> %d ℃\n", global_temp);
        //温度显示内容只是数字部分，只占屏幕一小部分，传入参数0不刷新整个屏幕
        APP_I2C_OLED_Refresh(0);
        
        os_wait2(K_TMO, 200);   // 1秒读取一次温度 200 * 5ms
    }
}

//NTC模块需要在I2C屏幕上显示的内容
void NTC_show() {
    char strbuff[20];
    //显示内容从“2”行开始
    //先输出空白，防止温度从两位数降到一位数时有残留
    I2C_OLED_ShowString(0, 2, "Temp :    C", 16);
    
    sprintf(strbuff, "Temp : %d C", global_temp);
    I2C_OLED_ShowString(0, 2, strbuff, 16);
}

void TASK_NTC_reset() {
    ADC_PowerControl(DISABLE);
    os_delete_task(TASK_NTC);
}