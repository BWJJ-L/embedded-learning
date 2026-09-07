#include "MATRIXKEY.h"


u16 states = 0xFFFF;

#define LAST_STATE(pos)     ((states >> pos) & 1)

#define SET_STATE(pos, now_state)   (states = ((states & ~(1 << pos)) | (now_state << pos)))

void ROW_OUT(u8 row) {
    ROW1 = (row == 0) ? 0 : 1;
    ROW2 = (row == 1) ? 0 : 1;
    ROW3 = (row == 2) ? 0 : 1;
    ROW4 = (row == 3) ? 0 : 1;
}

u8 COL_IN(u8 col) {
    switch(col) {
        case 0: return COL1;
        case 1: return COL2;
        case 2: return COL3;
        case 3: return COL4;
        default: return 0;
    }
}

void MK_init(void) {
    MK_GPIO_config;
}

void MK_key_state(MK_CallBack callback) {
    u8 row = 0, col = 0, pos = 0;


    for (row = 0; row < ROW_NUM; row++) {
        ROW_OUT(row);

        // 确认ROW电平变换完毕, 电平稳定
        //NOP2();      //可加可不加，后面还有代码做了延时

        for (col = 0; col < COL_NUM; col++) {

            if (LAST_STATE(pos) != COL_IN(col)) {
                SET_STATE(pos, COL_IN(col));

                if (COL_IN(col) == DOWN) {
//                    printf("Key (%d行%d列) 按下!!!\n", (int)(row + 1), (int)(col + 1));
                    callback(row, col, DOWN);
                } else {
//                    printf("Key (%d行%d列) 抬起!\n", (int)(row + 1), (int)(col + 1));
                    callback(row, col, UP);
                }
            }
            pos++;
        }

    }

}
