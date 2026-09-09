#include "PCF8563.h"

static void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin  = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.Mode = GPIO_OUT_OD;
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);
}

/****************  I2C初始化函数 *****************/
void I2C_config(void)
{
	I2C_InitTypeDef		I2C_InitStructure;

	I2C_InitStructure.I2C_Mode      = I2C_Mode_Master;	// 主从选择   I2C_Mode_Master, I2C_Mode_Slave
	I2C_InitStructure.I2C_Enable    = ENABLE;			// I2C功能使能,   ENABLE, DISABLE
	I2C_InitStructure.I2C_MS_WDTA   = DISABLE;			// 主机使能自动发送,  ENABLE, DISABLE
	I2C_InitStructure.I2C_Speed     = 13;				// 总线速度=Fosc/2/(Speed*2+4),      0~63
    /*                                           速度 =  24M / 2 / (16 * 2 + 4)
                                                 速度 =  12M / 36  = 333.3Kbit/s
                                                    
                                                 400 = 12K / (Speed * 2 + 4)
                                                 400Kbit/s -> 13
                                                 100Kbit/s -> 58
    */
	I2C_Init(&I2C_InitStructure);
    
	NVIC_I2C_Init(I2C_Mode_Master,DISABLE,Priority_0);	//主从模式, I2C_Mode_Master, I2C_Mode_Slave; 中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

    // 引脚选择
	I2C_SW(I2C_P33_P32);					//I2C_P14_P15,I2C_P24_P25,I2C_P33_P32
}

void PCF8563_init(void) {
    EAXSFR();
    GPIO_config();
    I2C_config();
}

void PCF8563_set_Clock(Clock_t c) {
    u8 p[NUMBER] = {0};
    u8 C;
    
    p[0] = ((c.seconds / 10) << 4) | (c.seconds % 10);
    p[1] = ((c.minutes / 10) << 4) | (c.minutes % 10);
    p[2] = ((c.hours / 10) << 4)   | (c.hours % 10);
    p[3] = ((c.days / 10) << 4)    | (c.days % 10);
    p[4] = c.weekdays;
    
    C = (c.years >= 2100) ? 1 : 0;
    
    p[5] = (C << 7) |((c.months / 10) << 4)  | (c.months % 10);
    p[6] = ((c.years % 100 / 10) << 4) | (c.years % 10);
    
    I2C_WriteNbyte(PCF8563_ADDR, PCF8563_REG, p, NUMBER);
}

void PCF8563_get_Clock(Clock_t * c) {
    u8 p[NUMBER] = {0};
    u8 C;
    
    I2C_ReadNbyte(PCF8563_ADDR, PCF8563_REG, p, NUMBER);
    
    c->seconds  = ((p[0] >> 4)& 0x07) * 10 + (p[0] & 0x0F);
    c->minutes  = ((p[1] >> 4)& 0x07) * 10 + (p[1] & 0x0F);
    c->hours    = ((p[2] >> 4)& 0x03) * 10 + (p[2] & 0x0F);
    c->days     = ((p[3] >> 4)& 0x03) * 10 + (p[3] & 0x0F);
    c->weekdays = p[4];
    c->months   = ((p[5] >> 4)& 0x01) * 10 + (p[5] & 0x0F);
    c->years    = ((p[6] >> 4)& 0x0F) * 10 + (p[6] & 0x0F);
    
    C = p[5] & 0x80;
    
    c->years += (C == 0) ? 2000 : 2100;
}