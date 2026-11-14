/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：club.szlcsc.com
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * Change Logs:
 * Date           Author       Notes
 * 2023-11-02     LCKFB-yzh    first version
 */
#include "bsp_ws2812.h"
#include "board.h"
#include "stdio.h"
#include "math.h"


unsigned char LedsArray[WS2812_MAX * 3];      //定义颜色数据存储数组
unsigned int  ledsCount   = WS2812_NUMBERS;   //定义实际彩灯默认个数
unsigned int  nbLedsBytes = WS2812_NUMBERS*3; //定义实际彩灯颜色数据个数

/******************************************************************
 * 函 数 名 称：WS2812_GPIO_Init
 * 函 数 说 明：对WS2812引脚初始化
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void WS2812_GPIO_Init(void)
{
    rcu_periph_clock_enable(RCU_DIN);   // 开启时钟
    /* 配置DIN为输出模式 上拉模式 */
    gpio_mode_set(PORT_DIN,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_DIN);
    /* 配置DIN为推挽输出 50MHZ */
    gpio_output_options_set(PORT_DIN,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_DIN);
    /* 默认输出低电平 */
    gpio_bit_reset(PORT_DIN, GPIO_DIN);
}
/******************************************************************
 * 函 数 名 称：rgb_SetColor
 * 函 数 说 明：设置彩灯颜色
 * 函 数 形 参：LedId控制的第几个灯  color颜色数据
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：在这里我将绿和红色进行颠倒，这样比较符合我们日常生活的红绿蓝的顺序
******************************************************************/

void rgb_SetColor(unsigned char LedId, unsigned long color)
{
    if( LedId > ledsCount )
    {
        return;    //to avoid overflow
    }
    LedsArray[LedId * 3]     = (color>>8)&0xff;
    LedsArray[LedId * 3 + 1] = (color>>16)&0xff;
    LedsArray[LedId * 3 + 2] = (color>>0)&0xff;
}

/******************************************************************
 * 函 数 名 称：rgb_SetRGB
 * 函 数 说 明：设置彩灯颜色(三原色设置)
 * 函 数 形 参：LedId控制的第几个灯 red红色数据  green绿色数据  blue蓝色数据
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void rgb_SetRGB(unsigned char LedId, unsigned long red, unsigned long green, unsigned long blue)
{
    unsigned long Color=red<<16|green<<8|blue;
    rgb_SetColor(LedId,Color);
}

/******************************************************************
 * 函 数 名 称：rgb_SendArray
 * 函 数 说 明：发送彩灯数据
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void rgb_SendArray(void)
{
    unsigned int i;
    //发送数据
    for(i=0; i<nbLedsBytes; i++)
        Ws2812b_WriteByte(LedsArray[i]);
}

/******************************************************************
 * 函 数 名 称：RGB_LED_Reset
 * 函 数 说 明：复位ws2812
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：低电平280us以上
******************************************************************/
void RGB_LED_Reset(void)
{
        RGB_PIN_L();              
        delay_us(281);
}

/******************************************************************
 * 函 数 名 称：Ws2812b_WriteByte
 * 函 数 说 明：向WS2812写入单字节数据
 * 函 数 形 参：byte写入的字节数据
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：1码的时序 = 高电平580ns~1us    再低电平220ns~420ns
 *              0码的时序 = 高电平220ns~380ns  再低电平580ns~1us
******************************************************************/
void Ws2812b_WriteByte(unsigned char byte)
{
    int i = 0, j = 0;
        for(i = 0; i < 8; i++ )
        {
                if( byte & (0x80 >> i) )//当前位为1
                { 
                        RGB_PIN_H();
                        delay_us(1);//0.75us
                        RGB_PIN_L();
                        for(j = 0; j < 8; j++ );//0.25us
                }
                else//当前位为0
                {
                        RGB_PIN_H();
                        for(j = 0; j < 8; j++ );//0.25us
                        RGB_PIN_L();
                        delay_us(1);//0.833us
                }
        }
}
    
     