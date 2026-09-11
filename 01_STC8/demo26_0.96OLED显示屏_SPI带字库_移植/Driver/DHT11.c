#include "DHT11.h"
#include "GPIO.h"
#include "delay.h"

#define DHT     P46

static void GPIO_config(void) {
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_6;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	    //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//初始化    
}

void DHT11_init(){
    GPIO_config();
}


void Delay1us(void)	//@24.000MHz
{
	unsigned char data i;

	i = 4;
	while (--i);
}

void delay_1us(void){
//    NOP12();        // 1us -> 1000ns   small
    NOP7();        // 1us -> 1000ns   large
//    NOP6();        // 1us -> 1000ns 多加最大范围判定时, 使用此NOP
    // 41.67ns * 12 = 500ns 
}

// 等待电平变换
#define wait_level_change(level, min, max, desc)                                                         \
    do{                                                                                                  \
        cnt = 0; /*确保开始是0*/                                                                          \
        while(DHT == level){                                                                             \
            /*每循环一次,代表过去了1us,通过cnt记录时间*/                                                   \
            delay_1us();                                                                                 \
            cnt++;                                                                                       \
        };                                                                                               \
                                                                                                         \
        /*不符合目标范围, 及时短路返回, 避免代码嵌套*/                                                     \
        if(cnt < min || cnt > max){                                                                      \
            printf("err: 时间[%dus], 不满足 %s [%dus, %dus] line: %d\n", cnt, desc, (int)min, (int)max, (int)__LINE__);    \
            return -2;                                                                                      \
        }                                                                                                \
    }while(0)


int8 on_read_dht11(u8 dat[]){
    u16 cnt = 0; // 计数器, 每+1, 代表时间过了1us
    int8 i, j;
    
    // 1. 主机发起起始信号: 拉低 18ms, 30ms
    DHT = 0;
    delay_ms(20);
    DHT = 1;
    
    // 主动延时, 确保DHT被拉高
    NOP2();
    
    // 2. 主机释放总线 (13us, 35us)
    cnt = 0; // 确保开始是0, 同时, 也让DHT有时间真正拉起来
    while(DHT == 1 && cnt < 45){
        // 每循环一次,代表过去了1us,通过cnt记录时间
        delay_1us();
        cnt++;
    };
    // 如果不符合目标范围, 及时短路返回, 避免代码嵌套
    if(cnt < 6 || cnt > 35){
        printf("err: 时间[%dus], 不满足 主机释放总线时间[%dus, %dus]\n", cnt, (int)6, (int)35); 
        return -1;
    }
    
    // 不要在此过程中随意打日志, 因为会消耗时间, 影响cnt计数
    
    // 3. 响应低电平时间 83us, [78, 88]us, 当前0, 直到1, 结束循环
    wait_level_change(0, 78, 88, "响应低电平时间");
    
    // 4. 响应高电平时间 87us, [78, 88]us, 当前1, 直到0, 结束循环
    wait_level_change(1, 77, 95, "响应高电平时间");
    
    // 5. 解析40bit的数据(5Byte * 8bit)
    // 外循环: 1次, 接收处理1个byte字节(一共5个字节)
    for(i = 0; i < 5; i++){ // 0,1,2,3,4
    
        // 内循环: 1次, 接收处理1个bit位(每个字节8bit)
        for(j = 7; j >= 0; j--){ // 7,6,5,4,2,1,0 先收到高位
            // 一个bit信号由一低一高的电平组成: 低电平一样长(54us), 区别在于高电平
            
            // 数据信号: 低电平时间 54us [50, 58]us 当前0, 直到1
            wait_level_change(0, 46, 62, "Data信号低电平时间");
            
//            printf("dat[i]: %02X\n", (int)dat[i]);
        
            // 数据信号: 高电平时间 [23, 74]us 当前1, 直到0
            wait_level_change(1, 20, 74, "Data信号高电平时间");
            
            // 信号0: cnt 24us左右 [23, 27]
            // 信号1: cnt 71us左右 [68, 74]
            // 假如收到的数据  0b 1001 1010 -> dat[i]
            // 0b 0 0 0 0 - 0 0 0 0 默认值
            // 0b 1 0 0 0 - 0 0 0 0 j = 7
            // 0b 1 0 0 0 - 0 0 0 0 j = 6
            // 0b 1 0 0 0 - 0 0 0 0 j = 5
            // 0b 1 0 0 1 - 0 0 0 0 j = 4
            // 0b 1 0 0 1 - 1 0 0 0 j = 3
            // 0b 1 0 0 1 - 1 0 0 0 j = 2
            // 0b 1 0 0 1 - 1 0 1 0 j = 1
            // 0b 1 0 0 1 - 1 0 1 0 j = 0
            
            // 通过高电平时长cnt, 区分是0还是1 (是0就不管, 默认dat存的都是0)
            // (24 + 71) / 2 = 47.5
            if(cnt > 47){ // 信号1: 指定置1
                dat[i] |= ( 1 << j ); 
            }
        }

    }
    // 主机拉高释放总线(可选)
    DHT = 1;
        
    printf("cnt -> %d us\n", cnt);
    // 打印5个字节的数据
    printf("dat-> ");
    for(i = 0; i < 5; i++){
        printf("%d ", (int)dat[i]);
    }
    printf("\n");
    
    //  0x01D3
    //& 0x00FF
    //      D3
    // 校验数据: 8bit 湿度整数数据 + 8bit 湿度小数数据 + 8bit 温度整数数据 + 8bit 温度小数数据”8bit 校验位等于所得结果的末 8 位。
    if(((dat[0] + dat[1] + dat[2] + dat[3]) & 0xFF) != dat[4]){
        printf("校验失败: %d!\n", (int)__LINE__);
        return -3;
    }
    
    printf("校验通过: %d!\n", (int)__LINE__);

    return 0;
}

int8 DHT11_get_info(float* p_humidity, float* p_temperature){
    
    u8 dat[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
    
    float humidity; // 湿度
    float temperature; // 温度    
    int8 rst; // rst -> result
    
    rst = on_read_dht11(dat);
    
    // 0b 1000 0100
    //&0b 0111 1111 -> 0x7F 只保留低7位
    // 0b 0000 0100
    
    if(rst != SUCCESS){         
        printf("读取温湿度信息失败, 错误码: %d\n", (int) rst);
        return rst;
    }
        
    // 读取成功, 解析数据
    
    // 湿度高8位为 整数部分数据
    humidity = dat[0];
    
    // 模拟负温度
//        dat[3] |= (1 << 7);
    
    // 温度高8位 整数部分, 低8位 小数部分
    // 整数部分 + 小数部分(低7位) * 0.1
    temperature = dat[2] + (dat[3] & 0x7F) * 0.1f;
    
    // 如果温度 小数部分 最高位是1, 表示温度为负
    if((dat[3] >> 7) & 0x01){ // (dat[3] & 0x80) == 0x80
        temperature *= -1;    // 取反, 变负数
    }
    
    *p_humidity = humidity;
    *p_temperature = temperature;
    
    return rst;    
}




