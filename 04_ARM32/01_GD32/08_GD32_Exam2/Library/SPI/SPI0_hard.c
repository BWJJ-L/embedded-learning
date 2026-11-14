#include "SPI0_hard.h"

// 初始化
void SPI0_hard_init() {
    // GPIO ---------------------------------
    // SCL, MOSI, MISO 复用
	GPIO_output_af(SPI0H_SCL_RCU, SPI0H_SCL_PORT, SPI0H_SCL_PIN, GPIO_OTYPE_PP, SPI0H_SCL_AF);
    GPIO_output_af(SPI0H_MOSI_RCU, SPI0H_MOSI_PORT, SPI0H_MOSI_PIN, GPIO_OTYPE_PP, SPI0H_MOSI_AF);
	GPIO_output_af(SPI0H_MISO_RCU, SPI0H_MISO_PORT, SPI0H_MISO_PIN, GPIO_OTYPE_PP, SPI0H_MISO_AF);
  
    // SPI ----------------------------------
    // 时钟使能 rcu_periph_clock_enable
	rcu_periph_clock_enable(RCU_SPI0);
    // spi初始化 spi_init
	spi_parameter_struct spi_struct;
	spi_struct.device_mode = SPI_MASTER; // 主设备
	spi_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX; // 全双工
	spi_struct.frame_size = SPI_FRAMESIZE_8BIT; // 数据帧大小 
	spi_struct.nss = SPI_NSS_SOFT; // 软件片选
	spi_struct.endian = SPI0H_PARAM_ENDIAN; // 大小端
	spi_struct.clock_polarity_phase = SPI0H_CPOL_CPHA; // 工作模式
	spi_struct.prescale = SPI0H_PSC; // 分频
	spi_init(SPI0, &spi_struct);
    // spi使能 spi_enable
	spi_enable(SPI0);
}

void SPI0_hard_write(uint8_t dat) {
    // 循环等待 SPI 数据发送缓冲区(SPI_FLAG_TBE)为空 spi_i2s_flag_get
	while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE));
    // 通知外设电路，发送数据 spi_i2s_data_transmit
	spi_i2s_data_transmit(SPI0, dat);
    // 循环等待 SPI 接收缓冲区有新数据可读(SPI_FLAG_RBNE) spi_i2s_flag_get
	while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE));
    // 接收数据(只是为了清空接收缓冲区) spi_i2s_data_receive
	spi_i2s_data_receive(SPI0);
}

uint8_t SPI0_hard_read() {
    // 循环等待 SPI 数据发送缓冲区(SPI_FLAG_TBE)为空 spi_i2s_flag_get
	while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE));
    // 通知外设电路，发送数据 spi_i2s_data_transmit
	spi_i2s_data_transmit(SPI0, 0x00);
    // 循环等待 SPI 接收缓冲区有新数据可读(SPI_FLAG_RBNE) spi_i2s_flag_get
	while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE));
    // 接收数据 spi_i2s_data_receive
	return spi_i2s_data_receive(SPI0);
}

// 读写数据
uint8_t SPI0_hard_read_write(uint8_t dat) {
    // 循环等待 SPI 数据发送缓冲区(SPI_FLAG_TBE)为空 spi_i2s_flag_get
	while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE));
    // 通知外设电路，发送数据 spi_i2s_data_transmit
	spi_i2s_data_transmit(SPI0, dat);
    // 循环等待 SPI 接收缓冲区有新数据可读(SPI_FLAG_RBNE) spi_i2s_flag_get
	while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE));
    // 接收数据 spi_i2s_data_receive
	return spi_i2s_data_receive(SPI0);
}