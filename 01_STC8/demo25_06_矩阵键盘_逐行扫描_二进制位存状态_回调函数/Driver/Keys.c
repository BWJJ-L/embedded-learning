#include "Keys.h"
#include "GPIO.h"

static void GPIO_config() {

    // 准双向
    KEYS_GPIO_INIT();

    // 高阻输入: 需要添加内部或外部上拉
//    P5_MODE_IN_HIZ(GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
//    P5_PULL_UP_ENABLE(GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);

}

#define DOWN        0
#define UP          1


// u32
// 0 0 0 0 - 1 1 1 1
static u8 states = 0x0F;

// [判断] 指定位置是否是 按下0 or 抬起1
//  0b 0 0 0 0 - 1 1 0 1 
// &0b 0 0 0 0 - 0 1 0 0 判断指定位置是否为1或0
//  0b 0 0 0 0 - 0 1 0 0

//  0b 0 0 0 0 - 1 1 0 1 
//  0b 0 0 0 0 - 0 0 1 1  >> 2   把目标位挪到最低位
//  0b 0 0 0 0 - 0 0 0 1  & 0x01 只保留最低位
#define IS_KEY_DOWN(k)      (((states >> k) & 0x01) == DOWN)  // 判断按下 states & (1 << k) == 0
#define IS_KEY_UP(k)        (((states >> k) & 0x01) == UP  )    // 判断抬起

// [设置] 指定位置为 按下0
//   0b 0 0 0 0 - 1 1 0 1 
//&= 0b 1 1 1 1 - 1 0 1 1   
//&=~0b 0 0 0 0 - 0 1 0 0   &=~(1L << k)
//   0b 0 0 0 0 - 1 0 0 1 
#define SET_KEY_DOWN(k)     (states &= ~(1L << k))   // 设置为按下

// [设置] 指定位置为 抬起1
//   0b 0 0 0 0 - 1 0 0 1 
// |=0b 0 0 0 0 - 0 1 0 0
//   0b 0 0 0 0 - 1 1 0 1
#define SET_KEY_UP(k)       (states |=  (1L << k))     // 设置为抬起


//const u8 keys[] = {KEY1, KEY2, KEY3, KEY4};

u8 get_value(u8 k){
    switch (k)
    {
    	case 0: return KEY1;
    	case 1: return KEY2;
    	case 2: return KEY3;
    	case 3: return KEY4;
    	default: return 0;
    }
    
    // main.c(103): warning C291: not every exit path returns a value
//    return 0;
}

void Keys_init(void) {

    GPIO_config();
}

void Keys_scan(void) {
    u8 i = 0;
    for(i = 0; i < 4; i++) {
        if(IS_KEY_DOWN(i) && get_value(i) == UP) {
            // 感知抬起: 判断是否是上升沿(上一次低电平0, 本次是高电平1)            
            Keys_on_keyup(i);
            // 记录上一次(最后一次)的状态
            SET_KEY_UP(i);
        } else if(IS_KEY_UP(i) && get_value(i) == DOWN) {
            // 感知按下: 判断是否是下降沿(上一次高电平1, 本次是低电平0)           
            Keys_on_keydown(i);
            // 记录上一次(最后一次)的状态
            SET_KEY_DOWN(i);
        }
    }
}