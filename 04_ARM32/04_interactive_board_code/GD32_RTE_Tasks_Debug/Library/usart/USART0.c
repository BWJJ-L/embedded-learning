#include "USART0.h"

static void DMA_tx_config() { // 内存到外设配置
	// 时钟使能
	rcu_periph_clock_enable(USART0_TX_DMA_RCU);
	// DMA初始化
	dma_single_data_parameter_struct init_struct;
	// 方向： 内存到外设
	init_struct.direction = DMA_MEMORY_TO_PERIPH;
	// 源头   内存
//	init_struct.memory0_addr = (uint32_t)dst;  // 后面动态指定
	init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;   // 内存增长
	// 目标  外设
	init_struct.periph_addr = USART0_DATA_ADDR;
	init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;  // 外设不增长

	// 搬运1个数据(1个元素)的大小
	init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT; // 8位
	// 搬运的总个数
//	init_struct.number = ARR_LEN;   // 后面动态指定
	// 是否循环，只有在ADC采样，才需要
	init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
	// 优先级
	init_struct.priority = DMA_PRIORITY_HIGH;
	
	
	dma_single_data_mode_init(USART0_TX_DMA_PERIPH_CH, &init_struct);
	
	// 子外设选择配置
	dma_channel_subperipheral_select(USART0_TX_DMA_PERIPH_CH, USART0_TX_DMA_PERIPH_SUB);
}

#define RX0_LENTH	128   // 数组总长度
static uint8_t RX0_Buffer[RX0_LENTH + 1]; // +1, 多留一个字符，放字符串结束符
static uint32_t rx0_cnt = 0; // 放了几个元素的个数，下标


static void DMA_rx_config() { // 外设到内存配置
	// 时钟使能
	rcu_periph_clock_enable(USART0_RX_DMA_RCU);
	// DMA初始化
	dma_single_data_parameter_struct init_struct;
	// 方向： 外设到内存
	init_struct.direction = DMA_PERIPH_TO_MEMORY;  // ==========   
	// 源头   外设
	init_struct.periph_addr = USART0_DATA_ADDR;
	init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;  // 外设不增长
	// 目标  内存
	init_struct.memory0_addr = (uint32_t)RX0_Buffer;  // ===== 指定
	init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;   // 内存增长

	// 搬运1个数据(1个元素)的大小
	init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT; // 8位
	// 搬运的总个数
	init_struct.number = RX0_LENTH;   // ======= 最大搬运个数
	// 是否循环，只有在ADC采样，才需要
	init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
	// 优先级
	init_struct.priority = DMA_PRIORITY_HIGH;
	
	
	dma_single_data_mode_init(USART0_RX_DMA_PERIPH_CH, &init_struct);
	
	// 子外设选择配置
	dma_channel_subperipheral_select(USART0_RX_DMA_PERIPH_CH, USART0_RX_DMA_PERIPH_SUB);

	// 清除标志位
	dma_flag_clear(USART0_RX_DMA_PERIPH_CH, DMA_FLAG_FTF);
	// 配置完，启用DMA
	dma_channel_enable(USART0_RX_DMA_PERIPH_CH);
}


void USART0_init() {
	#if USART0_TX_DMA_ENABLE
	// ================ +++++ 内存到外设  tx DMA配置
	DMA_tx_config();
	#endif
	#if USART0_RX_DMA_ENABLE
	// ================ +++++ 外设到内存  rx DMA配置
	DMA_rx_config();
	#endif
	
	// 引脚配置  复用功能
	// 复用串口0的TX
	GPIO_output_af(USART0_TX_RCU, USART0_TX_PORT, USART0_TX_PIN, GPIO_OTYPE_PP, USART0_TX_AF);
	// 复用串口0的RX
	GPIO_output_af(USART0_RX_RCU, USART0_RX_PORT, USART0_RX_PIN, GPIO_OTYPE_PP, USART0_RX_AF);
	
	// 串口配置
	// 时钟使能
	rcu_periph_clock_enable(RCU_USART0);
	/* reset USART 重置串口(可选) */
	usart_deinit(USART0);
	/* configure usart baud rate value 配置波特率*/
	usart_baudrate_set(USART0, USART0_BAUDRATE);
	
	/* configure USART transmitter 配置串口发送(允许发送)*/
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	#if USART0_TX_DMA_ENABLE
	// ================ +++++ 串口允许DMA发送
	usart_dma_transmit_config(USART0, USART_TRANSMIT_DMA_ENABLE);
	#endif
	
	/* configure USART receiver 配置串口接收(允许接收) */
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	#if USART0_RX_DMA_ENABLE
	// ================ +++++ 串口允许DMA接收
	usart_dma_receive_config(USART0, USART_RECEIVE_DMA_ENABLE);
	#endif
	
	// ================= 串口中断
	// nvic中断请求使能(中断优先级)
	// 参数1：在gd32f4xx.h 的159行
	nvic_irq_enable(USART0_IRQn, USART0_PRIORITY);
	#if !USART0_RX_DMA_ENABLE  // 非DMA使用
	// 串口中断使能  RBNE   有数据可读，指定触发中断
	usart_interrupt_enable(USART0, USART_INT_RBNE);
	#endif
	// 串口中断使能  IDLE   接收数据空闲，完成数据的接收后，自动触发中断
	usart_interrupt_enable(USART0, USART_INT_IDLE);
	
	/* enable usart 使能串口*/
	usart_enable(USART0);
}

