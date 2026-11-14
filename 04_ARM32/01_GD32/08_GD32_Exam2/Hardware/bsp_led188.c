#include "bsp_led188.h"

static uint32_t state = 0x0000; //都不亮，32位能标志32个灯，这边只用18位
// 个位
#define A3	(1L << 0)
#define B3	(1L << 1)
#define C3	(1L << 2)
#define D3	(1L << 3)
#define E3	(1L << 4)
#define F3	(1L << 5)
#define G3	(1L << 6)
// 十位
#define A2	(1L << 7)
#define B2	(1L << 8)
#define C2	(1L << 9)
#define D2	(1L << 10)
#define E2	(1L << 11)
#define F2	(1L << 12)
#define G2	(1L << 13)
// 百位 1
#define B1	(1L << 14)
#define C1	(1L << 15)
// 闪电
#define L1	(1L << 16) 
// %
#define L2	(1L << 17) 

// 初始化
void LED188_init() {
	printf("==LED188_init==\n");
	
	// 全部线路断开
	LED188_ALL_IN();
	
//	// 数字7
//	state = state | A3 | B3 | C3;
//	// 闪电
//	state = state | L1;
	
	// 低18位都设置为1，灯全亮
//	state = 0xfffff;
//	LED188_set_num(3);
//	LED188_set_num(23);
	
	LED188_charge(1); // 闪电
	LED188_percent(1); // %
	LED188_set_num(123);
//	LED188_set_num(3);
}

// 内容显示，点灯
void LED188_display() {  // 2ms 轮询1次
	static uint8_t i = 1;
	// 全部线路断开
	LED188_ALL_IN();
	if (i == 1) { // 点亮 1 开头的灯
		LED188_PIN1_OUT(1); // 引脚1设置为高电平
		if (state & A3) LED188_PIN2_OUT(0);
		if (state & C3) LED188_PIN3_OUT(0);
		if (state & E3) LED188_PIN4_OUT(0);
	} else if (i == 2) {
		LED188_PIN2_OUT(1); // 引脚2设置为高电平
		if (state & B3) LED188_PIN1_OUT(0);
		if (state & A2) LED188_PIN3_OUT(0);
		if (state & C1) LED188_PIN4_OUT(0);
		if (state & L2) LED188_PIN5_OUT(0);
	} else if (i == 3) {
		LED188_PIN3_OUT(1); // 引脚3设置为高电平
		if (state & D3) LED188_PIN1_OUT(0);
		if (state & B2) LED188_PIN2_OUT(0);
		if (state & B1) LED188_PIN4_OUT(0);
		if (state & L1) LED188_PIN5_OUT(0);
	} else if (i == 4) {
		LED188_PIN4_OUT(1); // 引脚4设置为高电平
		if (state & F3) LED188_PIN1_OUT(0);
		if (state & D2) LED188_PIN2_OUT(0);
		if (state & C2) LED188_PIN3_OUT(0);
	} else if (i == 5) {
		LED188_PIN5_OUT(1); // 引脚5设置为高电平
		if (state & G3) LED188_PIN1_OUT(0);
		if (state & E2) LED188_PIN2_OUT(0);
		if (state & F2) LED188_PIN3_OUT(0);
		if (state & G2) LED188_PIN4_OUT(0);
	}
	
	i++;
	if (i > 5) i = 1;
}

void LED188_clear() { // 灯灭
	state = 0;
}

static uint8_t table[] = {
    0x3F, //0
    0x06, //1
    0x5B, //2
    0x4F, //3
    0x66, //4
    0x6D, //5
    0x7D, //6
    0x07, //7
    0x7F, //8
    0x6F, //9
};
/*
3
23
123

数 % 10 === 个位
*/
void LED188_set_num(uint8_t num) {    // 数字
	if (num > 199) return;
	
	// 只清除低16位数据   0xffff
	state &= ~0xffff;  // 建议加
	
	// 个位
	uint8_t ge = num % 10;  // 取出个位
	uint8_t ge_bit = table[ge]; // 取出码表的元素
	state |= ge_bit;
	
	// 十位
	if (num > 9) {
		uint8_t shi = num / 10 % 10;  // 取出十位
		uint8_t shi_bit = table[shi]; // 取出码表的元素
		state |= (shi_bit << 7); // 左移7位
	}
	
	// 百位
	if (num > 99) {
		state |= C1;
		state |= B1;
	}
	
}

void LED188_charge(uint8_t enable) {  // 闪电
	if (enable) {
		state |= L1;
	} else {
		state &= ~L1;
	}
}

void LED188_percent(uint8_t enable) { // %
	if (enable) {
		state |= L2;
	} else {
		state &= ~L2;
	}
}