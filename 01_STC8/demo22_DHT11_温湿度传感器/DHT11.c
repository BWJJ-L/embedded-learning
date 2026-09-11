#include "DHT11.h"

#define DHT P46

static void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin  = GPIO_Pin_6;      //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;      //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//初始化
}


void DHT11_init(void) {
    EAXSFR();
    GPIO_config();
}


void delay_1us(void) {
    NOP12();
    //12 * 41.67ns = 500ns
    /*
    调用函数开销（LCALL）：执行函数跳转，消耗 4个时钟周期 ≈ 166.7ns
    返回函数开销（RET）：执行函数返回，消耗 4个时钟周期 ≈ 166.7ns
    合计总耗时：500 + 166.7 + 166.7 = 833.4ns ≈ 0.83μs
    虽然理论计算是0.83μs，但在工程实践中，加上指令读取、总线等待或流水线填充的微小延迟，实际时间非常接近1微秒
    */
    //NOP6();   // 1us -> 1000ns 判断响应电平的地方，多加最大范围判定时,使用此NOP
}


#define wait_level_change(level, min, max, desc)                                                              \
do{                                                                                                           \
    /*确保cnt开始是0，也是拖一会运行时间，让低电平变成高电平更久一会*/                                           \
    cnt = 0;                                                                                                  \
    while(level == DHT) {                                                                                     \
    /*每循环一次,代表过去了1us,通过cnt记录时间*/                                                                \
        delay_1us();                                                                                          \
        cnt++;                                                                                                \
    }                                                                                                         \
    /*不符合目标范围, 及时短路返回, 避免代码嵌套*/                                                              \
    if(cnt < min || cnt > max) {                                                                              \
        printf("Error : 时间[%dus],不满足%s[%dus, %dus] line: %d\n",cnt,desc,(int)min,(int)max,(int)__LINE__); \
        return -2;                                                                                            \
    }                                                                                                         \
}while(0) 


int8 on_read_DHT11(u8 dat[]) {
    u16 cnt = 0;
    u8 i;
    int8 j;
    
    // 1. 主机发起起始信号: 拉低 18ms, 30ms
    //起始信号:微处理器把数据总线（SDA）拉低一段时间至少18ms（最大不得超过30ms），通知传感器准备数据。 
    DHT = 0;
    delay_ms(20);
    DHT = 1;
    
    // 2. 主机释放总线 (13us, 35us)
    //主机释放总线时间 min:10us  type:13us  max:35us
    cnt = 0; //确保开始是0,同时也让DHT有时间真正拉起来
    while(1 == DHT && cnt < 45) {
        // 每循环一次,代表过去了1us,通过cnt记录时间
        delay_1us();
        cnt++;
    }
    // 如果不符合目标范围, 及时短路返回, 避免代码嵌套
    if(cnt < 6 || cnt > 35) {
        printf("Error:时间[%dus],不满足主机释放总线时间[%dus, %dus] line:%d\n",cnt,(int)6,(int)35,(int)__LINE__);
        return -1;
    }
    
    // 不要在此过程中随意打日志, 因为会消耗时间, 影响cnt计数
    
    // 3.响应信号:传感器把数据总线（SDA）拉低83μs，再接高87μs以响应主机的起始信号。 
    // 响应低电平时间 min:78 type:83 max:88, [78, 88]us, 当前0, 直到1, 结束循环
    wait_level_change(0, 73, 93, "低电平响应时间");
    // 响应高电平时间 min:80 type:87 max:92, [82, 92]us, 当前1, 直到0, 结束循环
    wait_level_change(1, 75, 97, "高电平响应时间");
    
    
    // 4.数据格式:收到主机起始信号后，传感器一次性从数据总线（SDA）串出40位数据，高位先出
    // 解析40bit的数据(5Byte * 8bit)
    // 外循环: 1次, 接收处理1个byte字节(一共5个字节)
    for(i = 0;i < 5;i++) {
        // 内循环: 1次, 接收处理1个bit位(每个字节8bit)
        for(j = 7;j >= 0;j--) {
            // 数据信号: 信号"0"和"1"低电平时间 min:50 type:54 max:58,  [50, 58]us 当前0, 直到1
            wait_level_change(0, 45, 63, "Data信号低电平响应时间");
            // 数据信号: 高电平时间 [23, 74]us 当前1, 直到0, 包括的是"0"和"1"的高电平时间
            wait_level_change(1, 23, 74, "Data信号高电平响应时间");
            
            // 通过高电平时长cnt, 区分是"0"还是"1" (是0就不管, 默认dat存的都是0，是1就变成1)
            // (24 + 71) / 2 = 47.5   24us是信号"1"高电平type时间，71us是信号"0"高电平type时间
            if(cnt > 47) {
                dat[i] |= (1 << j);
            }
        }
    }
    //主机拉高释放总线
    DHT = 1;
    
    printf("cnt->%dus\n",cnt);
    
    //校验位数据定义:“8bit湿度整数数据 + 8bit湿度小数数据 + 8bit温度整数数据 + 8bit温度小数数据”8bit校验位等于所得结果的末8位
    if(((dat[0] + dat[1] + dat[2] + dat[3]) & 0xFF) != dat[4]) {
        printf("校验失败:%d！\n",(int)__LINE__);
        return -3;
    }
    
    printf("校验通过:%d！\n",(int)__LINE__);
    return SUCCESS;
}


int8 DHT11_get_info(float* p_humidity, float* p_temperature) {
    int8 result;
    u8 dat[] = {0x00, 0x00, 0x00, 0x00, 0x00};
    
    result = on_read_DHT11(dat);
    
    if(result != SUCCESS) {
        printf("读取温湿度失败，错误码：%d",(int)result);
    }
    
    //湿度高位为湿度整数部分数据，湿度低位为湿度小数部分数据，其中湿度小数部分为0
    *p_humidity = dat[0];
    
    //温度高位为温度整数部分数据，温度低位为温度小数部分数据，且温度低8位的最高位置为1则表示负温度，否则为正温度
    *p_temperature = dat[2] + (dat[3] & 0x7F) * 0.1f;
    
    if(dat[3] & 0x80) {
        *p_temperature *= -1;
    }
    
    return result;
}