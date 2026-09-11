#include <config.h>
#include <GPIO.H>
#include <Delay.h>

//PU  -> Pull Up    上拉准双向口模式
//PP  -> Push Pull  推挽输出模式
//HIZ -> High Z     高阻输入/浮空输入模式
//OD  -> Open Drain 开漏输出模式

void main(void) {

    //方式一：
//    P5M1 = 0x00;
//    P5M0 = 0x00;


    //方式二：
//    P5M1 &= ~0x08;  //(1 << 3) == 0x08
//    P5M0 |=  0x08;


    //方式三：定义寄存器  sfr sbit


    //方式四：使用初始化函数
//    GPIO_InitTypeDef GPIOx;
//    GPIOx.Mode = GPIO_OUT_PP;
//    GPIOx.Pin  = GPIO_Pin_3;
//    GPIO_Inilize(GPIO_P5, &GPIOx);


    //方式五：宏函数
    P5_MODE_OUT_PP(GPIO_Pin_3);


    while (1) {
        P53 = !P53;

        delay_ms(250);  //函数内部参数为u char类型
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }
}