// 发送1个byte数据
void USART0_send_byte(uint8_t byte){
	usart_data_transmit(USART0, byte);
	while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
}

// 发送多个byte数据
void USART0_send_data(uint8_t* data, uint32_t len){
#if !USART0_TX_DMA_ENABLE  // 非DMA
	// 异常处理
    if (data == NULL) return;
	for(uint32_t i = 0; i < len; i++) {
		USART0_send_byte(data[i]);
	}
#else // DMA
	// 内存到外设   指定内存的源头
	dma_memory_address_config(USART0_TX_DMA_PERIPH_CH, DMA_MEMORY_0, (uint32_t)data);
	// 指定搬运的个数
	dma_transfer_number_config(USART0_TX_DMA_PERIPH_CH, len);
	
	// 使能DMA，告诉它开始搬运
	dma_channel_enable(USART0_TX_DMA_PERIPH_CH);
	// 等待搬运完成   没有办完，就不是1，就一直执行循环
	while(RESET == dma_flag_get(USART0_TX_DMA_PERIPH_CH, DMA_FLAG_FTF));
	// 循环的外面，不是里面，清除标志位，不清除，只能用1次
	dma_flag_clear(USART0_TX_DMA_PERIPH_CH, DMA_FLAG_FTF);
	
	// 等待 串口 传输完成
	while(RESET == usart_flag_get(USART0, USART_FLAG_TC));
	usart_flag_clear(USART0, USART_FLAG_TC);
	
#endif
}

#include <string.h>
// 发送字符串 (结尾标记\0)
void USART0_send_string(char *data){
#if !USART0_TX_DMA_ENABLE // 非DMA
	// 异常处理
    if (data == NULL) return;
	for(uint32_t i = 0; data[i] != '\0'; i++) {
		USART0_send_byte(data[i]);
	}
#else
	USART0_send_data((uint8_t*)data, strlen(data));
#endif
}

#if USART0_PRINTF
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f) {
	USART0_send_byte(ch);
    return ch;
}
#endif

// 中断处理函数，名字不能乱写，从 startup_gd32xxx.s 拷贝名字  124行
//  电脑串口工具发送了   abc 下面函数至少触发了3次
void USART0_IRQHandler() { 
#if !USART0_RX_DMA_ENABLE  // 非DMA
	// 是RBNE触发的中断，才处理逻辑
	// 是RBNE触发的中断，usart_interrupt_flag_get返回就是1
	if (SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) {
		/* USART receive data function 接收数据*/
		uint8_t data = usart_data_receive(USART0);
		if (rx0_cnt >= RX0_LENTH) rx0_cnt = 0; // 数据越界处理
		RX0_Buffer[rx0_cnt++] = data;// 接收到data, 放在数组里
		
		// rx0_cnt 数组元素个数
	}
	
	// IDLE标志位
	if (SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)) {
		// 再读一次，不是为了读取数据，为了把IDLE标志位置零
		usart_data_receive(USART0);
		
		// printf("====USART_INT_FLAG_IDLE=====\n");
		// %#x  可以打印任意内容
		// printf("%#x, %#x, %#x\n", RX0_Buffer[0], RX0_Buffer[1], RX0_Buffer[2]);
		// 如果对方发的是非16进制，用字符串打印，对用户而言，调试更方便
		RX0_Buffer[rx0_cnt] = '\0';  // 对于字符串的特殊处理
		// %s 只对字符串有效果，16进制会乱码
		// printf("recv[%d]: %s\n", rx0_cnt, RX0_Buffer);
		
		#if USART0_RECV_CALLBACK
		// 收到串口0数据，回调函数
		USART0_on_recv(RX0_Buffer, rx0_cnt); // 调用
		#endif 
		
//		printf("rx0_cnt = %d\n", rx0_cnt);
		
		rx0_cnt = 0;  // 置零操作
	}
#else // DMA
	// IDLE标志位
	if (SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)) {
		// 再读一次，不是为了读取数据，为了把IDLE标志位置零
		usart_data_receive(USART0);
		
		// DMA 把串口接收到的数据，放在  RX0_Buffer 数组里面
		// 禁用DMA
		dma_channel_disable(USART0_RX_DMA_PERIPH_CH);
		// 获取搬运个数(没有这个函数)    获取剩下没有搬运的  
		//   获取搬运个数 = 总个数  - 获取剩下没有搬运的个数
		rx0_cnt = RX0_LENTH - dma_transfer_number_get(USART0_RX_DMA_PERIPH_CH);
		
		RX0_Buffer[rx0_cnt] = '\0';  // 对于字符串的特殊处理

		#if USART0_RECV_CALLBACK
		// 收到串口0数据，回调函数
		USART0_on_recv(RX0_Buffer, rx0_cnt); // 调用
		#endif 
		
		rx0_cnt = 0;  // 置零操作
		
		// 清除标志位
		dma_flag_clear(USART0_RX_DMA_PERIPH_CH, DMA_FLAG_FTF);
		// 使能DMA
		dma_channel_enable(USART0_RX_DMA_PERIPH_CH);
	}

#endif
}