#include "SPI1_hard.h"
#include "gpio_cfg.h"

#if SPI1H_TX_DMA_ENABLE
static void DMA_m2p_config() { // 内存到外设, 发送
	// 时钟使能
	rcu_periph_clock_enable(SPI1H_TX_DMA_RCU);
	// 重置
	dma_deinit(SPI1H_TX_DMA_PERIPH_CH);
	// 结构体配置默认参数
	dma_single_data_parameter_struct init_struct;
	dma_single_data_para_struct_init(&init_struct);
	// 修改结构体成员
	// 1. 方向： 内存到外设
	init_struct.direction = DMA_MEMORY_TO_PERIPH;
	// 2. 源头：内存
	// init_struct.memory0_addr = (uint32_t)xxx; // 内存内容动态变化，不能写死
	init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE; // 是否自增长(内存自增长)
	// 3. 目标：外设
	init_struct.periph_addr = SPI1H_DATA_ADDR; // 外设地址
	init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE; // 是否自增长(外设不增长)
	// 4. 搬运一个元素的大小
	init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT; // 8位
	// 5.  搬运最大元素个数
	// init_struct.number = ARR_LEN;  // 动态变化
	// 6. 优先级
	init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	// DMA初始化
	dma_single_data_mode_init(SPI1H_TX_DMA_PERIPH_CH, &init_struct);
	// 子外设选择
	dma_channel_subperipheral_select(SPI1H_TX_DMA_PERIPH_CH, SPI1H_TX_DMA_PERIPH_SUB);
}
#endif

// 初始化
void SPI1_hard_init() {
	// 复用推挽输出
	GPIO_output_af(SPI1H_SCL_RCU, SPI1H_SCL_PORT, SPI1H_SCL_PIN, GPIO_OTYPE_PP, SPI1H_SCL_AF);
	GPIO_output_af(SPI1H_MOSI_RCU, SPI1H_MOSI_PORT, SPI1H_MOSI_PIN, GPIO_OTYPE_PP, SPI1H_MOSI_AF);
	GPIO_output_af(SPI1H_MISO_RCU, SPI1H_MISO_PORT, SPI1H_MISO_PIN, GPIO_OTYPE_PP, SPI1H_MISO_AF);
	// 时钟使能
	rcu_periph_clock_enable(RCU_SPI1);
	// --------------SPI配置
	// 重置，可选
	spi_i2s_deinit(SPI1); 
	// 结构体配置默认参数
	spi_parameter_struct spi_struct;
	spi_struct_para_init(&spi_struct);
	// 修改关键成员
	spi_struct.device_mode          = SPI_MASTER; // 主设备
    spi_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX; // 全双工
    spi_struct.frame_size           = SPI_FRAMESIZE_8BIT; // 数据帧8位
    spi_struct.nss                  = SPI_NSS_SOFT;  // 片选为软实现
    // spi_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE; // 模式3，空闲为高，第2个边沿采样
    spi_struct.clock_polarity_phase = SPI1H_CPOL_CPHA;
	// spi_struct.prescale             = SPI_PSC_4;  // 84M / 4 = 21M
	spi_struct.prescale             = SPI1H_PSC; 
    // spi_struct.endian               = SPI_ENDIAN_MSB; // 大端
	spi_struct.endian               = SPI1H_PARAM_ENDIAN; // 大端
	
	// 上面的结构体做初始化
	spi_init(SPI1, &spi_struct);
	
	#if SPI1H_TX_DMA_ENABLE
	DMA_m2p_config(); 	// DMA 发送 配置
	spi_dma_enable(SPI1, SPI_DMA_TRANSMIT);
	#endif
	
	// SPI使能
	spi_enable(SPI1);
}

// 写数据
void SPI1_hard_write(uint8_t dat) {
	// 等发送缓冲区为空标志位为1，说明可以发送, 退出循环
	//	为RESET, 说明没有为1，死等
	while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE));
	// 发送
	spi_i2s_data_transmit(SPI1, dat);
//	// 等有数据可读标志位为1，说明可以读数据
//	while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE));
//	// 读取，读取的内容不用
//	spi_i2s_data_receive(SPI1);
}

// 读数据
uint8_t SPI1_hard_read() {
	// 等发送缓冲区为空标志位为1，说明可以发送, 退出循环
	//	为RESET, 说明没有为1，死等
	while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE));
	// 发送
	spi_i2s_data_transmit(SPI1, 0x00);
	// 等有数据可读标志位为1，说明可以读数据
	while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE));
	// 读取，读取的内容不用
	return spi_i2s_data_receive(SPI1);
}

// 读写数据
uint8_t SPI1_hard_read_write(uint8_t dat) {
	// 等发送缓冲区为空标志位为1，说明可以发送, 退出循环
	//	为RESET, 说明没有为1，死等
	while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE));
	// 发送
	spi_i2s_data_transmit(SPI1, dat);
	// 等有数据可读标志位为1，说明可以读数据
	while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE));
	// 读取，读取的内容不用
	return spi_i2s_data_receive(SPI1);
}

#if SPI1H_TX_DMA_ENABLE
void SPI1_dma_write(uint32_t memery_addr, uint32_t memery_len) {
	// 数据来源 和 长度
    dma_memory_address_config(SPI1H_TX_DMA_PERIPH_CH, DMA_MEMORY_0, memery_addr);
    dma_transfer_number_config(SPI1H_TX_DMA_PERIPH_CH, memery_len);

    // 触发传输
    dma_channel_enable(SPI1H_TX_DMA_PERIPH_CH);
	
    // 等待DMA传输完成
    while(RESET == dma_flag_get(SPI1H_TX_DMA_PERIPH_CH, DMA_FLAG_FTF));
    // 清理标记
    dma_flag_clear(SPI1H_TX_DMA_PERIPH_CH, DMA_FLAG_FTF);
}
#endif