#include "USART0.h"

void USART0_init() {
	// 串口0 TX 复用功能   
	GPIO_output_af(USART0_TX_RCU, USART0_TX_PORT, USART0_TX_PIN, GPIO_OTYPE_PP, USART0_TX_AF);
	// 串口0 RX 复用功能  
	GPIO_output_af(USART0_RX_RCU, USART0_RX_PORT, USART0_RX_PIN, GPIO_OTYPE_PP, USART0_RX_AF);
	
	// =========== 串口配置
	// 时钟使能
	rcu_periph_clock_enable(RCU_USART0);
	// 重置串口(可选)
	usart_deinit(USART0);
	// 设置波特率(必须要配置)
	usart_baudrate_set(USART0, USART0_BAUDRATE);
	// 允许串口发送
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	// ================允许串口接收
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	
	// ================中断配置
	// 中断优先级，使能中断请求  nvic_irq_enable在misc.h中
	// 参数1： 中断类型枚举常量, 在 gd32f4xx.h第159行
	// 参数2： 抢占优先级
	// 参数3： 响应优先级
	nvic_irq_enable(USART0_IRQn, USART0_PRIORITY);
	// (使能串口中断)RBNE, 有数据可读，自动触发中断，配置中断触发条件
	usart_interrupt_enable(USART0, USART_INT_RBNE);
	// IDLE, 空闲触发中断，数据读取完毕(或发送完毕)，即可触发中断
	usart_interrupt_enable(USART0, USART_INT_IDLE);
	
	// 使能串口
	usart_enable(USART0);
}

// 发送1个byte数据
void USART0_send_byte(uint8_t byte){
	usart_data_transmit(USART0, byte);
	// 等待发送完成, 没有完成，说明返回值为RESET, 一直循环，直到变为SET退出循环
	// USART_FLAG_TBE: transmit data buffer empty  发送缓冲区为空
	// 发送缓冲区为空, 硬件置1，说明发送完成   SET就是1
	while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
}

// 发送多个byte数据
void USART0_send_data(uint8_t* data, uint32_t len){
	if (data == NULL) return;
	
	for(uint32_t i = 0; i < len; i++) {
		USART0_send_byte(data[i]); // 发送单个字符
	}
}

// 发送字符串 (结尾标记\0)
void USART0_send_string(char *data){
	if (data == NULL) return;
	
	for(uint32_t i = 0; data[i] != '\0'; i++) {
		USART0_send_byte(data[i]); // 发送单个字符
	}
}

#if USART0_PRINTF
#include <stdio.h>
// 重写fputc， printf即可使用
int fputc(int ch, FILE *f) {
	USART0_send_byte(ch); // 串口发送1个字节
	
	return ch;
}
#endif

/*
1. 串口中断处理函数，函数名，不能乱写，因为汇编启动文件已经规定好
USART0_IRQHandler  在 startup_gd32f407_427.s 的124行 
2. 触发中断的条件有很多，一定要通过标志位区分，处理完也要清零
STC8 接收逻辑
if(COM1.RX_Cnt >= COM_RX1_Lenth)	COM1.RX_Cnt = 0;
RX1_Buffer[COM1.RX_Cnt++] = SBUF;
*/

#define RX0_LENTH	128		// 最大大小，数组长度
static uint8_t RX0_Buffer[RX0_LENTH + 1]; // +1，多一个位置给字符串结束符
static uint32_t  rx0_cnt = 0; // 元素个数

void USART0_IRQHandler() {
	// RBNE(有数据可读) 触发的中断，硬件置1(SET)
	if (SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) {
		// 软件清零, 清零后，才能反复读取
		usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
		
		// 读取1个字节
		uint8_t data = usart_data_receive(USART0);
		if(rx0_cnt >= RX0_LENTH)	rx0_cnt = 0; // 越界处理
		// 来一个数据，保存到一个数组中
		RX0_Buffer[rx0_cnt++] = data;
	}
	
	// IDLE, 空闲触发中断，数据读取完毕(或发送完毕)
	if (SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)) { 
		// 清除标志位
		usart_data_receive(USART0); // 读取一次，不是为了读数据，为了清除空闲的标志位
		
		// %s 打印只针对字符串，如果内容为16进制，%s打印会乱码
		RX0_Buffer[rx0_cnt] = '\0'; // 字符串结束符， 如果内容为16进制，这个处理没有意义

		#if USART0_RECV_CALLBACK
		// 收到串口0数据，回调函数
		USART0_on_recv(RX0_Buffer, rx0_cnt); // 函数调用
		#endif
		
		rx0_cnt = 0; // 元素个数置零
	}
	
}
