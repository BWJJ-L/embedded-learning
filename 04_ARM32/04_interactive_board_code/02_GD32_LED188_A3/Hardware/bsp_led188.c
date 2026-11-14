#include "bsp_led188.h"

// 初始化
void LED188_init() {
	printf("==LED188_init==\n");
	
	// 全部线路断开
	LED188_ALL_IN();
}