#include "Key.h"

// 初始化
void Key_init(){
    P0_MODE_IO_PU(GPIO_Pin_5);
    
//    P0_MODE_OUT_OD(GPIO_Pin_5);
}

u8 last_Key_state = UP;

// 扫描按键
void Key_scan2(){
    //第一种扫描按键写法
//    if(UP == last_Key_state && DOWN == KEY) {
//        last_Key_state = KEY;
//        #if USE_KEYDOWN
//        Key_on_keydown();
//        #endif
//    } else if(DOWN == last_Key_state && UP == KEY){
//        last_Key_state = KEY;
//        #if USE_KEYUP
//        Key_on_keyup();
//        #endif
//    }
    
    //第二种扫描按键写法
    if(last_Key_state != KEY) {
        last_Key_state = KEY;
        if(KEY == DOWN) {
            #if USE_KEYDOWN
            Key_on_keydown();
            #endif
        } else if(KEY == UP) {
            #if USE_KEYUP
            Key_on_keyup();
            #endif
        }
    }
    
}

//第三种扫描按键写法
// 扫描按键，函数指针回调函数版本
// 不要和Key_scan()同时使用
void Key_scan(void (*down)(), void (*up)()){
    if(last_Key_state != KEY) {
        last_Key_state = KEY;
        if(KEY == DOWN) {
            if(down) down();
        } else if(KEY == UP) {
            if(up) up();
        }
    }
}