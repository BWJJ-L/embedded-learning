/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#ifndef	__STC8G_PWM15BIT_H
#define	__STC8G_PWM15BIT_H

#include	"config.h"

//========================================================================
//                              寄存器位定义
//========================================================================

sfr	PWMSET   = 0xF1;	//增强型PWM全局配置寄存器
sfr	PWMCFG01 = 0xF6;	//增强型PWM配置寄存器
sfr	PWMCFG23 = 0xF7;	//增强型PWM配置寄存器
sfr	PWMCFG45 = 0xFE;	//增强型PWM配置寄存器

/////////////////////////////////////////////////
//FD00H-FDFFH
/////////////////////////////////////////////////

#define     PWM0C       (*(unsigned int volatile xdata *)0xff00)
#define     PWM0CH      (*(unsigned char volatile xdata *)0xff00)
#define     PWM0CL      (*(unsigned char volatile xdata *)0xff01)
#define     PWM0CKS     (*(unsigned char volatile xdata *)0xff02)
#define     PWM0TADC    (*(unsigned int volatile xdata *)0xff03)
#define     PWM0TADCH   (*(unsigned char volatile xdata *)0xff03)
#define     PWM0TADCL   (*(unsigned char volatile xdata *)0xff04)
#define     PWM0IF      (*(unsigned char volatile xdata *)0xff05)
#define     PWM0FDCR    (*(unsigned char volatile xdata *)0xff06)
#define     PWM00T1     (*(unsigned int volatile xdata *)0xff10)
#define     PWM00T1H    (*(unsigned char volatile xdata *)0xff10)
#define     PWM00T1L    (*(unsigned char volatile xdata *)0xff11)
#define     PWM00T2     (*(unsigned int volatile xdata *)0xff12)
#define     PWM00T2H    (*(unsigned char volatile xdata *)0xff12)
#define     PWM00T2L    (*(unsigned char volatile xdata *)0xff13)
#define     PWM00CR     (*(unsigned char volatile xdata *)0xff14)
#define     PWM00HLD    (*(unsigned char volatile xdata *)0xff15)
#define     PWM01T1     (*(unsigned int volatile xdata *)0xff18)
#define     PWM01T1H    (*(unsigned char volatile xdata *)0xff18)
#define     PWM01T1L    (*(unsigned char volatile xdata *)0xff19)
#define     PWM01T2    	(*(unsigned int volatile xdata *)0xff1a)
#define     PWM01T2H    (*(unsigned char volatile xdata *)0xff1a)
#define     PWM01T2L    (*(unsigned char volatile xdata *)0xff1b)
#define     PWM01CR     (*(unsigned char volatile xdata *)0xff1c)
#define     PWM01HLD    (*(unsigned char volatile xdata *)0xff1d)
#define     PWM02T1     (*(unsigned int volatile xdata *)0xff20)
#define     PWM02T1H    (*(unsigned char volatile xdata *)0xff20)
#define     PWM02T1L    (*(unsigned char volatile xdata *)0xff21)
#define     PWM02T2    	(*(unsigned int volatile xdata *)0xff22)
#define     PWM02T2H    (*(unsigned char volatile xdata *)0xff22)
#define     PWM02T2L    (*(unsigned char volatile xdata *)0xff23)
#define     PWM02CR     (*(unsigned char volatile xdata *)0xff24)
#define     PWM02HLD    (*(unsigned char volatile xdata *)0xff25)
#define     PWM03T1    	(*(unsigned int volatile xdata *)0xff28)
#define     PWM03T1H    (*(unsigned char volatile xdata *)0xff28)
#define     PWM03T1L    (*(unsigned char volatile xdata *)0xff29)
#define     PWM03T2    	(*(unsigned int volatile xdata *)0xff2a)
#define     PWM03T2H    (*(unsigned char volatile xdata *)0xff2a)
#define     PWM03T2L    (*(unsigned char volatile xdata *)0xff2b)
#define     PWM03CR     (*(unsigned char volatile xdata *)0xff2c)
#define     PWM03HLD    (*(unsigned char volatile xdata *)0xff2d)
#define     PWM04T1    	(*(unsigned int volatile xdata *)0xff30)
#define     PWM04T1H    (*(unsigned char volatile xdata *)0xff30)
#define     PWM04T1L    (*(unsigned char volatile xdata *)0xff31)
#define     PWM04T2    	(*(unsigned int volatile xdata *)0xff32)
#define     PWM04T2H    (*(unsigned char volatile xdata *)0xff32)
#define     PWM04T2L    (*(unsigned char volatile xdata *)0xff33)
#define     PWM04CR     (*(unsigned char volatile xdata *)0xff34)
#define     PWM04HLD    (*(unsigned char volatile xdata *)0xff35)
#define     PWM05T1     (*(unsigned int volatile xdata *)0xff38)
#define     PWM05T1H    (*(unsigned char volatile xdata *)0xff38)
#define     PWM05T1L    (*(unsigned char volatile xdata *)0xff39)
#define     PWM05T2     (*(unsigned int volatile xdata *)0xff3a)
#define     PWM05T2H    (*(unsigned char volatile xdata *)0xff3a)
#define     PWM05T2L    (*(unsigned char volatile xdata *)0xff3b)
#define     PWM05CR     (*(unsigned char volatile xdata *)0xff3c)
#define     PWM05HLD    (*(unsigned char volatile xdata *)0xff3d)
#define     PWM06T1     (*(unsigned int volatile xdata *)0xff40)
#define     PWM06T1H    (*(unsigned char volatile xdata *)0xff40)
#define     PWM06T1L    (*(unsigned char volatile xdata *)0xff41)
#define     PWM06T2     (*(unsigned int volatile xdata *)0xff42)
#define     PWM06T2H    (*(unsigned char volatile xdata *)0xff42)
#define     PWM06T2L    (*(unsigned char volatile xdata *)0xff43)
#define     PWM06CR     (*(unsigned char volatile xdata *)0xff44)
#define     PWM06HLD    (*(unsigned char volatile xdata *)0xff45)
#define     PWM07T1     (*(unsigned int volatile xdata *)0xff48)
#define     PWM07T1H    (*(unsigned char volatile xdata *)0xff48)
#define     PWM07T1L    (*(unsigned char volatile xdata *)0xff49)
#define     PWM07T2     (*(unsigned int volatile xdata *)0xff4a)
#define     PWM07T2H    (*(unsigned char volatile xdata *)0xff4a)
#define     PWM07T2L    (*(unsigned char volatile xdata *)0xff4b)
#define     PWM07CR     (*(unsigned char volatile xdata *)0xff4c)
#define     PWM07HLD    (*(unsigned char volatile xdata *)0xff4d)
#define     PWM1C       (*(unsigned int volatile xdata *)0xff50)
#define     PWM1CH      (*(unsigned char volatile xdata *)0xff50)
#define     PWM1CL      (*(unsigned char volatile xdata *)0xff51)
#define     PWM1CKS     (*(unsigned char volatile xdata *)0xff52)
#define     PWM1IF      (*(unsigned char volatile xdata *)0xff55)
#define     PWM1FDCR    (*(unsigned char volatile xdata *)0xff56)
#define     PWM10T1     (*(unsigned int volatile xdata *)0xff60)
#define     PWM10T1H    (*(unsigned char volatile xdata *)0xff60)
#define     PWM10T1L    (*(unsigned char volatile xdata *)0xff61)
#define     PWM10T2     (*(unsigned int volatile xdata *)0xff62)
#define     PWM10T2H    (*(unsigned char volatile xdata *)0xff62)
#define     PWM10T2L    (*(unsigned char volatile xdata *)0xff63)
#define     PWM10CR     (*(unsigned char volatile xdata *)0xff64)
#define     PWM10HLD    (*(unsigned char volatile xdata *)0xff65)
#define     PWM11T1     (*(unsigned int volatile xdata *)0xff68)
#define     PWM11T1H    (*(unsigned char volatile xdata *)0xff68)
#define     PWM11T1L    (*(unsigned char volatile xdata *)0xff69)
#define     PWM11T2     (*(unsigned int volatile xdata *)0xff6a)
#define     PWM11T2H    (*(unsigned char volatile xdata *)0xff6a)
#define     PWM11T2L    (*(unsigned char volatile xdata *)0xff6b)
#define     PWM11CR     (*(unsigned char volatile xdata *)0xff6c)
#define     PWM11HLD    (*(unsigned char volatile xdata *)0xff6d)
#define     PWM12T1     (*(unsigned int volatile xdata *)0xff70)
#define     PWM12T1H    (*(unsigned char volatile xdata *)0xff70)
#define     PWM12T1L    (*(unsigned char volatile xdata *)0xff71)
#define     PWM12T2     (*(unsigned int volatile xdata *)0xff72)
#define     PWM12T2H    (*(unsigned char volatile xdata *)0xff72)
#define     PWM12T2L    (*(unsigned char volatile xdata *)0xff73)
#define     PWM12CR     (*(unsigned char volatile xdata *)0xff74)
#define     PWM12HLD    (*(unsigned char volatile xdata *)0xff75)
#define     PWM13T1     (*(unsigned int volatile xdata *)0xff78)
#define     PWM13T1H    (*(unsigned char volatile xdata *)0xff78)
#define     PWM13T1L    (*(unsigned char volatile xdata *)0xff79)
#define     PWM13T2     (*(unsigned int volatile xdata *)0xff7a)
#define     PWM13T2H    (*(unsigned char volatile xdata *)0xff7a)
#define     PWM13T2L    (*(unsigned char volatile xdata *)0xff7b)
#define     PWM13CR     (*(unsigned char volatile xdata *)0xff7c)
#define     PWM13HLD    (*(unsigned char volatile xdata *)0xff7d)
#define     PWM14T1     (*(unsigned int volatile xdata *)0xff80)
#define     PWM14T1H    (*(unsigned char volatile xdata *)0xff80)
#define     PWM14T1L    (*(unsigned char volatile xdata *)0xff81)
#define     PWM14T2     (*(unsigned int volatile xdata *)0xff82)
#define     PWM14T2H    (*(unsigned char volatile xdata *)0xff82)
#define     PWM14T2L    (*(unsigned char volatile xdata *)0xff83)
#define     PWM14CR     (*(unsigned char volatile xdata *)0xff84)
#define     PWM14HLD    (*(unsigned char volatile xdata *)0xff85)
#define     PWM15T1     (*(unsigned int volatile xdata *)0xff88)
#define     PWM15T1H    (*(unsigned char volatile xdata *)0xff88)
#define     PWM15T1L    (*(unsigned char volatile xdata *)0xff89)
#define     PWM15T2     (*(unsigned int volatile xdata *)0xff8a)
#define     PWM15T2H    (*(unsigned char volatile xdata *)0xff8a)
#define     PWM15T2L    (*(unsigned char volatile xdata *)0xff8b)
#define     PWM15CR     (*(unsigned char volatile xdata *)0xff8c)
#define     PWM15HLD    (*(unsigned char volatile xdata *)0xff8d)
#define     PWM16T1     (*(unsigned int volatile xdata *)0xff90)
#define     PWM16T1H    (*(unsigned char volatile xdata *)0xff90)
#define     PWM16T1L    (*(unsigned char volatile xdata *)0xff91)
#define     PWM16T2     (*(unsigned int volatile xdata *)0xff92)
#define     PWM16T2H    (*(unsigned char volatile xdata *)0xff92)
#define     PWM16T2L    (*(unsigned char volatile xdata *)0xff93)
#define     PWM16CR     (*(unsigned char volatile xdata *)0xff94)
#define     PWM16HLD    (*(unsigned char volatile xdata *)0xff95)
#define     PWM17T1     (*(unsigned int volatile xdata *)0xff98)
#define     PWM17T1H    (*(unsigned char volatile xdata *)0xff98)
#define     PWM17T1L    (*(unsigned char volatile xdata *)0xff99)
#define     PWM17T2     (*(unsigned int volatile xdata *)0xff9a)
#define     PWM17T2H    (*(unsigned char volatile xdata *)0xff9a)
#define     PWM17T2L    (*(unsigned char volatile xdata *)0xff9b)
#define     PWM17CR     (*(unsigned char volatile xdata *)0xff9c)
#define     PWM17HLD    (*(unsigned char volatile xdata *)0xff9d)
#define     PWM2C       (*(unsigned int volatile xdata *)0xffa0)
#define     PWM2CH      (*(unsigned char volatile xdata *)0xffa0)
#define     PWM2CL      (*(unsigned char volatile xdata *)0xffa1)
#define     PWM2CKS     (*(unsigned char volatile xdata *)0xffa2)
#define     PWM2TADC    (*(unsigned int volatile xdata *)0xffa3)
#define     PWM2TADCH   (*(unsigned char volatile xdata *)0xffa3)
#define     PWM2TADCL   (*(unsigned char volatile xdata *)0xffa4)
#define     PWM2IF      (*(unsigned char volatile xdata *)0xffa5)
#define     PWM2FDCR    (*(unsigned char volatile xdata *)0xffa6)
#define     PWM20T1     (*(unsigned int volatile xdata *)0xffb0)
#define     PWM20T1H    (*(unsigned char volatile xdata *)0xffb0)
#define     PWM20T1L    (*(unsigned char volatile xdata *)0xffb1)
#define     PWM20T2     (*(unsigned int volatile xdata *)0xffb2)
#define     PWM20T2H    (*(unsigned char volatile xdata *)0xffb2)
#define     PWM20T2L    (*(unsigned char volatile xdata *)0xffb3)
#define     PWM20CR     (*(unsigned char volatile xdata *)0xffb4)
#define     PWM20HLD    (*(unsigned char volatile xdata *)0xffb5)
#define     PWM21T1     (*(unsigned int volatile xdata *)0xffb8)
#define     PWM21T1H    (*(unsigned char volatile xdata *)0xffb8)
#define     PWM21T1L    (*(unsigned char volatile xdata *)0xffb9)
#define     PWM21T2     (*(unsigned int volatile xdata *)0xffba)
#define     PWM21T2H    (*(unsigned char volatile xdata *)0xffba)
#define     PWM21T2L    (*(unsigned char volatile xdata *)0xffbb)
#define     PWM21CR     (*(unsigned char volatile xdata *)0xffbc)
#define     PWM21HLD    (*(unsigned char volatile xdata *)0xffbd)
#define     PWM22T1     (*(unsigned int volatile xdata *)0xffc0)
#define     PWM22T1H    (*(unsigned char volatile xdata *)0xffc0)
#define     PWM22T1L    (*(unsigned char volatile xdata *)0xffc1)
#define     PWM22T2     (*(unsigned int volatile xdata *)0xffc2)
#define     PWM22T2H    (*(unsigned char volatile xdata *)0xffc2)
#define     PWM22T2L    (*(unsigned char volatile xdata *)0xffc3)
#define     PWM22CR     (*(unsigned char volatile xdata *)0xffc4)
#define     PWM22HLD    (*(unsigned char volatile xdata *)0xffc5)
#define     PWM23T1     (*(unsigned int volatile xdata *)0xffc8)
#define     PWM23T1H    (*(unsigned char volatile xdata *)0xffc8)
#define     PWM23T1L    (*(unsigned char volatile xdata *)0xffc9)
#define     PWM23T2     (*(unsigned int volatile xdata *)0xffca)
#define     PWM23T2H    (*(unsigned char volatile xdata *)0xffca)
#define     PWM23T2L    (*(unsigned char volatile xdata *)0xffcb)
#define     PWM23CR     (*(unsigned char volatile xdata *)0xffcc)
#define     PWM23HLD    (*(unsigned char volatile xdata *)0xffcd)
#define     PWM24T1     (*(unsigned int volatile xdata *)0xffd0)
#define     PWM24T1H    (*(unsigned char volatile xdata *)0xffd0)
#define     PWM24T1L    (*(unsigned char volatile xdata *)0xffd1)
#define     PWM24T2     (*(unsigned int volatile xdata *)0xffd2)
#define     PWM24T2H    (*(unsigned char volatile xdata *)0xffd2)
#define     PWM24T2L    (*(unsigned char volatile xdata *)0xffd3)
#define     PWM24CR     (*(unsigned char volatile xdata *)0xffd4)
#define     PWM24HLD    (*(unsigned char volatile xdata *)0xffd5)
#define     PWM25T1     (*(unsigned int volatile xdata *)0xffd8)
#define     PWM25T1H    (*(unsigned char volatile xdata *)0xffd8)
#define     PWM25T1L    (*(unsigned char volatile xdata *)0xffd9)
#define     PWM25T2     (*(unsigned int volatile xdata *)0xffda)
#define     PWM25T2H    (*(unsigned char volatile xdata *)0xffda)
#define     PWM25T2L    (*(unsigned char volatile xdata *)0xffdb)
#define     PWM25CR     (*(unsigned char volatile xdata *)0xffdc)
#define     PWM25HLD    (*(unsigned char volatile xdata *)0xffdd)
#define     PWM26T1     (*(unsigned int volatile xdata *)0xffe0)
#define     PWM26T1H    (*(unsigned char volatile xdata *)0xffe0)
#define     PWM26T1L    (*(unsigned char volatile xdata *)0xffe1)
#define     PWM26T2     (*(unsigned int volatile xdata *)0xffe2)
#define     PWM26T2H    (*(unsigned char volatile xdata *)0xffe2)
#define     PWM26T2L    (*(unsigned char volatile xdata *)0xffe3)
#define     PWM26CR     (*(unsigned char volatile xdata *)0xffe4)
#define     PWM26HLD    (*(unsigned char volatile xdata *)0xffe5)
#define     PWM27T1     (*(unsigned int volatile xdata *)0xffe8)
#define     PWM27T1H    (*(unsigned char volatile xdata *)0xffe8)
#define     PWM27T1L    (*(unsigned char volatile xdata *)0xffe9)
#define     PWM27T2     (*(unsigned int volatile xdata *)0xffea)
#define     PWM27T2H    (*(unsigned char volatile xdata *)0xffea)
#define     PWM27T2L    (*(unsigned char volatile xdata *)0xffeb)
#define     PWM27CR     (*(unsigned char volatile xdata *)0xffec)
#define     PWM27HLD    (*(unsigned char volatile xdata *)0xffed)

