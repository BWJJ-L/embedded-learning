#include "Config.h"
#include "GPIO.h"
#include "delay.h"
#include "UART.h"
#include "Switch.h"
#include "NVIC.h"
#include "MATRIXKEY.h"
#include "NIXIE.h"
#include "OLED.h"
#include "I2C.h"

void GPIO_config(void) {
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
    
	GPIO_InitStructure.Pin  = GPIO_Pin_2 | GPIO_Pin_3;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_OUT_OD;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
}

void UART_config(void) {
    // >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine     COMx_InitStructure;                 //结构定义
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx; //模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;         //选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;           //波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;             //接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;            //波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);     //初始化串口1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE, Priority_1);    //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);     // 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

void	I2C_config(void)
{
	I2C_InitTypeDef		I2C_InitStructure;

	I2C_InitStructure.I2C_Mode      = I2C_Mode_Master;	//主从选择   I2C_Mode_Master, I2C_Mode_Slave
	I2C_InitStructure.I2C_Enable    = ENABLE;			//I2C功能使能,   ENABLE, DISABLE
	I2C_InitStructure.I2C_MS_WDTA   = DISABLE;			//主机使能自动发送,  ENABLE, DISABLE
	I2C_InitStructure.I2C_Speed     = 13;				//总线速度=Fosc/2/(Speed*2+4),      0~63
                                                        // 400k, 24M => 13
	I2C_Init(&I2C_InitStructure);
	NVIC_I2C_Init(I2C_Mode_Master,DISABLE,Priority_0);	//主从模式, I2C_Mode_Master, I2C_Mode_Slave; 中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

	I2C_SW(I2C_P33_P32);					//I2C_P14_P15,I2C_P24_P25,I2C_P33_P32
}

void sys_init(void) {
    EAXSFR();

    GPIO_config();
    UART_config();
    I2C_config();
    OLED_Init();

    EA = 1;
}

//最多15个任务，就是_task_ 0 到 _task_ 15
// 这里函数名可随意, 建议不要使用start, 会和I2C.h里的Start冲突
void task_0(void) _task_ 0 {
    //初始化
    sys_init();
    Nixie_GPIO_INIT();
    MK_init();
    //创建任务1
    os_create_task(1);
    //创建任务2
    os_create_task(2);
    //创建任务3
    os_create_task(3);
    //结束任务0
    os_delete_task(0);
}

u8 number[] = {2, 3};
u8 num = 23;
void KEY_scan(u8 row, u8 col, u8 state) {
    state = state;
    
    if(row == 0 && col == 0) {
        if(num >= 90) {
            num -= 90;
        } else {
            num += 10;
        }
        
    } else if(row == 0 && col == 1) {
        if(num == 99) {
            num = 0;
        } else {
            num += 1;
        }
        
    } else if(row == 1 && col == 0) {
        if(num <= 10) {
            num += 90;
        } else {
            num -= 10;
        }
    } else if(row == 1 && col == 1) {
        if(num == 0) {
            num = 99;
        } else {
            num -= 1;
        }
    }
    
    number[0] = num / 10;
    number[1] = num % 10;
    
    // 发信号, 通知oled屏幕刷新
    os_send_signal(3);
}
//任务1: 循环扫描矩阵键盘, 按下修改数字 (10ms)
void task_1(void) _task_ 1 {
    while (1) {
        MK_key_state(KEY_scan);
        
        os_wait2(K_TMO, 2);
    }
}
// 任务2: 数码管切换显示
void task_2(void) _task_ 2 {
    while (1) {
        u8 i = 0;
        for(i = 0;i < 2;i++) {
            Nixie_Display(number[i], i);
            os_wait2(K_TMO, 1);
        }
    }
}
// 任务3: oled屏幕刷新任务
void task_3(void) _task_ 3 {
    OLED_ColorTurn(0);
    OLED_DisplayTurn(0);
    while(1) {
        OLED_ShowNum(0, 0, num, 2, 16);
        // 等待刷新信号
        os_wait1(K_SIG);
    }
}