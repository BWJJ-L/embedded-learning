#include "I2C0_soft.h"
#include "gpio_cfg.h" // GPIO配置头文件
#define I2C0S_SDA(bit)  gpio_bit_write(I2C0S_SDA_PORT, I2C0S_SDA_PIN, (bit) ? SET : RESET)
#define I2C0S_SCL(bit)  gpio_bit_write(I2C0S_SCL_PORT, I2C0S_SCL_PIN, (bit) ? SET : RESET)

// 标准模式 100Kbits/s, 快速模式 400Kbits/s
// 100Kbits/s -> 100 000 bit / 1 000 000us -> 1bit/10us  一个高一个低才是1bit
// 400Kbits/s -> 400 000 bit / 1 000 000us -> 4bit/10us -> 每1bit大概需要2.5us
#if !FAST
#define I2C0S_DELAY()   delay_1us(5)	// 标准模式
#else
#define I2C0S_DELAY()   delay_1us(1)	// 快速模式
#endif


#define I2C0S_SDA_STATE() gpio_input_bit_get(I2C0S_SDA_PORT, I2C0S_SDA_PIN) // 读取SDA电平

// 开始信号
static void start() {
//	// SDA 拉高
////	gpio_bit_set(I2C0S_SDA_PORT, I2C0S_SDA_PIN);
//	// gpio_bit_write(I2C0S_SDA_PORT, I2C0S_SDA_PIN, SET);
//	gpio_bit_write(I2C0S_SDA_PORT, I2C0S_SDA_PIN, 1 ? SET : RESET);
//	// 延时
//	delay_1us(1);
//	// SCL 拉高
//	gpio_bit_set(I2C0S_SCL_PORT, I2C0S_SCL_PIN);
//	// 延时
//	delay_1us(1);
//	// SDA 拉低
//	// gpio_bit_reset(I2C0S_SDA_PORT, I2C0S_SDA_PIN);
////	gpio_bit_write(I2C0S_SDA_PORT, I2C0S_SDA_PIN, RESET);
//	gpio_bit_write(I2C0S_SDA_PORT, I2C0S_SDA_PIN, 0 ? SET : RESET);
//	// 延时
//	delay_1us(1);
//	// SCL 拉低
//	gpio_bit_reset(I2C0S_SCL_PORT, I2C0S_SCL_PIN);
//	// 延时
//	delay_1us(1);
	
	
	// SDA 拉高
	I2C0S_SDA(1);
	// 延时
	I2C0S_DELAY();
	// SCL 拉高
	I2C0S_SCL(1);
	// 延时
	I2C0S_DELAY();
	// SDA 拉低
	I2C0S_SDA(0);
	// 延时
	I2C0S_DELAY();
	// SCL 拉低
	I2C0S_SCL(0);
	// 延时
	I2C0S_DELAY();
}

// 设备地址，寄存器地址，数据
static void send(uint8_t dat) {
	for(uint8_t i = 0; i < 8; i++) {
		// 确定dat最高位是否为1，为1，说明为真   SDA拉高，否则拉低
		if (dat & 0x80)  I2C0S_SDA(1);
		else			 I2C0S_SDA(0);
		// 延时
		I2C0S_DELAY();
		// SCL 拉高
		I2C0S_SCL(1);
		// 延时
		I2C0S_DELAY();
		// SCL 拉低
		I2C0S_SCL(0);
		// 延时
		I2C0S_DELAY();
		// dat 左移1位
		dat <<= 1;
	}	
}

// 停止信号
static void stop() {
	// SDA 拉低，延时
	I2C0S_SDA(0); I2C0S_DELAY();
	// SCL 拉高，延时
	I2C0S_SCL(1); I2C0S_DELAY();
	// SDA 拉高，延时
	I2C0S_SDA(1); I2C0S_DELAY();
	
}

// 接收1个字节
static uint8_t recv() {
	uint8_t dat = 0;
	// SDA 拉高
	I2C0S_SDA(1);
	// 循环接收8个位的数据
	for(uint8_t i = 0; i < 8; i++) {
		// SCL 拉高，延时
		I2C0S_SCL(1); I2C0S_DELAY();
		// dat 左移1位，留位置填充数据
		dat <<= 1;  	// <<=
//		if (gpio_input_bit_get(I2C0S_SDA_PORT, I2C0S_SDA_PIN) == SET) {
		if (I2C0S_SDA_STATE() == SET) {
			dat |= 1;   // |= 
		} 
		
		// SCL 拉低，延时
		I2C0S_SCL(0); I2C0S_DELAY();
	}	
	
	return dat;
}

