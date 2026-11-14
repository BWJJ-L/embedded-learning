#include "SPI0_soft.h"

// 初始化
void SPI0_soft_init() {
	// SCL  MOSI 输出  拉高
	GPIO_output(SPI0S_SCL_RCU, SPI0S_SCL_PORT, SPI0S_SCL_PIN, GPIO_OTYPE_PP, 1);
	GPIO_output(SPI0S_MOSI_RCU, SPI0S_MOSI_PORT, SPI0S_MOSI_PIN, GPIO_OTYPE_PP, 1);
	// MISO 上拉输入
	GPIO_input(SPI0S_MISO_RCU, SPI0S_MISO_PORT, SPI0S_MISO_PIN, GPIO_PUPD_PULLUP);
}

// 写数据
void SPI0_soft_write(uint8_t dat) {
	for(uint8_t i=0;i<8;i++){			  
		SPI0S_SCL(0); // 时钟线拉低
		if(dat&0x80) SPI0S_MOSI(1);
		else 		 SPI0S_MOSI(0);
		SPI0S_SCL(1); // 时钟线拉高
		dat <<= 1;   
	}
}

// 读数据
uint8_t SPI0_soft_read() {
	uint8_t read = 0;
	for(uint8_t i=0;i<8;i++){			  
		SPI0S_SCL(0); // 时钟线拉低
		
		read <<= 1;  // 左移1位，留最低位放数据，默认为0
		if (SPI0S_MISO()) read |= 1;
		

		SPI0S_SCL(1); // 时钟线拉高 
	}
	
	return read;
}

// 读写数据
uint8_t SPI0_soft_read_write(uint8_t dat) {
	uint8_t read = 0;
	for(uint8_t i=0;i<8;i++){			  
		SPI0S_SCL(0); // 时钟线拉低
		
		// ============================ 写
		if(dat&0x80) SPI0S_MOSI(1);
		else 		 SPI0S_MOSI(0);
		dat <<= 1;   
		
		// ============================ 读
		read <<= 1;  // 左移1位，留最低位放数据，默认为0
		if (SPI0S_MISO()) read |= 1;
		
		SPI0S_SCL(1); // 时钟线拉高 
	}
	
	return read;
}