/////////////////////////////////////////////////
//FC00H-FCFFH
/////////////////////////////////////////////////

#define     PWM3C       (*(unsigned int volatile xdata *)0xfc00)
#define     PWM3CH      (*(unsigned char volatile xdata *)0xfc00)
#define     PWM3CL      (*(unsigned char volatile xdata *)0xfc01)
#define     PWM3CKS     (*(unsigned char volatile xdata *)0xfc02)
#define     PWM3IF      (*(unsigned char volatile xdata *)0xfc05)
#define     PWM3FDCR    (*(unsigned char volatile xdata *)0xfc06)
#define     PWM30T1     (*(unsigned int volatile xdata *)0xfc10)
#define     PWM30T1H    (*(unsigned char volatile xdata *)0xfc10)
#define     PWM30T1L    (*(unsigned char volatile xdata *)0xfc11)
#define     PWM30T2     (*(unsigned int volatile xdata *)0xfc12)
#define     PWM30T2H    (*(unsigned char volatile xdata *)0xfc12)
#define     PWM30T2L    (*(unsigned char volatile xdata *)0xfc13)
#define     PWM30CR     (*(unsigned char volatile xdata *)0xfc14)
#define     PWM30HLD    (*(unsigned char volatile xdata *)0xfc15)
#define     PWM31T1     (*(unsigned int volatile xdata *)0xfc18)
#define     PWM31T1H    (*(unsigned char volatile xdata *)0xfc18)
#define     PWM31T1L    (*(unsigned char volatile xdata *)0xfc19)
#define     PWM31T2     (*(unsigned int volatile xdata *)0xfc1a)
#define     PWM31T2H    (*(unsigned char volatile xdata *)0xfc1a)
#define     PWM31T2L    (*(unsigned char volatile xdata *)0xfc1b)
#define     PWM31CR     (*(unsigned char volatile xdata *)0xfc1c)
#define     PWM31HLD    (*(unsigned char volatile xdata *)0xfc1d)
#define     PWM32T1     (*(unsigned int volatile xdata *)0xfc20)
#define     PWM32T1H    (*(unsigned char volatile xdata *)0xfc20)
#define     PWM32T1L    (*(unsigned char volatile xdata *)0xfc21)
#define     PWM32T2     (*(unsigned int volatile xdata *)0xfc22)
#define     PWM32T2H    (*(unsigned char volatile xdata *)0xfc22)
#define     PWM32T2L    (*(unsigned char volatile xdata *)0xfc23)
#define     PWM32CR     (*(unsigned char volatile xdata *)0xfc24)
#define     PWM32HLD    (*(unsigned char volatile xdata *)0xfc25)
#define     PWM33T1     (*(unsigned int volatile xdata *)0xfc28)
#define     PWM33T1H    (*(unsigned char volatile xdata *)0xfc28)
#define     PWM33T1L    (*(unsigned char volatile xdata *)0xfc29)
#define     PWM33T2     (*(unsigned int volatile xdata *)0xfc2a)
#define     PWM33T2H    (*(unsigned char volatile xdata *)0xfc2a)
#define     PWM33T2L    (*(unsigned char volatile xdata *)0xfc2b)
#define     PWM33CR     (*(unsigned char volatile xdata *)0xfc2c)
#define     PWM33HLD    (*(unsigned char volatile xdata *)0xfc2d)
#define     PWM34T1     (*(unsigned int volatile xdata *)0xfc30)
#define     PWM34T1H    (*(unsigned char volatile xdata *)0xfc30)
#define     PWM34T1L    (*(unsigned char volatile xdata *)0xfc31)
#define     PWM34T2     (*(unsigned int volatile xdata *)0xfc32)
#define     PWM34T2H    (*(unsigned char volatile xdata *)0xfc32)
#define     PWM34T2L    (*(unsigned char volatile xdata *)0xfc33)
#define     PWM34CR     (*(unsigned char volatile xdata *)0xfc34)
#define     PWM34HLD    (*(unsigned char volatile xdata *)0xfc35)
#define     PWM35T1     (*(unsigned int volatile xdata *)0xfc38)
#define     PWM35T1H    (*(unsigned char volatile xdata *)0xfc38)
#define     PWM35T1L    (*(unsigned char volatile xdata *)0xfc39)
#define     PWM35T2     (*(unsigned int volatile xdata *)0xfc3a)
#define     PWM35T2H    (*(unsigned char volatile xdata *)0xfc3a)
#define     PWM35T2L    (*(unsigned char volatile xdata *)0xfc3b)
#define     PWM35CR     (*(unsigned char volatile xdata *)0xfc3c)
#define     PWM35HLD    (*(unsigned char volatile xdata *)0xfc3d)
#define     PWM36T1     (*(unsigned int volatile xdata *)0xfc40)
#define     PWM36T1H    (*(unsigned char volatile xdata *)0xfc40)
#define     PWM36T1L    (*(unsigned char volatile xdata *)0xfc41)
#define     PWM36T2     (*(unsigned int volatile xdata *)0xfc42)
#define     PWM36T2H    (*(unsigned char volatile xdata *)0xfc42)
#define     PWM36T2L    (*(unsigned char volatile xdata *)0xfc43)
#define     PWM36CR     (*(unsigned char volatile xdata *)0xfc44)
#define     PWM36HLD    (*(unsigned char volatile xdata *)0xfc45)
#define     PWM37T1     (*(unsigned int volatile xdata *)0xfc48)
#define     PWM37T1H    (*(unsigned char volatile xdata *)0xfc48)
#define     PWM37T1L    (*(unsigned char volatile xdata *)0xfc49)
#define     PWM37T2     (*(unsigned int volatile xdata *)0xfc4a)
#define     PWM37T2H    (*(unsigned char volatile xdata *)0xfc4a)
#define     PWM37T2L    (*(unsigned char volatile xdata *)0xfc4b)
#define     PWM37CR     (*(unsigned char volatile xdata *)0xfc4c)
#define     PWM37HLD    (*(unsigned char volatile xdata *)0xfc4d)
#define     PWM4C       (*(unsigned int volatile xdata *)0xfc50)
#define     PWM4CH      (*(unsigned char volatile xdata *)0xfc50)
#define     PWM4CL      (*(unsigned char volatile xdata *)0xfc51)
#define     PWM4CKS     (*(unsigned char volatile xdata *)0xfc52)
#define     PWM4TADC    (*(unsigned int volatile xdata *)0xfc53)
#define     PWM4TADCH   (*(unsigned char volatile xdata *)0xfc53)
#define     PWM4TADCL   (*(unsigned char volatile xdata *)0xfc54)
#define     PWM4IF      (*(unsigned char volatile xdata *)0xfc55)
#define     PWM4FDCR    (*(unsigned char volatile xdata *)0xfc56)
#define     PWM40T1     (*(unsigned int volatile xdata *)0xfc60)
#define     PWM40T1H    (*(unsigned char volatile xdata *)0xfc60)
#define     PWM40T1L    (*(unsigned char volatile xdata *)0xfc61)
#define     PWM40T2     (*(unsigned int volatile xdata *)0xfc62)
#define     PWM40T2H    (*(unsigned char volatile xdata *)0xfc62)
#define     PWM40T2L    (*(unsigned char volatile xdata *)0xfc63)
#define     PWM40CR     (*(unsigned char volatile xdata *)0xfc64)
#define     PWM40HLD    (*(unsigned char volatile xdata *)0xfc65)
#define     PWM41T1     (*(unsigned int volatile xdata *)0xfc68)
#define     PWM41T1H    (*(unsigned char volatile xdata *)0xfc68)
#define     PWM41T1L    (*(unsigned char volatile xdata *)0xfc69)
#define     PWM41T2     (*(unsigned int volatile xdata *)0xfc6a)
#define     PWM41T2H    (*(unsigned char volatile xdata *)0xfc6a)
#define     PWM41T2L    (*(unsigned char volatile xdata *)0xfc6b)
#define     PWM41CR     (*(unsigned char volatile xdata *)0xfc6c)
#define     PWM41HLD    (*(unsigned char volatile xdata *)0xfc6d)
#define     PWM42T1     (*(unsigned int volatile xdata *)0xfc70)
#define     PWM42T1H    (*(unsigned char volatile xdata *)0xfc70)
#define     PWM42T1L    (*(unsigned char volatile xdata *)0xfc71)
#define     PWM42T2     (*(unsigned int volatile xdata *)0xfc72)
#define     PWM42T2H    (*(unsigned char volatile xdata *)0xfc72)
#define     PWM42T2L    (*(unsigned char volatile xdata *)0xfc73)
#define     PWM42CR     (*(unsigned char volatile xdata *)0xfc74)
#define     PWM42HLD    (*(unsigned char volatile xdata *)0xfc75)
#define     PWM43T1     (*(unsigned int volatile xdata *)0xfc78)
#define     PWM43T1H    (*(unsigned char volatile xdata *)0xfc78)
#define     PWM43T1L    (*(unsigned char volatile xdata *)0xfc79)
#define     PWM43T2     (*(unsigned int volatile xdata *)0xfc7a)
#define     PWM43T2H    (*(unsigned char volatile xdata *)0xfc7a)
#define     PWM43T2L    (*(unsigned char volatile xdata *)0xfc7b)
#define     PWM43CR     (*(unsigned char volatile xdata *)0xfc7c)
#define     PWM43HLD    (*(unsigned char volatile xdata *)0xfc7d)
#define     PWM44T1     (*(unsigned int volatile xdata *)0xfc80)
#define     PWM44T1H    (*(unsigned char volatile xdata *)0xfc80)
#define     PWM44T1L    (*(unsigned char volatile xdata *)0xfc81)
#define     PWM44T2     (*(unsigned int volatile xdata *)0xfc82)
#define     PWM44T2H    (*(unsigned char volatile xdata *)0xfc82)
#define     PWM44T2L    (*(unsigned char volatile xdata *)0xfc83)
#define     PWM44CR     (*(unsigned char volatile xdata *)0xfc84)
#define     PWM44HLD    (*(unsigned char volatile xdata *)0xfc85)
#define     PWM45T1     (*(unsigned int volatile xdata *)0xfc88)
#define     PWM45T1H    (*(unsigned char volatile xdata *)0xfc88)
#define     PWM45T1L    (*(unsigned char volatile xdata *)0xfc89)
#define     PWM45T2     (*(unsigned int volatile xdata *)0xfc8a)
#define     PWM45T2H    (*(unsigned char volatile xdata *)0xfc8a)
#define     PWM45T2L    (*(unsigned char volatile xdata *)0xfc8b)
#define     PWM45CR     (*(unsigned char volatile xdata *)0xfc8c)
#define     PWM45HLD    (*(unsigned char volatile xdata *)0xfc8d)
#define     PWM46T1     (*(unsigned int volatile xdata *)0xfc90)
#define     PWM46T1H    (*(unsigned char volatile xdata *)0xfc90)
#define     PWM46T1L    (*(unsigned char volatile xdata *)0xfc91)
#define     PWM46T2     (*(unsigned int volatile xdata *)0xfc92)
#define     PWM46T2H    (*(unsigned char volatile xdata *)0xfc92)
#define     PWM46T2L    (*(unsigned char volatile xdata *)0xfc93)
#define     PWM46CR     (*(unsigned char volatile xdata *)0xfc94)
#define     PWM46HLD    (*(unsigned char volatile xdata *)0xfc95)
#define     PWM47T1     (*(unsigned int volatile xdata *)0xfc98)
#define     PWM47T1H    (*(unsigned char volatile xdata *)0xfc98)
#define     PWM47T1L    (*(unsigned char volatile xdata *)0xfc99)
#define     PWM47T2     (*(unsigned int volatile xdata *)0xfc9a)
#define     PWM47T2H    (*(unsigned char volatile xdata *)0xfc9a)
#define     PWM47T2L    (*(unsigned char volatile xdata *)0xfc9b)
#define     PWM47CR     (*(unsigned char volatile xdata *)0xfc9c)
#define     PWM47HLD    (*(unsigned char volatile xdata *)0xfc9d)
#define     PWM5C       (*(unsigned int volatile xdata *)0xfca0)
#define     PWM5CH      (*(unsigned char volatile xdata *)0xfca0)
#define     PWM5CL      (*(unsigned char volatile xdata *)0xfca1)
#define     PWM5CKS     (*(unsigned char volatile xdata *)0xfca2)
#define     PWM5IF      (*(unsigned char volatile xdata *)0xfca5)
#define     PWM5FDCR    (*(unsigned char volatile xdata *)0xfca6)
#define     PWM50T1     (*(unsigned int volatile xdata *)0xfcb0)
#define     PWM50T1H    (*(unsigned char volatile xdata *)0xfcb0)
#define     PWM50T1L    (*(unsigned char volatile xdata *)0xfcb1)
#define     PWM50T2     (*(unsigned int volatile xdata *)0xfcb2)
#define     PWM50T2H    (*(unsigned char volatile xdata *)0xfcb2)
#define     PWM50T2L    (*(unsigned char volatile xdata *)0xfcb3)
#define     PWM50CR     (*(unsigned char volatile xdata *)0xfcb4)
#define     PWM50HLD    (*(unsigned char volatile xdata *)0xfcb5)
#define     PWM51T1     (*(unsigned int volatile xdata *)0xfcb8)
#define     PWM51T1H    (*(unsigned char volatile xdata *)0xfcb8)
#define     PWM51T1L    (*(unsigned char volatile xdata *)0xfcb9)
#define     PWM51T2     (*(unsigned int volatile xdata *)0xfcba)
#define     PWM51T2H    (*(unsigned char volatile xdata *)0xfcba)
#define     PWM51T2L    (*(unsigned char volatile xdata *)0xfcbb)
#define     PWM51CR     (*(unsigned char volatile xdata *)0xfcbc)
#define     PWM51HLD    (*(unsigned char volatile xdata *)0xfcbd)
#define     PWM52T1     (*(unsigned int volatile xdata *)0xfcc0)
#define     PWM52T1H    (*(unsigned char volatile xdata *)0xfcc0)
#define     PWM52T1L    (*(unsigned char volatile xdata *)0xfcc1)
#define     PWM52T2     (*(unsigned int volatile xdata *)0xfcc2)
#define     PWM52T2H    (*(unsigned char volatile xdata *)0xfcc2)
#define     PWM52T2L    (*(unsigned char volatile xdata *)0xfcc3)
#define     PWM52CR     (*(unsigned char volatile xdata *)0xfcc4)
#define     PWM52HLD    (*(unsigned char volatile xdata *)0xfcc5)
#define     PWM53T1     (*(unsigned int volatile xdata *)0xfcc8)
#define     PWM53T1H    (*(unsigned char volatile xdata *)0xfcc8)
#define     PWM53T1L    (*(unsigned char volatile xdata *)0xfcc9)
#define     PWM53T2     (*(unsigned int volatile xdata *)0xfcca)
#define     PWM53T2H    (*(unsigned char volatile xdata *)0xfcca)
#define     PWM53T2L    (*(unsigned char volatile xdata *)0xfccb)
#define     PWM53CR     (*(unsigned char volatile xdata *)0xfccc)
#define     PWM53HLD    (*(unsigned char volatile xdata *)0xfccd)
#define     PWM54T1     (*(unsigned int volatile xdata *)0xfcd0)
#define     PWM54T1H    (*(unsigned char volatile xdata *)0xfcd0)
#define     PWM54T1L    (*(unsigned char volatile xdata *)0xfcd1)
#define     PWM54T2     (*(unsigned int volatile xdata *)0xfcd2)
#define     PWM54T2H    (*(unsigned char volatile xdata *)0xfcd2)
#define     PWM54T2L    (*(unsigned char volatile xdata *)0xfcd3)
#define     PWM54CR     (*(unsigned char volatile xdata *)0xfcd4)
#define     PWM54HLD    (*(unsigned char volatile xdata *)0xfcd5)
#define     PWM55T1     (*(unsigned int volatile xdata *)0xfcd8)
#define     PWM55T1H    (*(unsigned char volatile xdata *)0xfcd8)
#define     PWM55T1L    (*(unsigned char volatile xdata *)0xfcd9)
#define     PWM55T2     (*(unsigned int volatile xdata *)0xfcda)
#define     PWM55T2H    (*(unsigned char volatile xdata *)0xfcda)
#define     PWM55T2L    (*(unsigned char volatile xdata *)0xfcdb)
#define     PWM55CR     (*(unsigned char volatile xdata *)0xfcdc)
#define     PWM55HLD    (*(unsigned char volatile xdata *)0xfcdd)
#define     PWM56T1     (*(unsigned int volatile xdata *)0xfce0)
#define     PWM56T1H    (*(unsigned char volatile xdata *)0xfce0)
#define     PWM56T1L    (*(unsigned char volatile xdata *)0xfce1)
#define     PWM56T2     (*(unsigned int volatile xdata *)0xfce2)
#define     PWM56T2H    (*(unsigned char volatile xdata *)0xfce2)
#define     PWM56T2L    (*(unsigned char volatile xdata *)0xfce3)
#define     PWM56CR     (*(unsigned char volatile xdata *)0xfce4)
#define     PWM56HLD    (*(unsigned char volatile xdata *)0xfce5)
#define     PWM57T1     (*(unsigned int volatile xdata *)0xfce8)
#define     PWM57T1H    (*(unsigned char volatile xdata *)0xfce8)
#define     PWM57T1L    (*(unsigned char volatile xdata *)0xfce9)
#define     PWM57T2     (*(unsigned int volatile xdata *)0xfcea)
#define     PWM57T2H    (*(unsigned char volatile xdata *)0xfcea)
#define     PWM57T2L    (*(unsigned char volatile xdata *)0xfceb)
#define     PWM57CR     (*(unsigned char volatile xdata *)0xfcec)
#define     PWM57HLD    (*(unsigned char volatile xdata *)0xfced)

