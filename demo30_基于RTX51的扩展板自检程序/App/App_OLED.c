#include "App.h"
#include "I2C_OLED.h"
#include "SPI_OLED.h"

//菜单结构体,title显示在SPI屏幕上,content用来显示在I2C屏幕上
typedef struct {
    char title[20];
    char content[20];
} menu;
//菜单内容
menu menu_arr[] = {
    {"8个LED闪烁",  "LED"},
    {"热敏电阻",    "NTC"},
    {"电位器+马达", "POT + MOTOR"},
    {"RTC时钟",     "RTC"},
    {"数码管",      "NIXIE Digital"},
    {"温湿度",      "DHT11"},
    {"键盘蜂鸣器",  "MatrixKey+Buzzer"}
};
//菜单数量
u8 menu_cnt = sizeof(menu_arr) / sizeof(menu_arr[0]);

// SPI oled屏幕刷新任务
void task_2(void) _task_ TASK_SPI_OLED {
    u8 i = 0;
    char strbuff[20];

    SPI_OLED_Init();
    SPI_OLED_ColorTurn(0);
    SPI_OLED_DisplayTurn(0);
    SPI_OLED_Clear();

    while (1) {
        SPI_OLED_Clear();
        
        //更新SPI_OLED内容
        for (i = 0; i < 4; i++) {
            u8 index = (global_cur_pos + i) % menu_cnt;
            sprintf(strbuff, "%c %d %s", (i ? ' ' : '>'), (int)index, menu_arr[index].title);
            SPI_OLED_Display_GB2312_string(0, i * 2, strbuff);
        }
        
        // 等待刷新信号
        os_wait1(K_SIG);
    }
}

//在I2C屏幕内用来判断是否要刷新屏幕
static u8 global_is_clear_screen = 1;

//更新整个I2C_OLED屏幕内容需要传入参数1刷新，只更新屏幕内一部分内容传入参数0不刷新就行
void APP_I2C_OLED_Refresh(u8 clear_screen) {
    if (clear_screen) {
        global_is_clear_screen = 1;
    }
    os_send_signal(TASK_I2C_OLED);
}

// I2C_OLED屏幕刷新任务
void task_3(void) _task_ TASK_I2C_OLED {
    u8 i = 0;
    I2C_OLED_Init();
    I2C_OLED_ColorTurn(0);
    I2C_OLED_DisplayTurn(0);

    while (1) {
        if (global_is_clear_screen) {
            global_is_clear_screen = 0;
            
            I2C_OLED_Clear();
            
            printf("Clear OLED\n");
        }
        
        I2C_OLED_ShowString(0, 0, menu_arr[global_cur_pos].content, 16);
        
        //I2C_OLED屏幕切换任务显示
        switch (global_cur_pos) {
            case 0: LED_show(); break;
            case 1: NTC_show(); break;
            case 2: MOTOR_show(); break;
            case 3: RTC_show(); break;
            case 4: NixieDigital_show();break;
            case 5: break;
            case 6: break;
            default: break;
        }

        // 等待刷新信号
        os_wait1(K_SIG);
    }
}