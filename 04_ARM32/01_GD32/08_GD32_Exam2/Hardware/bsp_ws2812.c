#include "bsp_ws2812.h"
#include "SPI1_hard.h"
#include <string.h>

static uint8_t colors[WS2812_NUM][30] = {0};

void WS2812_init() {
	printf("==WS2812_init==\n");
	
#if 1
//	WS2812_set_color(0, 0xff0000); // 红
//	WS2812_set_color(1, 0x00ff00); // 绿
//	WS2812_set_color(2, 0x0000ff); // 蓝
//	WS2812_set_color(3, 0xffffff); // 白
	
	WS2812_set_color_brightness(0, 0x00ff00, 1); // 红
	WS2812_set_color_brightness(1, 0x00ff00, 1); // 绿
	WS2812_set_color_brightness(2, 0x00ff00, 1); // 蓝
	WS2812_set_color_brightness(3, 0x00ff00, 1); // 白

#else // 灭
	WS2812_set_color(0, 0x000000); 
	WS2812_set_color(1, 0x000000); 
	WS2812_set_color(2, 0x000000); 
	WS2812_set_color(3, 0x000000); 
#endif
	
	WS2812_display();  // 发送数据给彩灯
	
}

// 0和1的10位模式
#define BIT_0 0b1110000000  // 0x380
#define BIT_1 0b1111111000  // 0x3F8

static void byte_to_10bytes(uint8_t a, uint8_t b[10]) {
    uint16_t patterns[8];

    // 为每个bit生成对应的10位模式
    for (int i = 0; i < 8; i++) {
        patterns[i] = (a & (1 << (7 - i))) ? BIT_1 : BIT_0;
    }

    // 将8个10位模式紧密打包到10个字节中
    memset(b, 0, 10);

    int bit_pos = 0;
    for (int i = 0; i < 8; i++) {
        // 将当前10位模式放入输出缓冲区
        for (int j = 0; j < 10; j++) {
            if (patterns[i] & (1 << (9 - j))) {
                b[bit_pos / 8] |= (1 << (7 - (bit_pos % 8)));
            }
            bit_pos++;
        }
    }
}

/*
index = 0
static uint8_t colors[1][30] = {
    // 第0个
	0xE0, 0x38, 0x0E, 0x03, 0x80, 0xE0, 0x38, 0x0E, 0x03, 0x80, // G
    0xFE, 0x3F, 0x8F, 0xE3, 0xF8, 0xFE, 0x3F, 0x8F, 0xE3, 0xF8, // R
    0xE0, 0x38, 0x0E, 0x03, 0x80, 0xE0, 0x38, 0x0E, 0x03, 0x80, // B
};



*/

// index  哪一个灯  0, 1, 2, 3
// color: 8bit(保留) + 8bit(RED) + 8bit(GREEN) + 8bit(BLUE)
void WS2812_set_color(uint16_t index, uint32_t color) {
	if (index >= WS2812_NUM)  return;   // 必须是 >=，否则 index=4 越界写 colors[4]
	
	uint8_t r = (color >> 16 ) & 0xff;
	uint8_t g = (color >> 8 ) & 0xff;
	uint8_t b = color & 0xff;
	
	#if 0
	uint8_t temp[10];
	// G
	byte_to_10bytes(g, temp);
	memcpy(&colors[index][0], temp, 10);
	// R
	byte_to_10bytes(r, temp);
	memcpy(&colors[index][10], temp, 10);
	// B
	byte_to_10bytes(b, temp);
	memcpy(&colors[index][20], temp, 10);
	#else
	// G
	byte_to_10bytes(g, &colors[index][0]);
	// R
	byte_to_10bytes(r, &colors[index][10]);
	// B
	byte_to_10bytes(b, &colors[index][20]);
	#endif
}

// 显示
void WS2812_display() { // 40 ms 轮询1次
	SPI1_dma_write((uint32_t)colors, WS2812_NUM*30);
	
	// reset 码   
	uint8_t reset_buff[365] = {0};
	SPI1_dma_write((uint32_t)reset_buff, 365);
}

// 设置灯的亮度
// index  哪一个灯  0, 1, 2, 3
// color: 8bit(保留) + 8bit(RED) + 8bit(GREEN) + 8bit(BLUE)
// brightness: 亮度 参数0~100   代表0% ~ 100%
void WS2812_set_color_brightness(uint16_t index, uint32_t color, uint8_t brightness) {
    if (index >= WS2812_NUM) {
        return;
    }

    // 限制亮度范围
    if (brightness > 100) {
        brightness = 100;
    }

    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    // 按亮度比例缩放 RGB，+50 用于四舍五入
    r = (uint8_t)(((uint32_t)r * brightness ) / 100);
    g = (uint8_t)(((uint32_t)g * brightness ) / 100);
    b = (uint8_t)(((uint32_t)b * brightness ) / 100);

    uint32_t scaled_color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;

    WS2812_set_color(index, scaled_color);
}