// 发送ack响应
static void send_ack() {
	// SDA 拉低，延时
	I2C0S_SDA(0); I2C0S_DELAY();
	// SCL 拉高，延时
	I2C0S_SCL(1); I2C0S_DELAY();
	// SCL 拉低，延时
	I2C0S_SCL(0); I2C0S_DELAY();
}
// 发送nack响应
static void send_nack() {
	// SDA 拉高，延时
	I2C0S_SDA(1); I2C0S_DELAY();
	// SCL 拉高，延时
	I2C0S_SCL(1); I2C0S_DELAY();
	// SCL 拉低，延时
	I2C0S_SCL(0); I2C0S_DELAY();
}

// 等待响应，返回值 > 0, 出现异常
static uint8_t wait_ack() {
	// MCU 控制 SDA
	// SDA 拉高, 延时
	I2C0S_SDA(1); I2C0S_DELAY();
	// SCL 拉高, 延时
	I2C0S_SCL(1); I2C0S_DELAY();
	// 从设备控制 SDA，MCU读取
	// 读到低电平，说明响应成功
//	if (gpio_input_bit_get(I2C0S_SDA_PORT, I2C0S_SDA_PIN) == RESET) {
	if (I2C0S_SDA_STATE() == RESET) {
		I2C0S_SCL(0); I2C0S_DELAY();
		return 0;
	} else {// 读取高电平，响应失败
		stop();
		return 1;
	}
}

void I2C0_soft_init() {
	// SCL 开漏输出
	GPIO_output(I2C0S_SCL_RCU, I2C0S_SCL_PORT, I2C0S_SCL_PIN, GPIO_OTYPE_OD, 2); // 2 不处理电平
	// SDA 开漏输出
	GPIO_output(I2C0S_SDA_RCU, I2C0S_SDA_PORT, I2C0S_SDA_PIN, GPIO_OTYPE_OD, 2);
}

/**********************************************************
 * @brief 写数据到I2C设备指定寄存器
 * @param  addr 设备地址 (写地址的前7位) 0x51
                写地址 (addr << 1) | 0     0xA2
 * @param  reg  寄存器地址
 * @param  dat  字节数组
 * @param  len  数据长度
 * @return 0成功，1 设备不存在, 2 寄存器不存在, 3 数据无响应
 **********************************************************/
uint8_t I2C0_soft_write(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len) {
	// 起始信号
	start();
	// 发设备地址
	send((addr << 1) | 0);
	// 等待响应成功后，才下一步
	if (wait_ack() > 0) return 1;
	// 发寄存器地址
	send(reg);
	// 等待响应
	if (wait_ack()) return 2;
	// 循环发送数据，等待响应
	for(uint32_t i = 0; i < len; i++) {
		send(dat[i]);
		if (wait_ack()) return 3;
	}
	
	// 停止信号
	stop();
	
	return 0;
}

/**********************************************************
 * @brief 从I2C设备指定寄存器读取数据
 * @param  addr 设备地址  (写地址的前7位)   0x51
                写地址 (addr << 1) | 0     0xA2
                读地址 (addr << 1) | 1     0xA3
 * @param  reg  寄存器地址
 * @param  dat  字节数组
 * @param  len  数据长度
 * @return 0成功，1 写设备不存在, 2 寄存器不存在, 3 读设备不存在
 * @return
 **********************************************************/
uint8_t I2C0_soft_read(uint8_t addr, uint8_t reg, uint8_t* dat, uint32_t len) {
	// 起始信号
	start();
	// 发设备地址: 写
	send((addr << 1) | 0);
	// 等待响应成功后，才下一步
	if (wait_ack() > 0) return 1;
	// 发寄存器地址
	send(reg);
	// 等待响应
	if (wait_ack()) return 2;
	// 起始信号
	start();
	// 发设备地址: 读    读地址为 (addr << 1) | 1
	send((addr << 1) | 1);
	// 等待响应成功后，才下一步
	if (wait_ack() > 0) return 3;
	// ============ 循环读取数据
	for(uint32_t i = 0; i < len; i++) {
		dat[i] = recv();
		
		// 不是最后一个数据, 发送响应
		if (i != len - 1) send_ack();
		// 最后一个数据，发送空响应
		else 			send_nack();
	}
	
	// 停止信号
	stop();
	
	return 0;
}