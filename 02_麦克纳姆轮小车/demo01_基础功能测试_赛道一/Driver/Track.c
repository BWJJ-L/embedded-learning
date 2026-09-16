#include "Track.h"

// 初始化
void Track_init(){
    P0_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
    
}
// 获取寻迹坐标： 高电平-不亮-压到黑线  低电平-亮起-正常反射面
int Track_get_position(){
    static int last_position;
    u8 cnt = 0;
    int position = 0;
    
    if(LED1 == 1){
        position += -64;
        cnt++;
    }
    
    if(LED2 == 1){
        position += -32;
        cnt++;
    }
    
    if(LED3 == 1){
        position += 0;
        cnt++;
    }
    
    if(LED4 == 1){
        position += 32;
        cnt++;
    }
    
    if(LED5 == 1){
        position += 64;
        cnt++;
    }
    
    //没有灯在黑线上时
    if(cnt == 0){
        return last_position;
    }
    
    //多个灯在黑线上时，算平均值
    position = position / cnt;
    
    last_position = position;
    
    return position;
}