#include "NIXIE.h"
#include "GPIO.h"

// ��byte�ֽ���, ȡ��ָ��posλ��ֵ
#define GET_BIT_VAL(byte, pos) (((byte) >> (pos)) & 1)

// SCK���к�(��׼д��)
#define SCK_ACTION() do {   \
    /* �Ĵ�������λ���� */   \
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

// �±��Ӧ���μ���
u8 code LED_TABLE[] = 
{
	// 0 	1	 2	-> 9	(����0,1,2...9)
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,
	// 0. 1. 2. -> 9.	(����10,11,12....19)
    0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
	// . -						(����20,21)
	0x7F, 0xBF,
	// AbCdEFHJLPqUo		(����22,23,24....33, 34, 35)
	0x88,0x83,0xC6,0xA1,0x86,0x8E,0x89,0xF1,0xC7,0x8C,0x98,0xC1,0xA3,0xFF
};

void NIXIE_init(void){
    NIXIE_GPIO_INIT();
}

void NIXIE_show(u8 a_num, u8 b_idx){
    char i; // ����ѭ��һ���������޷�����

    // ------------------------------- SEG��λ�� (��ʾ����)
    for(i = 7; i >= 0; i--){ // 7, 6, 5, 4, 3, 2, 1, 0
        // �ȷ��ֽڵĸ�λ, �ȿ��Ƿ�0�����(0�Ƕ�λ��, ����)
//        NIX_DI = a_num & (1 << i);     // �����Ÿ�ֵ��0, ��ͬ�ڸ�ֵ1. ��Ч��: (a_num >> i) & 1
        NIX_DI = GET_BIT_VAL(a_num, i);  // �����Ÿ�ֵ��0, ��ͬ�ڸ�ֵ1. ��Ч��: (a_num >> i) & 1        
        /* �Ĵ�������λ���� */
        SCK_ACTION();
    }
    
    // ------------------------------- COM������ (ѡ���ļ���λ)    
    // 0b0000 1000
    for(i = 7; i >= 0; i--){
        // �ȷ��ֽڵĸ�λ, �ȿ��Ƿ�1�����(1�ǵ���������, ����)
//        NIX_DI = (b_idx >> i) & 1;
        NIX_DI = GET_BIT_VAL(b_idx, i);
        /* �Ĵ�������λ���� */
        SCK_ACTION();
    }
    
    // ------------------------
    /* �Ĵ������������, ���е�IO�ڲ������ */
    RCK_ACTION();
}

/**********************************************************
 * @brief ��ָ��λ��pos��ʾָ������num_id, ÿ��ֻ��ʾ1������
 * @param num_id Ҫ��ʾ�����ݵ��±�[0,9] [10, 19] ...
 * @param pos ��ʾ��λ�� [0, 7]
 **********************************************************/
void NIXIE_display(u8 num_id, u8 pos){
    u8 a_num = LED_TABLE[num_id]; // �����±�ȡ����ӦHex,ȷ����ʾ����
    u8 b_idx = 1 << pos;          // ���ĸ�λ����ʾ����
    
    NIXIE_show(a_num, b_idx);
}

//NIXIE.c(82): warning C316: unterminated conditionals
