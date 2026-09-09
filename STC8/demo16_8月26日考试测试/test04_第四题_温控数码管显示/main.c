#include "Config.h"
#include "GPIO.h"
#include "NVIC.h"
#include "UART.h"
#include "Switch.h"
#include "ADC.h"
#include "Timer.h"
#include "STC8H_PWM.h"
#include "Delay.h"
#include "NIXIE.h"

void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin  = GPIO_Pin_4;       //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_HighZ;       //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;      //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化

    Nixie_GPIO_INIT();
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

/******************* AD配置函数 *******************/
void    ADC_config(void) {
    ADC_InitTypeDef     ADC_InitStructure;      //结构定义

    ADC_InitStructure.ADC_SMPduty   = 31;       //ADC 模拟信号采样时间控制, 0~31（注意： SMPDUTY 一定不能设置小于 10）
    ADC_InitStructure.ADC_CsSetup   = 0;        //ADC 通道选择时间控制 0(默认),1
    ADC_InitStructure.ADC_CsHold    = 1;        //ADC 通道选择保持时间控制 0,1(默认),2,3
    ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X16T;       //设置 ADC 工作时钟频率 ADC_SPEED_2X1T~ADC_SPEED_2X16T
    ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;  //ADC结果调整,  ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
    ADC_Inilize(&ADC_InitStructure);        //初始化
    ADC_PowerControl(ENABLE);               //ADC电源开关, ENABLE或DISABLE
    NVIC_ADC_Init(DISABLE, Priority_0);     //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

void    Timer_config(void) {
    TIM_InitTypeDef     TIM_InitStructure;                      //结构定义
    //定时器0做16位自动重装, 中断频率为1000HZ
    TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;  //指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;     //指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut    = DISABLE;              //是否输出高速脉冲, ENABLE或DISABLE
    TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000UL);       //初值,
    TIM_InitStructure.TIM_Run       = ENABLE;               //是否初始化后启动定时器, ENABLE或DISABLE
    Timer_Inilize(Timer0, &TIM_InitStructure);              //初始化Timer0    Timer0,Timer1,Timer2,Timer3,Timer4
    NVIC_Timer0_Init(DISABLE, Priority_0);      //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

void Timer0_ISR_Handler(void) interrupt TMR0_VECTOR {       //进中断时已经清除标志

}

u16 code temp_table[] = {
    58354, // -55 0
    55464, // -54 1
    52698, // -53 2
    50048, // -52 3
    47515, // -51 4
    45097, // -50 5
    42789, // -49 6
    40589, // -48 7
    38492, // -47 8
    36496, // -46 9
    34597, // -45 10
    32791, // -44
    31075, // -43
    29444, // -42
    27896, // -41
    26427, // -40
    25034, // -39
    23713, // -38
    22460, // -37
    21273, // -36
    20148, // -35
    19083, // -34
    18075, // -33
    17120, // -32
    16216, // -31
    15361, // -30
    14551, // -29
    13785, // -28
    13061, // -27
    12376, // -26
    11728, // -25
    11114, // -24
    10535, // -23
    9986,  // -22
    9468,  // -21
    8977,  // -20
    8513,  // -19
    8075,  // -18
    7660,  // -17
    7267,  // -16
    6896,  // -15
    6545,  // -14
    6212,  // -13
    5898,  // -12
    5601,  // -11
    5319,  // -10
    5053,  // -9
    4801,  // -8
    4562,  // -7
    4336,  // -6
    4122,  // -5
    3920,  // -4
    3728,  // -3
    3546,  // -2
    3374,  // -1
    3211,  // 0
    3057,  // 1
    2910,  // 2
    2771,  // 3
    2639,  // 4
    2515,  // 5
    2396,  // 6
    2284,  // 7
    2177,  // 8
    2076,  // 9
    1978,  // 10
    1889,  // 11
    1802,  // 12
    1720,  // 13
    1642,  // 14
    1568,  // 15
    1497,  // 16
    1430,  // 17
    1366,  // 18
    1306,  // 19
    1248,  // 20
    1193,  // 21
    1141,  // 22
    1092,  // 23
    1044,  // 24
    1000,  // 25
    957,   // 26
    916,   // 27
    877,   // 28
    840,   // 29
    805,   // 30
    771,   // 31
    739,   // 32
    709,   // 33
    679,   // 34
    652,   // 35
    625,   // 36
    600,   // 37
    576,   // 38
    552,   // 39
    530,   // 40
    509,   // 41
    489,   // 42
    470,   // 43
    452,   // 44
    434,   // 45
    417,   // 46
    401,   // 47
    386,   // 48
    371,   // 49
    358,   // 50
    344,   // 51
    331,   // 52
    318,   // 53
    306,   // 54
    295,   // 55
    284,   // 56
    274,   // 57
    264,   // 58
    254,   // 59
    245,   // 60
    236,   // 61
    228,   // 62
    220,   // 63
    212,   // 64
    205,   // 65
    198,   // 66
    191,   // 67
    184,   // 68
    178,   // 69
    172,   // 70
    166,   // 71
    160,   // 72
    155,   // 73
    150,   // 74
    145,   // 75
    140,   // 76
    135,   // 77
    131,   // 78
    126,   // 79
    122,   // 80
    118,   // 81
    115,   // 82
    111,   // 83
    107,   // 84
    104,   // 85
    101,   // 86
    97,    // 87
    94,    // 88
    91,    // 89
    89,    // 90
    86,    // 91
    83,    // 92
    81,    // 93
    78,    // 94
    76,    // 95
    74,    // 96
    71,    // 97
    69,    // 98
    67,    // 99
    65,    // 100
    63,    // 101
    61,    // 102
    60,    // 103
    58,    // 104
    56,    // 105
    55,    // 106
    53,    // 107
    52,    // 108
    50,    // 109
    49,    // 110
    47,    // 111
    46,    // 112
    45,    // 113
    43,    // 114
    42,    // 115
    41,    // 116
    40,    // 117
    39,    // 118
    38,    // 119
    37,    // 120
    36,    // 121
    35,    // 122
    34,    // 123
    33,    // 124
    32,    // 125
};

#define ABS(x) ((x) > 0 ? (x) : -(x))

/**********************************************************
 * @brief 根据电阻值(K欧)x100计算得到温度值
 * @param 电阻值(K欧)x100
 * @return 温度索引
 **********************************************************/
int search_temp(float ntc_Rx100) {
    int min_index = 0, i = 0;
    // 记录最小距离
    float min_distance = ABS(ntc_Rx100 - temp_table[0]);
    // 计算数组长度
    int len = sizeof(temp_table) / sizeof(temp_table[0]);

    for (i = 1; i < len; i++) {
        // 逐个计算 ntc_Rx100 和每个数的距离(求绝对值)
        float distance = ABS(ntc_Rx100 - temp_table[i]);

        // 判断是否距离更近
        if (distance < min_distance) {
            // 记录更近的距离
            min_distance = distance;
            // 记录索引位置
            min_index = i;
        }
//      printf("距离[%d] -> %.1f index: %d\n", temp_table[i], distance, (int)i);
    }

    return min_index;
}


int main(void) {
    u16 adc_Value;
    float ntc_V, ntc_R;
    int temp;
    u8 a;
    u8 b;

    EAXSFR();

    GPIO_config();
    UART_config();
    ADC_config();
    Timer_config();

    EA = 1;



    while (1) {
        adc_Value = Get_ADCResult(ADC_CH12);
        ntc_V = adc_Value * 2.5 / 4096;
        ntc_R = ntc_V * 10 / (3.3 - ntc_V);
        temp = search_temp(ntc_R * 100) - 55;

        a = temp / 10;
        b = temp % 10;
        
        Nixie_Display(a, 0);
        delay_ms(5);
        Nixie_Display(b, 1);
        delay_ms(5);
    }
}