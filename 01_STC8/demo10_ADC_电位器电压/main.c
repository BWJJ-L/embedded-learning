#include "Config.h"
#include	"ADC.h"
#include	"GPIO.h"
#include	"Delay.h"
#include	"UART.h"
#include	"NVIC.h"
#include	"Switch.h"

/*******************
读取电位器电压:

ADC13	P0.5

可测量的电压范围: 

0 ->  2.5V Voltage
0 ->  4095 ADC_VALUE

电位器范围: 1.65V -> 3.3V


1.65V -> 2.5V

*******************/

void GPIO_config(){

    // 配置P05为高阻输入模式
    P0_MODE_IN_HIZ(GPIO_Pin_5);
    
    // P3.0,P3.1 设置为准双向口
	P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);	
}
/******************* AD配置函数 *******************/
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;		//结构定义

    // 通过输入引脚给电容充电, 时间够长, 才能确保电容电压和外部输入电压一致
	ADC_InitStructure.ADC_SMPduty   = 31;		//ADC 模拟信号采样时间控制, 0~31（注意： SMPDUTY 一定不能设置小于 10）
    
    // 内部多个通道选择, 需要一段时间稳定(1个ADC时钟 n+1)
	ADC_InitStructure.ADC_CsSetup   = 0;		//ADC 通道选择时间控制 0(默认),1
    
    // 在真正采样之前, 需要保持一段时间(2个ADC时钟 n+1)
	ADC_InitStructure.ADC_CsHold    = 1;		//ADC 通道选择保持时间控制 0,1(默认),2,3
    
    // 配置速度 2X1T 最快, 可能会损失精度
	ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X16T;		//设置 ADC 工作时钟频率	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC结果调整,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_Inilize(&ADC_InitStructure);		//初始化
	ADC_PowerControl(ENABLE);				//ADC电源开关, ENABLE或DISABLE
	NVIC_ADC_Init(DISABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

/***************  串口初始化函数 *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2, 所以不用选择)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口2 USART1,USART2,USART3,USART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
	UART1_SW(UART1_SW_P30_P31);	//通道切换，选择P3.0, P3.1引脚通信
}

int main() {
    float rst_V = 0, ref_V = 0;
    u16 adc_value;
    EAXSFR();

    GPIO_config();
    UART_config();
    ADC_config();
    
    EA = 1;

    while(1) {
        // 读取 ADC13 通道的数值 (P05)
        adc_value = Get_ADCResult(ADC_CH13);
        
        // 得到12位的数据 [0, 4095]
        // adc_value / 4096 = rst_V / 2.5
        rst_V = adc_value * 2.5 / 4096;
        printf("ADC13: %d, rst_V: %.2f\n", adc_value, rst_V);
        
        // 读取 ADC15 通道的数值
        adc_value = Get_ADCResult(ADC_CH15);
        // adc_value / 4096 = rst_V / 2.5
        rst_V = adc_value * 2.5 / 4096;
        printf("ADC15: %d, rst_V: %.2f\n", adc_value, rst_V);
        
        // 已知: 内部参考电压:1.18V, 倒推出Vref引脚接入电压
        ref_V = 1.18 * 4096 / adc_value;
        printf("Vref外接电压: %.2f V\n", ref_V);
         
        delay_ms(250);
        delay_ms(250);
    }
}