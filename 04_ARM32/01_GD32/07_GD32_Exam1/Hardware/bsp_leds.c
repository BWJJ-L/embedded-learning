#include "bsp_leds.h"

/*

// 结构体类型
typedef struct {
	rcu_periph_enum rcu;
	uint32_t port;
	uint32_t pin;
	uint8_t otype;
	uint8_t flag;
}GPIO_info_t;

GPIO_info_t  g_leds_list[] = {
	{RCU_GPIOC, GPIOC, GPIO_PIN_6, GPIO_OTYPE_PP, 0}, // 第0个，三极管
	{RCU_GPIOD, GPIOD, GPIO_PIN_8, GPIO_OTYPE_PP, 1},  // 第1个  LED1
	{RCU_GPIOD, GPIOD, GPIO_PIN_9, GPIO_OTYPE_PP, 1},  // 第2个  LED1
};

g_leds_list[0].rcu
g_leds_list[0].port


结构体类型  数组名字[] = {
	{RCU_GPIOC, GPIOC, GPIO_PIN_6, GPIO_OTYPE_PP, 0}, // 第0个，三极管
	{RCU_GPIOD, GPIOD, GPIO_PIN_8, GPIO_OTYPE_PP, 1},  // 第1个  LED1
	{RCU_GPIOD, GPIOD, GPIO_PIN_9, GPIO_OTYPE_PP, 1},  // 第2个  LED1
};

宏定义，求数据元素
#define  个数 		sizeof(数组名字)/sizeof(数组名字[0])

*/

// 结构体类型
typedef struct {
	rcu_periph_enum rcu;
	uint32_t port;
	uint32_t pin;
	uint8_t otype;
	uint8_t flag;
}GPIO_info_t;   // 分号

GPIO_info_t  g_leds_list[] = {
	{RCU_GPIOC, GPIOC, GPIO_PIN_6, GPIO_OTYPE_PP, 0}, // 第0个，三极管
	{RCU_GPIOD, GPIOD, GPIO_PIN_8, GPIO_OTYPE_PP, 1},  // 第1个  LED1
	{RCU_GPIOD, GPIOD, GPIO_PIN_9, GPIO_OTYPE_PP, 1},  // 第2个  LED2
	{RCU_GPIOD, GPIOD, GPIO_PIN_10, GPIO_OTYPE_PP, 1},  // 第3个  LED3
	{RCU_GPIOD, GPIOD, GPIO_PIN_11, GPIO_OTYPE_PP, 1},  // 第4个  LED4
	{RCU_GPIOD, GPIOD, GPIO_PIN_12, GPIO_OTYPE_PP, 1},  // 第5个  LED5
	{RCU_GPIOD, GPIOD, GPIO_PIN_13, GPIO_OTYPE_PP, 1},  // 第6个  LED6
	{RCU_GPIOD, GPIOD, GPIO_PIN_14, GPIO_OTYPE_PP, 1},  // 第7个  LED7
	{RCU_GPIOD, GPIOD, GPIO_PIN_15, GPIO_OTYPE_PP, 1},  // 第8个  LED8
}; // 分号

#define  G_LEDS_NUM() 		(sizeof(g_leds_list)/sizeof(g_leds_list[0]))

// 初始化
void Leds_init() {
	for(uint8_t i = 0; i < G_LEDS_NUM(); i++) {
		GPIO_output(g_leds_list[i].rcu, g_leds_list[i].port, 
			g_leds_list[i].pin, g_leds_list[i].otype, g_leds_list[i].flag);
	}
}

// 点亮某个灯
void Leds_turn_on(uint8_t index) { // 1 ~ 4
	// 下标异常处理
	if (index == 0 || index >= G_LEDS_NUM()) return;
	
	gpio_bit_reset(g_leds_list[index].port, g_leds_list[index].pin);
	
}

// 熄灭某个灯
void Leds_turn_off(uint8_t index) {
	// 下标异常处理
	if (index == 0 || index >= G_LEDS_NUM()) return;
	
	gpio_bit_set(g_leds_list[index].port, g_leds_list[index].pin);
}

// 点亮或熄灭某个灯，flag为真，灯亮；flag为假，灯灭
void Leds_turn(uint8_t index, uint8_t flag) {
	if (flag) { // 为真即可满足条件  亮
		Leds_turn_on(index);
	} else {
		Leds_turn_off(index);
	}
}

// 点亮或熄灭所有灯  flag为1，灯亮；flag为0，灯灭
void Leds_turn_all(uint8_t flag) {
	// 从1开始
	for(uint8_t i = 1; i < G_LEDS_NUM(); i++) {
		if (flag == 1) Leds_turn_on(i);
		else Leds_turn_off(i);
	}
}