#include "App.h"

void App_debug_init() {
	printf("==App_debug_init==\n");
}

void App_debug_test_task() { // 1000ms 轮询1次
//	printf("App_debug_test_task\n");
}


// ======== 188数码管业务
void App_debug_led188_test_task() { // 2ms 轮询1次
//	// 点亮 A3
//	// 所有引脚高阻(断开)
//	LED188_ALL_IN();
//	// 引脚1高电平   引脚2低电平
//	LED188_PIN1_OUT(1); LED188_PIN2_OUT(0);
    
    
    
    static uint8_t cnt = 1;
    LED188_ALL_IN();
    
//    if(cnt == 1){
//        LED188_PIN3_OUT(1);
//        LED188_PIN4_OUT(0);
//    }else if(cnt == 2){
//        LED188_PIN2_OUT(1);
//        LED188_PIN4_OUT(0);
//    }else if(cnt == 3){
//        LED188_PIN2_OUT(1);
//        LED188_PIN3_OUT(0);
//    }else if(cnt == 4){
//        LED188_PIN3_OUT(1);
//        LED188_PIN2_OUT(0);
//    }else if(cnt == 5){
//        LED188_PIN4_OUT(1);
//        LED188_PIN3_OUT(0);
//    }else if(cnt == 6){
//        LED188_PIN4_OUT(1);
//        LED188_PIN2_OUT(0);
//    }else if(cnt == 7){
//        LED188_PIN5_OUT(1);
//        LED188_PIN2_OUT(0);
//    }else if(cnt == 8){
//        LED188_PIN5_OUT(1);
//        LED188_PIN3_OUT(0);
//    }else if(cnt == 9){
//        LED188_PIN5_OUT(1);
//        LED188_PIN4_OUT(0);
//    }

    if(cnt == 1){
        LED188_PIN3_OUT(1);
        LED188_PIN5_OUT(0);
    }else if(cnt == 2){
        LED188_PIN2_OUT(1);
        LED188_PIN5_OUT(0);
    }
    
    cnt++;
    if(cnt > 2) cnt = 1;
}