#include "I2C_OLED.h"
#include "oledfont.h"
#include "GPIO.h"
#include "I2C.h"
#include "NVIC.h"
#include "Switch.h"

//#include "bmp.h" // bitmap
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
//void delay_ms(unsigned int ms)
//{                         
//	unsigned int a;
//	while(ms)
//	{
//		a=1800;
//		while(a--);
//		ms--;
//	}
//	return;
//}

//反显函数
void I2C_OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			I2C_OLED_WR_Byte(0xA6,I2C_OLED_CMD);//正常显示
		}
	if(i==1)
		{
			I2C_OLED_WR_Byte(0xA7,I2C_OLED_CMD);//反色显示
		}
}

//屏幕旋转180度
void I2C_OLED_DisplayTurn(u8 i)
{
	if(i==0)
		{
			I2C_OLED_WR_Byte(0xC8,I2C_OLED_CMD);//正常显示
			I2C_OLED_WR_Byte(0xA1,I2C_OLED_CMD);
		}
	if(i==1)
		{
			I2C_OLED_WR_Byte(0xC0,I2C_OLED_CMD);//反转显示
			I2C_OLED_WR_Byte(0xA0,I2C_OLED_CMD);
		}
}


//延时
void IIC_delay(void)
{
	u8 t=1;
	while(t--);
}

//起始信号
void I2C_Start(void)
{
	I2C_OLED_SDA_Set();
	I2C_OLED_SCL_Set();
	IIC_delay();
	I2C_OLED_SDA_Clr();
	IIC_delay();
	I2C_OLED_SCL_Clr();
	 
}

//结束信号
void I2C_Stop(void)
{
	I2C_OLED_SDA_Clr();
	I2C_OLED_SCL_Set();
	IIC_delay();
	I2C_OLED_SDA_Set();
}

//等待信号响应
void I2C_WaitAck(void) //测数据信号的电平
{
	I2C_OLED_SDA_Set();
	IIC_delay();
	I2C_OLED_SCL_Set();
	IIC_delay();
	I2C_OLED_SCL_Clr();
	IIC_delay();
}

//写入一个字节
void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		I2C_OLED_SCL_Clr();//将时钟信号设置为低电平
		if(dat&0x80)//将dat的8位从最高位依次写入
		{
			I2C_OLED_SDA_Set();
    }
		else
		{
			I2C_OLED_SDA_Clr();
    }
		IIC_delay();
		I2C_OLED_SCL_Set();
		IIC_delay();
		I2C_OLED_SCL_Clr();
		dat<<=1;
  }
}

#if 0
//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void I2C_OLED_WR_Byte(u8 dat,u8 mode)
{
	I2C_Start();
	Send_Byte(0x78);
	I2C_WaitAck();
	if(mode){Send_Byte(0x40);}
  else{Send_Byte(0x00);}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}
#endif

void I2C_OLED_WR_Byte(u8 dat,u8 mode) {
    if(mode) {
        I2C_WriteNbyte(0x78, 0x40, &dat, 1);
    } else {
        I2C_WriteNbyte(0x78, 0x00, &dat, 1);
    }
}



//坐标设置

