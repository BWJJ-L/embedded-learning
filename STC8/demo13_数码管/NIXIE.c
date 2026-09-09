#include "NIXIE.h"
#include "GPIO.h"

#define get_Value(Tube, Position)  ((Tube >> Position) & 0x01)

static void GPIO_config(void) {
    NIXIE_GPIO_INIT;
}

void Nixie_GPIO_INIT(void) {
    GPIO_config();
}

#define NIXIE_DATA_OUTPUT(byte) do{     \
        int8 i;                         \
        for (i = 7; i >= 0; i--) {      \
        NIX_DI = get_Value(byte, i);    \
        NIX_SCK = 0;                    \
        NOP2();                         \
        NIX_SCK = 1;                    \
        NOP2();                         \
    }                                   \
}while(0)

#define NIXIE_RCK_OPERATION   do{    \
    NIX_RCK = 0;                     \
    NOP2();                          \
    NIX_RCK = 1;                     \
    NOP2();                          \
}while(0)

// 索引对应表格参见：
// https://www.yuque.com/icheima/stc8h/kmz2mllvxs1uvdfy#lLhhp
u8 code LED_TABLE[] = {
    // 0    1    2  -> 9    (索引012...9)
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90,
    // 0. 1. 2. -> 9.   (索引10,11,12....19)
    0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10,
    // . -                      (索引20,21)
    0x7F, 0xBF,
    // AbCdEFHJLPqU     (索引22,23,24....33)
    0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0x89, 0xF1, 0xC7, 0x8C, 0x98, 0xC1
};

void Nixie_Operation(u8 Nixie_Index, u8 Switch_Index) {
    NIXIE_DATA_OUTPUT(LED_TABLE[Nixie_Index]);

    NIXIE_DATA_OUTPUT(1 << Switch_Index);

    NIXIE_RCK_OPERATION;
}



void Nixie_Display(u8 Nixie_Index, u8 Switch_Index) {

    Nixie_Operation(Nixie_Index, Switch_Index);
}