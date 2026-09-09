#include "App.h"
#include "Keys.h"

//任务序号,用来定位OLED屏幕执行的任务序号 0 - 6
int global_cur_pos = 0;

void Keys_on_keydown(u8 key_index) {
    //上一次屏幕执行的任务序号0 - 6
    int8 global_prev_pos = global_cur_pos;
    
    printf("keys->%d\n",(int)key_index);
    
    //按键按下更新当前的任务序号
    if(key_index == 0 || key_index == 1) {
        global_cur_pos = (global_cur_pos - 1 + 7) % 7;
    } else if(key_index == 2 || key_index == 3) {
        global_cur_pos = (global_cur_pos + 1) % 7;
    }
    
    printf("global_cur_pos -> %d\n",(int)global_cur_pos);
    

    //结束上一次的任务
    switch(global_prev_pos) {
        case 0:TASK_LED_reset();break;
        case 1:TASK_NTC_reset();break;
        case 2:TASK_MOTOR_reset();break;
        case 3:TASK_RTC_reset();break;
        case 4:TASK_NIXIE_reset();break;
        case 5: break;
        case 6: break;
        default: break;
    }
    
    //开始本次的任务
    switch(global_cur_pos) {
        case 0:os_create_task(TASK_LED);break;
        case 1:os_create_task(TASK_NTC);break;
        case 2:os_create_task(TASK_Motor);break;
        case 3:os_create_task(TASK_RTC);break;
        case 4:os_create_task(TASK_NIXIE);break;
        case 5: break;
        case 6: break;
        default: break;
    }
    
    
    //SPI_OLED刷新不需要封装，因为固定需要刷新屏幕
    os_send_signal(TASK_SPI_OLED);
    //I2C_OLED需要判断是否要刷新屏幕
    APP_I2C_OLED_Refresh(1);

}

void task_1(void) _task_ TASK_KEYS {
    Keys_init();
    while (1) {
        Keys_scan();
        
        os_wait2(K_TMO, 2);
    }
}