void I2C_OLED_Set_Pos(u8 x, u8 y) 
{ 
	I2C_OLED_WR_Byte(0xb0+y,I2C_OLED_CMD);
	I2C_OLED_WR_Byte(((x&0xf0)>>4)|0x10,I2C_OLED_CMD);
	I2C_OLED_WR_Byte((x&0x0f),I2C_OLED_CMD);
}   	  
//开启OLED显示    
void I2C_OLED_Display_On(void)
{
	I2C_OLED_WR_Byte(0X8D,I2C_OLED_CMD);  //SET DCDC命令
	I2C_OLED_WR_Byte(0X14,I2C_OLED_CMD);  //DCDC ON
	I2C_OLED_WR_Byte(0XAF,I2C_OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void I2C_OLED_Display_Off(void)
{
	I2C_OLED_WR_Byte(0X8D,I2C_OLED_CMD);  //SET DCDC命令
	I2C_OLED_WR_Byte(0X10,I2C_OLED_CMD);  //DCDC OFF
	I2C_OLED_WR_Byte(0XAE,I2C_OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void I2C_OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		I2C_OLED_WR_Byte (0xb0+i,I2C_OLED_CMD);    //设置页地址（0~7）
		I2C_OLED_WR_Byte (0x00,I2C_OLED_CMD);      //设置显示位置—列低地址
		I2C_OLED_WR_Byte (0x10,I2C_OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)I2C_OLED_WR_Byte(0,I2C_OLED_DATA); 
	} //更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63				 
//sizey:选择字体 6x8  8x16
void I2C_OLED_ShowChar(u8 x,u8 y,u8 chr,u8 sizey)
{      	
	u8 c=0,sizex=sizey/2;
	u16 i=0,size1;
	if(sizey==8)size1=6;
	else size1=(sizey/8+((sizey%8)?1:0))*(sizey/2);
	c=chr-' ';//得到偏移后的值
	I2C_OLED_Set_Pos(x,y);
	for(i=0;i<size1;i++)
	{
		if(i%sizex==0&&sizey!=8) I2C_OLED_Set_Pos(x,y++);
		if(sizey==8) I2C_OLED_WR_Byte(asc2_0806[c][i],I2C_OLED_DATA);//6X8字号
		else if(sizey==16) I2C_OLED_WR_Byte(asc2_1608[c][i],I2C_OLED_DATA);//8x16字号
//		else if(sizey==xx) I2C_OLED_WR_Byte(asc2_xxxx[c][i],I2C_OLED_DATA);//用户添加字号
		else return;
	}
}
//m^n函数
u32 I2C_OLED_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示数字
//x,y :起点坐标
//num:要显示的数字
//len :数字的位数
//sizey:字体大小		  
void I2C_OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey)
{         	
	u8 t,temp,m=0;
	u8 enshow=0;
	if(sizey==8)m=2;
	for(t=0;t<len;t++)
	{
		temp=(num/I2C_OLED_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				I2C_OLED_ShowChar(x+(sizey/2+m)*t,y,' ',sizey);
				continue;
			}else enshow=1;
		}
	 	I2C_OLED_ShowChar(x+(sizey/2+m)*t,y,temp+'0',sizey);
	}
}
//显示一个字符号串
void I2C_OLED_ShowString(u8 x,u8 y,u8 *chr,u8 sizey)
{
	u8 j=0;
	while (chr[j]!='\0')
	{		
		I2C_OLED_ShowChar(x,y,chr[j++],sizey);
		if(sizey==8)x+=6;
		else x+=sizey/2;
	}
}
//显示汉字
void I2C_OLED_ShowChinese(u8 x,u8 y,u8 no,u8 sizey)
{
	u16 i,size1=(sizey/8+((sizey%8)?1:0))*sizey;
	for(i=0;i<size1;i++)
	{
		if(i%sizey==0) I2C_OLED_Set_Pos(x,y++);
		if(sizey==16) I2C_OLED_WR_Byte(Hzk[no][i],I2C_OLED_DATA);//16x16字号
//		else if(sizey==xx) I2C_OLED_WR_Byte(xxx[c][i],I2C_OLED_DATA);//用户添加字号
		else return;
	}				
}


//显示图片
//x,y显示坐标
//sizex,sizey,图片长宽
//BMP：要显示的图片
void I2C_OLED_DrawBMP(u8 x,u8 y,u8 sizex, u8 sizey,u8 BMP[])
{ 	
  u16 j=0;
	u8 i,m;
	sizey=sizey/8+((sizey%8)?1:0);
	for(i=0;i<sizey;i++)
	{
		I2C_OLED_Set_Pos(x,i+y);
    for(m=0;m<sizex;m++)
		{      
			I2C_OLED_WR_Byte(BMP[j++],I2C_OLED_DATA);	    	
		}
	}
} 

static void	I2C_config(void)
{
	I2C_InitTypeDef		I2C_InitStructure;

	I2C_InitStructure.I2C_Mode      = I2C_Mode_Master;	//主从选择   I2C_Mode_Master, I2C_Mode_Slave
	I2C_InitStructure.I2C_Enable    = ENABLE;			//I2C功能使能,   ENABLE, DISABLE
	I2C_InitStructure.I2C_MS_WDTA   = DISABLE;			//主机使能自动发送,  ENABLE, DISABLE
	I2C_InitStructure.I2C_Speed     = 13;				//总线速度=Fosc/2/(Speed*2+4),      0~63
                                                        // 400k, 24M => 13
	I2C_Init(&I2C_InitStructure);
	NVIC_I2C_Init(I2C_Mode_Master,DISABLE,Priority_0);	//主从模式, I2C_Mode_Master, I2C_Mode_Slave; 中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

	I2C_SW(I2C_P33_P32);					//I2C_P14_P15,I2C_P24_P25,I2C_P33_P32
}

//初始化				    
void I2C_OLED_Init(void)
{
    P3_MODE_IO_PU(GPIO_Pin_2 | GPIO_Pin_3);
    I2C_config();
    
//	I2C_OLED_RES_Clr();
//  delay_ms(200);
//	I2C_OLED_RES_Set();
	
	I2C_OLED_WR_Byte(0xAE,I2C_OLED_CMD);//--turn off oled panel
	I2C_OLED_WR_Byte(0x00,I2C_OLED_CMD);//---set low column address
	I2C_OLED_WR_Byte(0x10,I2C_OLED_CMD);//---set high column address
	I2C_OLED_WR_Byte(0x40,I2C_OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	I2C_OLED_WR_Byte(0x81,I2C_OLED_CMD);//--set contrast control register
	I2C_OLED_WR_Byte(0xCF,I2C_OLED_CMD); // Set SEG Output Current Brightness
	I2C_OLED_WR_Byte(0xA1,I2C_OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	I2C_OLED_WR_Byte(0xC8,I2C_OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	I2C_OLED_WR_Byte(0xA6,I2C_OLED_CMD);//--set normal display
	I2C_OLED_WR_Byte(0xA8,I2C_OLED_CMD);//--set multiplex ratio(1 to 64)
	I2C_OLED_WR_Byte(0x3f,I2C_OLED_CMD);//--1/64 duty
	I2C_OLED_WR_Byte(0xD3,I2C_OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	I2C_OLED_WR_Byte(0x00,I2C_OLED_CMD);//-not offset
	I2C_OLED_WR_Byte(0xd5,I2C_OLED_CMD);//--set display clock divide ratio/oscillator frequency
	I2C_OLED_WR_Byte(0x80,I2C_OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	I2C_OLED_WR_Byte(0xD9,I2C_OLED_CMD);//--set pre-charge period
	I2C_OLED_WR_Byte(0xF1,I2C_OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	I2C_OLED_WR_Byte(0xDA,I2C_OLED_CMD);//--set com pins hardware configuration
	I2C_OLED_WR_Byte(0x12,I2C_OLED_CMD);
	I2C_OLED_WR_Byte(0xDB,I2C_OLED_CMD);//--set vcomh
	I2C_OLED_WR_Byte(0x40,I2C_OLED_CMD);//Set VCOM Deselect Level
	I2C_OLED_WR_Byte(0x20,I2C_OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	I2C_OLED_WR_Byte(0x02,I2C_OLED_CMD);//
	I2C_OLED_WR_Byte(0x8D,I2C_OLED_CMD);//--set Charge Pump enable/disable
	I2C_OLED_WR_Byte(0x14,I2C_OLED_CMD);//--set(0x10) disable
	I2C_OLED_WR_Byte(0xA4,I2C_OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	I2C_OLED_WR_Byte(0xA6,I2C_OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	I2C_OLED_Clear();
	I2C_OLED_WR_Byte(0xAF,I2C_OLED_CMD); /*display ON*/ 
}




