#ifndef __I2C_OLED_H_
#define __I2C_OLED_H_

#include "Config.h"

 
#define  u8 unsigned char 
#define  u16 unsigned int
#define  u32 unsigned int
	
#define I2C_OLED_CMD  0	//写命令
#define I2C_OLED_DATA 1	//写数据

sbit I2C_OLED_SCL=P3^2;//SCL
sbit I2C_OLED_SDA=P3^3;//SDA
//sbit I2C_OLED_RES =P1^2;//RES

//-----------------OLED端口定义----------------

#define I2C_OLED_SCL_Clr() I2C_OLED_SCL=0
#define I2C_OLED_SCL_Set() I2C_OLED_SCL=1

#define I2C_OLED_SDA_Clr() I2C_OLED_SDA=0
#define I2C_OLED_SDA_Set() I2C_OLED_SDA=1

#define I2C_OLED_RES_Clr() I2C_OLED_RES=0
#define I2C_OLED_RES_Set() I2C_OLED_RES=1



//OLED控制用函数
//void delay_ms(unsigned int ms);
void I2C_OLED_ColorTurn(u8 i);
void I2C_OLED_DisplayTurn(u8 i);
void I2C_OLED_WR_Byte(u8 dat,u8 cmd);
void I2C_OLED_Set_Pos(u8 x, u8 y);
void I2C_OLED_Display_On(void);
void I2C_OLED_Display_Off(void);
void I2C_OLED_Clear(void);
void I2C_OLED_ShowChar(u8 x,u8 y,u8 chr,u8 sizey);
u32 I2C_OLED_pow(u8 m,u8 n);
void I2C_OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey);
void I2C_OLED_ShowString(u8 x,u8 y,u8 *chr,u8 sizey);
void I2C_OLED_ShowChinese(u8 x,u8 y,u8 no,u8 sizey);
void I2C_OLED_DrawBMP(u8 x,u8 y,u8 sizex, u8 sizey,u8 BMP[]);
void I2C_OLED_Init(void);

#endif