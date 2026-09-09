#include "NIXIE.h"
#include "GPIO.h"

// 从byte字节里, 取出指定pos位的值
#define GET_BIT_VAL(byte, pos) (((byte) >> (pos)) & 1)

// SCK多行宏(标准写法)
#define SCK_ACTION() do {   \
    /* 寄存器的移位操作 */   \
    NIX_SCK = 0;            \
    NOP2();                 \
    NIX_SCK = 1;            \
    NOP2();                 \
}while(0)

#define RCK_ACTION() do{    \
    NIX_RCK = 0;            \
    NOP2();                 \
    NIX_RCK = 1;            \
    NOP2();                 \
}while(0)

// 下标对应表格参见：
u8 code LED_TABLE[] = 
{
	// 0 	1	 2	-> 9	(索引0,1,2...9)
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,
	// 0. 1. 2. -> 9.	(索引10,11,12....19)
    0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
	// . -						(索引20,21)
	0x7F, 0xBF,
	// AbCdEFHJLPqUo		(索引22,23,24....33, 34, 35)
	0x88,0x83,0xC6,0xA1,0x86,0x8E,0x89,0xF1,0xC7,0x8C,0x98,0xC1,0xA3,0xFF
};

void NIXIE_init(void){
    NIXIE_GPIO_INIT();
}

void NIXIE_show(u8 a_num, u8 b_idx){
    char i; // 倒序循环一定不能用无符号数

    // ------------------------------- SEG段位端 (显示内容)
    for(i = 7; i >= 0; i--){ // 7, 6, 5, 4, 3, 2, 1, 0
        // 先发字节的高位, 先考虑发0的情况(0是段位端, 拉低)
//        NIX_DI = a_num & (1 << i);     // 给引脚赋值非0, 等同于赋值1. 等效于: (a_num >> i) & 1
        NIX_DI = GET_BIT_VAL(a_num, i);  // 给引脚赋值非0, 等同于赋值1. 等效于: (a_num >> i) & 1        
        /* 寄存器的移位操作 */
        SCK_ACTION();
    }
    
    // ------------------------------- COM公共端 (选择哪几个位)    
    // 0b0000 1000
    for(i = 7; i >= 0; i--){
        // 先发字节的高位, 先考虑发1的情况(1是点亮公共端, 拉高)
//        NIX_DI = (b_idx >> i) & 1;
        NIX_DI = GET_BIT_VAL(b_idx, i);
        /* 寄存器的移位操作 */
        SCK_ACTION();
    }
    
    // ------------------------
    /* 寄存器的锁存操作, 所有的IO口并行输出 */
    RCK_ACTION();
}

/**********************************************************
 * @brief 在指定位置pos显示指定内容num_id, 每次只显示1个数字
 * @param num_id 要显示的内容的下标[0,9] [10, 19] ...
 * @param pos 显示的位置 [0, 7]
 **********************************************************/
void NIXIE_display(u8 num_id, u8 pos){
    u8 a_num = LED_TABLE[num_id]; // 根据下标取出对应Hex,确定显示内容
    u8 b_idx = 1 << pos;          // 在哪个位置显示内容
    
    NIXIE_show(a_num, b_idx);
}

//NIXIE.c(82): warning C316: unterminated conditionals