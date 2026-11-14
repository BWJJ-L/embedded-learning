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
#include "board.h"
#include "bsp_ws2812.h"

uint8_t Co = 100;
unsigned int buff[]={RED,GREEN,BLUE,WHITE,YELLOW};

int main(void)
{
	int i = 0;
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // 优先级分组
    board_init();
	bsp_uart_init();
	
    WS2812_GPIO_Init();
    printf("Start--->\r\n");
     
    while(1)
    {
        for( i = 0; i < 8; i++ )
        {
            rgb_SetColor(i,buff[i%3]);
            rgb_SendArray();
			delay_ms(10);
        }
		delay_ms(3000);
		
		i = 0;
		while( 1 )
		{
			rgb_SetColor(0,BLACK);
			rgb_SetColor(1,BLACK);
            rgb_SetColor(2,BLACK);
            rgb_SetColor(3,BLACK);
            rgb_SetColor(4,BLACK);
            
            
			rgb_SendArray();
			delay_ms(200);
			Co--;
			i++;
		}
		Co = 100;
    }
}


