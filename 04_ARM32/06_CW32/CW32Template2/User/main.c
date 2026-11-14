#include "board.h"
#include "uart1.h"

int main(void) {
	board_init(); 	// 开发板初始化，配置时钟，滴答定时器
    uart1_init();	// 串口1初始化
    
    uart1_sendstring("\r\nCW32L011 UART Interrupt\r\n");
    printf("\r\nCW32L011 UART Interrupt Example\r\n");
	printf("数字为%d\n", 250);
	
    while(1) {
		//中断收发
    }
}