//========================================================================
//                              PWM设置
//========================================================================

#define PWM15_PWM0_Set(n)					(n==1?(PWMSET |= 0x01):(PWMSET &= ~0x01))				//使能/关闭 PWM0（包括 PWM00~PWM07）
#define PWM15_PWM1_Set(n)					(n==1?(PWMSET |= 0x02):(PWMSET &= ~0x02))				//使能/关闭 PWM1（包括 PWM10~PWM17）
#define PWM15_PWM2_Set(n)					(n==1?(PWMSET |= 0x04):(PWMSET &= ~0x04))				//使能/关闭 PWM2（包括 PWM20~PWM27）
#define PWM15_PWM3_Set(n)					(n==1?(PWMSET |= 0x08):(PWMSET &= ~0x08))				//使能/关闭 PWM3（包括 PWM30~PWM37）
#define PWM15_PWM4_Set(n)					(n==1?(PWMSET |= 0x10):(PWMSET &= ~0x10))				//使能/关闭 PWM4（包括 PWM40~PWM47）
#define PWM15_PWM5_Set(n)					(n==1?(PWMSET |= 0x20):(PWMSET &= ~0x20))				//使能/关闭 PWM5（包括 PWM50~PWM54）

#define PWM0_Clock_Sel(n)					(n==1?(PWM0CKS |= 0x10):(PWM0CKS &= ~0x10))			//PWMn时钟源选择
#define PWM1_Clock_Sel(n)					(n==1?(PWM1CKS |= 0x10):(PWM1CKS &= ~0x10))			//PWMn时钟源选择
#define PWM2_Clock_Sel(n)					(n==1?(PWM2CKS |= 0x10):(PWM2CKS &= ~0x10))			//PWMn时钟源选择
#define PWM3_Clock_Sel(n)					(n==1?(PWM3CKS |= 0x10):(PWM3CKS &= ~0x10))			//PWMn时钟源选择
#define PWM4_Clock_Sel(n)					(n==1?(PWM4CKS |= 0x10):(PWM4CKS &= ~0x10))			//PWMn时钟源选择
#define PWM5_Clock_Sel(n)					(n==1?(PWM5CKS |= 0x10):(PWM5CKS &= ~0x10))			//PWMn时钟源选择

