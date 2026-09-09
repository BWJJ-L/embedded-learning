#ifndef __SPI_OLED_H
#define __SPI_OLED_H

#include "Config.h"
#include "delay.h"

sbit SPI_OLED_SCL=P5^0;   // SCLK
sbit SPI_OLED_SDA=P1^3;   // MOSI
sbit SPI_OLED_DC=P1^6;      
sbit SPI_OLED_CS=P4^7;      
sbit SPI_OLED_READ_FS0=P1^1; // MISO
sbit SPI_OLED_ROM_CS=P1^0;

//#define u8  unsigned char
//#define u32 unsigned long
#define SPI_OLED_CMD  0   //Ð´ÃüÁî
#define SPI_OLED_DATA 1   //Ð´Êý¾Ý

#define SPI_OLED_SCL_Set()       SPI_OLED_SCL=1;
#define SPI_OLED_SCL_Clr()       SPI_OLED_SCL=0;

#define SPI_OLED_SDA_Set()       SPI_OLED_SDA=1;
#define SPI_OLED_SDA_Clr()       SPI_OLED_SDA=0;

#define SPI_OLED_DC_Set()        SPI_OLED_DC=1;
#define SPI_OLED_DC_Clr()        SPI_OLED_DC=0;

#define SPI_OLED_CS_Set()        SPI_OLED_CS=1;
#define SPI_OLED_CS_Clr()        SPI_OLED_CS=0;

#define SPI_OLED_ROM_CS_Set()    SPI_OLED_ROM_CS=1;
#define SPI_OLED_ROM_CS_Clr()    SPI_OLED_ROM_CS=0;

//void delay_ms(unsigned int ms);
void SPI_OLED_ColorTurn(u8 i);
void SPI_OLED_DisplayTurn(u8 i);
void SPI_OLED_WR_Byte(u8 dat,u8 cmd);
void SPI_OLED_Clear(void);
void SPI_OLED_address(u8 x,u8 y);
void SPI_OLED_Display_128x64(u8 *dp);
void SPI_OLED_Display_16x16(u8 x,u8 y,u8 *dp);
void SPI_OLED_Display_8x16(u8 x,u8 y,u8 *dp);
void SPI_OLED_Display_5x7(u8 x,u8 y,u8 *dp);
void Send_Command_to_ROM(u8 dat);
u8 Get_data_from_ROM(void);
void SPI_OLED_get_data_from_ROM(u8 addrHigh,u8 addrMid,u8 addrLow,u8 *pbuff,u8 DataLen);
void SPI_OLED_Display_GB2312_string(u8 x,u8 y,u8 *text);
void SPI_OLED_Display_string_5x7(u8 x,u8 y,u8 *text);
void SPI_OLED_ShowNum(u8 x,u8 y,float num,u8 len);
void SPI_OLED_Init(void);
#endif