#define PWM15_Counter0_Set(n)			(n==1?(PWMCFG01 |= 0x01):(PWMCFG01 &= ~0x01))		//PWM0计数器使能设置
#define PWM15_Counter1_Set(n)			(n==1?(PWMCFG01 |= 0x10):(PWMCFG01 &= ~0x10))		//PWM1计数器使能设置
#define PWM15_Counter2_Set(n)			(n==1?(PWMCFG23 |= 0x01):(PWMCFG23 &= ~0x01))		//PWM2计数器使能设置
#define PWM15_Counter3_Set(n)			(n==1?(PWMCFG23 |= 0x10):(PWMCFG23 &= ~0x10))		//PWM3计数器使能设置
#define PWM15_Counter4_Set(n)			(n==1?(PWMCFG45 |= 0x01):(PWMCFG45 &= ~0x01))		//PWM4计数器使能设置
#define PWM15_Counter5_Set(n)			(n==1?(PWMCFG45 |= 0x10):(PWMCFG45 &= ~0x10))		//PWM5计数器使能设置

#define PWM15_Counter0Int_Set(n)	(n==1?(PWMCFG01 |= 0x04):(PWMCFG01 &= ~0x04))		//PWM0计数器归零中断使能设置
#define PWM15_Counter1Int_Set(n)	(n==1?(PWMCFG01 |= 0x40):(PWMCFG01 &= ~0x40))		//PWM1计数器归零中断使能设置
#define PWM15_Counter2Int_Set(n)	(n==1?(PWMCFG23 |= 0x04):(PWMCFG23 &= ~0x04))		//PWM2计数器归零中断使能设置
#define PWM15_Counter3Int_Set(n)	(n==1?(PWMCFG23 |= 0x40):(PWMCFG23 &= ~0x40))		//PWM3计数器归零中断使能设置
#define PWM15_Counter4Int_Set(n)	(n==1?(PWMCFG45 |= 0x04):(PWMCFG45 &= ~0x04))		//PWM4计数器归零中断使能设置
#define PWM15_Counter5Int_Set(n)	(n==1?(PWMCFG45 |= 0x40):(PWMCFG45 &= ~0x40))		//PWM5计数器归零中断使能设置

//						             7  6  5     4      3      2      1      0     Reset Value
//sfr PWM0CKS = 0xFF02H;   -  -  -   SELT2          PWM_PS[3:0]          xxx0,0000  /* 增强型PWM时钟选择存器 */ 
//sfr PWM1CKS = 0xFF52H;   -  -  -   SELT2          PWM_PS[3:0]          xxx0,0000  /* 增强型PWM时钟选择存器 */ 
//sfr PWM2CKS = 0xFFA2H;   -  -  -   SELT2          PWM_PS[3:0]          xxx0,0000  /* 增强型PWM时钟选择存器 */ 
//sfr PWM3CKS = 0xFC02H;   -  -  -   SELT2          PWM_PS[3:0]          xxx0,0000  /* 增强型PWM时钟选择存器 */ 
//sfr PWM4CKS = 0xFC52H;   -  -  -   SELT2          PWM_PS[3:0]          xxx0,0000  /* 增强型PWM时钟选择存器 */ 
//sfr PWM5CKS = 0xFCA2H;   -  -  -   SELT2          PWM_PS[3:0]          xxx0,0000  /* 增强型PWM时钟选择存器 */ 
#define PWM0_PS_Clock()				PWM0CKS &= ~0x10		//0：PWMn时钟源为系统时钟分频后的时钟
#define PWM0_T2_Clock()				PWM0CKS |= 0x10			//1：PWMn时钟源为定时器2的溢出脉冲
#define PWM0_PS_Set(n)				PWM0CKS = (PWM0CKS & ~0x0F) | (n & 0x0F)		//系统时钟预分频参数设置：SYSclk / (PWM_PS+1)
#define PWM1_PS_Clock()				PWM1CKS &= ~0x10		//0：PWMn时钟源为系统时钟分频后的时钟
#define PWM1_T2_Clock()				PWM1CKS |= 0x10			//1：PWMn时钟源为定时器2的溢出脉冲
#define PWM1_PS_Set(n)				PWM1CKS = (PWM1CKS & ~0x0F) | (n & 0x0F)		//系统时钟预分频参数设置：SYSclk / (PWM_PS+1)
#define PWM2_PS_Clock()				PWM2CKS &= ~0x10		//0：PWMn时钟源为系统时钟分频后的时钟
#define PWM2_T2_Clock()				PWM2CKS |= 0x10			//1：PWMn时钟源为定时器2的溢出脉冲
#define PWM2_PS_Set(n)				PWM2CKS = (PWM2CKS & ~0x0F) | (n & 0x0F)		//系统时钟预分频参数设置：SYSclk / (PWM_PS+1)
#define PWM3_PS_Clock()				PWM3CKS &= ~0x10		//0：PWMn时钟源为系统时钟分频后的时钟
#define PWM3_T2_Clock()				PWM3CKS |= 0x10			//1：PWMn时钟源为定时器2的溢出脉冲
#define PWM3_PS_Set(n)				PWM3CKS = (PWM3CKS & ~0x0F) | (n & 0x0F)		//系统时钟预分频参数设置：SYSclk / (PWM_PS+1)
#define PWM4_PS_Clock()				PWM4CKS &= ~0x10		//0：PWMn时钟源为系统时钟分频后的时钟
#define PWM4_T2_Clock()				PWM4CKS |= 0x10			//1：PWMn时钟源为定时器2的溢出脉冲
#define PWM4_PS_Set(n)				PWM4CKS = (PWM4CKS & ~0x0F) | (n & 0x0F)		//系统时钟预分频参数设置：SYSclk / (PWM_PS+1)
#define PWM5_PS_Clock()				PWM5CKS &= ~0x10		//0：PWMn时钟源为系统时钟分频后的时钟
#define PWM5_T2_Clock()				PWM5CKS |= 0x10			//1：PWMn时钟源为定时器2的溢出脉冲
#define PWM5_PS_Set(n)				PWM5CKS = (PWM5CKS & ~0x0F) | (n & 0x0F)		//系统时钟预分频参数设置：SYSclk / (PWM_PS+1)

/*  PWMnTADC  */
#define PWM15_PWM0TADC(n)			PWM0TADC = (n & 0x7fff)		//PWM触发ADC时间点
#define PWM15_PWM2TADC(n)			PWM2TADC = (n & 0x7fff)		//PWM触发ADC时间点
#define PWM15_PWM4TADC(n)			PWM4TADC = (n & 0x7fff)		//PWM触发ADC时间点

/*  PWMnTADC  */
#define PWM15_PWM0TADC(n)			PWM0TADC = (n & 0x7fff)		//PWM触发ADC时间点

//========================================================================
//                              定义声明
//========================================================================

#define		PWM0_VECTOR		22
#define		PWMFD_VECTOR	23
#define		PWM1_VECTOR		28
#define		PWM2_VECTOR		29
#define		PWM3_VECTOR		30
#define		PWM4_VECTOR		31
#define		PWM5_VECTOR		32

#define	PWM0				0
#define	PWM1				1
#define	PWM2				2
#define	PWM3				3
#define	PWM4				4
#define	PWM5				5

#define	PWM00				0
#define	PWM01				1
#define	PWM02				2
#define	PWM03				3
#define	PWM04				4
#define	PWM05				5
#define	PWM06				6
#define	PWM07				7
#define	PWM10				8
#define	PWM11				9
#define	PWM12				10
#define	PWM13				11
#define	PWM14				12
#define	PWM15				13
#define	PWM16				14
#define	PWM17				15
#define	PWM20				16
#define	PWM21				17
#define	PWM22				18
#define	PWM23				19
#define	PWM24				20
#define	PWM25				21
#define	PWM26				22
#define	PWM27				23
#define	PWM30				24
#define	PWM31				25
#define	PWM32				26
#define	PWM33				27
#define	PWM34				28
#define	PWM35				29
#define	PWM36				30
#define	PWM37				31
#define	PWM40				32
#define	PWM41				33
#define	PWM42				34
#define	PWM43				35
#define	PWM44				36
#define	PWM45				37
#define	PWM46				38
#define	PWM47				39
#define	PWM50				40
#define	PWM51				41
#define	PWM52				42
#define	PWM53				43
#define	PWM54				44
#define	PWM55				45
#define	PWM56				46
#define	PWM57				47

#define	PWMn_CLK_SYS			0
#define	PWMn_CLK_TM2			1

typedef struct
{
	u8	PWM_Enable;	//PWM使能,  ENABLE, DISABLE
	u8	PWM_Counter;		//计数器使能,  ENABLE, DISABLE
	u8	PWM_CInt;				//计数器归零中断使能,  ENABLE, DISABLE
	u8	PWM_Clock_Sel;	//时钟源选择,  PWMn_CLK_SYS, PWMn_CLK_TM2
	u8	PWM_Clock_PS;		//系统时钟分频参数,  0~15
	u16	PWM_Period;			//PWM周期,  0~0x7fff
} PWM15_InitTypeDef;

u8	PWM15_Init(u8 PWM_id, PWM15_InitTypeDef *PWMx);
u8	PWMChannelCtrl(u8 PWM_id, u8 pwm_eno, u8 pwm_ini, u8 pwm_eni, u8 pwm_ent2i, u8 pwm_ent1i);
u8	PWMLevelSet(u8 PWM_id, u8 pwm_hldl, u8 pwm_hldh);
u8	PWM15Duty(u8 PWM_id, u16 dutyL, u16 dutyH);

#endif

