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

#include	"STC8G_PWM15bit.h"

u16 code PWMxCR[48] = {
	0xff14,		/* PWM00CR */
	0xff1c,		/* PWM01CR */
	0xff24,		/* PWM02CR */
	0xff2c,		/* PWM03CR */
	0xff34,		/* PWM04CR */
	0xff3c,		/* PWM05CR */
	0xff44,		/* PWM06CR */
	0xff4c,		/* PWM07CR */
	0xff64,		/* PWM10CR */
	0xff6c,		/* PWM11CR */
	0xff74,		/* PWM12CR */
	0xff7c,		/* PWM13CR */
	0xff84,		/* PWM14CR */
	0xff8c,		/* PWM15CR */
	0xff94,		/* PWM16CR */
	0xff9c,		/* PWM17CR */
	0xffb4,		/* PWM20CR */
	0xffbc,		/* PWM21CR */
	0xffc4,		/* PWM22CR */
	0xffcc,		/* PWM23CR */
	0xffd4,		/* PWM24CR */
	0xffdc,		/* PWM25CR */
	0xffe4,		/* PWM26CR */
	0xffec,		/* PWM27CR */
	0xfc14,		/* PWM30CR */
	0xfc1c,		/* PWM31CR */
	0xfc24,		/* PWM32CR */
	0xfc2c,		/* PWM33CR */
	0xfc34,		/* PWM34CR */
	0xfc3c,		/* PWM35CR */
	0xfc44,		/* PWM36CR */
	0xfc4c,		/* PWM37CR */
	0xfc64,		/* PWM40CR */
	0xfc6c,		/* PWM41CR */
	0xfc74,		/* PWM42CR */
	0xfc7c,		/* PWM43CR */
	0xfc84,		/* PWM44CR */
	0xfc8c,		/* PWM45CR */
	0xfc94,		/* PWM46CR */
	0xfc9c,		/* PWM47CR */
	0xfcb4,		/* PWM50CR */
	0xfcbc,		/* PWM51CR */
	0xfcc4,		/* PWM52CR */
	0xfccc,		/* PWM53CR */
	0xfcd4,		/* PWM54CR */
	0xfcdc,		/* PWM55CR */
	0xfce4,		/* PWM56CR */
	0xfcec,		/* PWM57CR */
};

u16 code PWMxT1[48] = {
	0xff10,		/* PWM00T1 */
	0xff18,		/* PWM01T1 */
	0xff20,		/* PWM02T1 */
	0xff28,		/* PWM03T1 */
	0xff30,		/* PWM04T1 */
	0xff38,		/* PWM05T1 */
	0xff40,		/* PWM06T1 */
	0xff48,		/* PWM07T1 */
	0xff60,		/* PWM10T1 */
	0xff68,		/* PWM11T1 */
	0xff70,		/* PWM12T1 */
	0xff78,		/* PWM13T1 */
	0xff80,		/* PWM14T1 */
	0xff88,		/* PWM15T1 */
	0xff90,		/* PWM16T1 */
	0xff98,		/* PWM17T1 */
	0xffb0,		/* PWM20T1 */
	0xffb8,		/* PWM21T1 */
	0xffc0,		/* PWM22T1 */
	0xffc8,		/* PWM23T1 */
	0xffd0,		/* PWM24T1 */
	0xffd8,		/* PWM25T1 */
	0xffe0,		/* PWM26T1 */
	0xffe8,		/* PWM27T1 */
	0xfc10,		/* PWM30T1 */
	0xfc18,		/* PWM31T1 */
	0xfc20,		/* PWM32T1 */
	0xfc28,		/* PWM33T1 */
	0xfc30,		/* PWM34T1 */
	0xfc38,		/* PWM35T1 */
	0xfc40,		/* PWM36T1 */
	0xfc48,		/* PWM37T1 */
	0xfc60,		/* PWM40T1 */
	0xfc68,		/* PWM41T1 */
	0xfc70,		/* PWM42T1 */
	0xfc78,		/* PWM43T1 */
	0xfc80,		/* PWM44T1 */
	0xfc88,		/* PWM45T1 */
	0xfc90,		/* PWM46T1 */
	0xfc98,		/* PWM47T1 */
	0xfcb0,		/* PWM50T1 */
	0xfcb8,		/* PWM51T1 */
	0xfcc0,		/* PWM52T1 */
	0xfcc8,		/* PWM53T1 */
	0xfcd0,		/* PWM54T1 */
	0xfcd8,		/* PWM55T1 */
	0xfce0,		/* PWM56T1 */
	0xfce8,		/* PWM57T1 */
};

u16 code PWMxT2[48] = {
	0xff12,		/* PWM00T2 */
	0xff1a,		/* PWM01T2 */
	0xff22,		/* PWM02T2 */
	0xff2a,		/* PWM03T2 */
	0xff32,		/* PWM04T2 */
	0xff3a,		/* PWM05T2 */
	0xff42,		/* PWM06T2 */
	0xff4a,		/* PWM07T2 */
	0xff62,		/* PWM10T2 */
	0xff6a,		/* PWM11T2 */
	0xff72,		/* PWM12T2 */
	0xff7a,		/* PWM13T2 */
	0xff82,		/* PWM14T2 */
	0xff8a,		/* PWM15T2 */
	0xff92,		/* PWM16T2 */
	0xff9a,		/* PWM17T2 */
	0xffb2,		/* PWM20T2 */
	0xffba,		/* PWM21T2 */
	0xffc2,		/* PWM22T2 */
	0xffca,		/* PWM23T2 */
	0xffd2,		/* PWM24T2 */
	0xffda,		/* PWM25T2 */
	0xffe2,		/* PWM26T2 */
	0xffea,		/* PWM27T2 */
	0xfc12,		/* PWM30T2 */
	0xfc1a,		/* PWM31T2 */
	0xfc22,		/* PWM32T2 */
	0xfc2a,		/* PWM33T2 */
	0xfc32,		/* PWM34T2 */
	0xfc3a,		/* PWM35T2 */
	0xfc42,		/* PWM36T2 */
	0xfc4a,		/* PWM37T2 */
	0xfc62,		/* PWM40T2 */
	0xfc6a,		/* PWM41T2 */
	0xfc72,		/* PWM42T2 */
	0xfc7a,		/* PWM43T2 */
	0xfc82,		/* PWM44T2 */
	0xfc8a,		/* PWM45T2 */
	0xfc92,		/* PWM46T2 */
	0xfc9a,		/* PWM47T2 */
	0xfcb2,		/* PWM50T2 */
	0xfcba,		/* PWM51T2 */
	0xfcc2,		/* PWM52T2 */
	0xfcca,		/* PWM53T2 */
	0xfcd2,		/* PWM54T2 */
	0xfcda,		/* PWM55T2 */
	0xfce2,		/* PWM56T2 */
	0xfcea,		/* PWM57T2 */
};

u16 code PWMxHLD[48] = {
	0xff15,		/* PWM00HLD */
	0xff1d,		/* PWM01HLD */
	0xff25,		/* PWM02HLD */
	0xff2d,		/* PWM03HLD */
	0xff35,		/* PWM04HLD */
	0xff3d,		/* PWM05HLD */
	0xff45,		/* PWM06HLD */
	0xff4d,		/* PWM07HLD */
	0xff65,		/* PWM10HLD */
	0xff6d,		/* PWM11HLD */
	0xff75,		/* PWM12HLD */
	0xff7d,		/* PWM13HLD */
	0xff85,		/* PWM14HLD */
	0xff8d,		/* PWM15HLD */
	0xff95,		/* PWM16HLD */
	0xff9d,		/* PWM17HLD */
	0xffb5,		/* PWM20HLD */
	0xffbd,		/* PWM21HLD */
	0xffc5,		/* PWM22HLD */
	0xffcd,		/* PWM23HLD */
	0xffd5,		/* PWM24HLD */
	0xffdd,		/* PWM25HLD */
	0xffe5,		/* PWM26HLD */
	0xffed,		/* PWM27HLD */
	0xfc15,		/* PWM30HLD */
	0xfc1d,		/* PWM31HLD */
	0xfc25,		/* PWM32HLD */
	0xfc2d,		/* PWM33HLD */
	0xfc35,		/* PWM34HLD */
	0xfc3d,		/* PWM35HLD */
	0xfc45,		/* PWM36HLD */
	0xfc4d,		/* PWM37HLD */
	0xfc65,		/* PWM40HLD */
	0xfc6d,		/* PWM41HLD */
	0xfc75,		/* PWM42HLD */
	0xfc7d,		/* PWM43HLD */
	0xfc85,		/* PWM44HLD */
	0xfc8d,		/* PWM45HLD */
	0xfc95,		/* PWM46HLD */
	0xfc9d,		/* PWM47HLD */
	0xfcb5,		/* PWM50HLD */
	0xfcbd,		/* PWM51HLD */
	0xfcc5,		/* PWM52HLD */
	0xfccd,		/* PWM53HLD */
	0xfcd5,		/* PWM54HLD */
	0xfcdd,		/* PWM55HLD */
	0xfce5,		/* PWM56HLD */
	0xfced,		/* PWM57HLD */
};

//========================================================================
// 函数: u8 PWMChannelCtrl(u8 PWM_id, u8 pwm_eno, u8 pwm_ini, u8 pwm_eni, u8 pwm_ent2i, u8 pwm_ent1i)
// 描述: PWM通道控制寄存器设置. 
// 参数: PWM_id: PWM通道序号. 取值 0~57
//			 pwm_eno:   pwm输出使能, 0设为GPIO, 1设为PWM输出.
//			 pwm_ini:   pwm输出端的初始电平, 0为低电平, 1为高电平.
//			 pwm_eni:   pwm通道中断使能控制, 0为关闭PWM中断, 1为使能PWM中断.
//			 pwm_ent2i: pwm通道第二个触发点中断使能控制, 0为关闭PWM第二个触发点中断, 1为使能PWM第二个触发点中断.
//			 pwm_ent1i: pwm通道第一个触发点中断使能控制, 0为关闭PWM第一个触发点中断, 1为使能PWM第一个触发点中断.
// 返回: 成功返回 SUCCESS, 错误返回 FAIL.
// 版本: V1.0, 2020-09-22
//========================================================================
u8	PWMChannelCtrl(u8 PWM_id, u8 pwm_eno, u8 pwm_ini, u8 pwm_eni, u8 pwm_ent2i, u8 pwm_ent1i)
{
	u8	xdata *pPWMxCR;
	
	if(PWM_id > PWM57)	return FAIL;	//id错误
	if(pwm_eno > 1)	return FAIL;	//输出使能错误
	if(pwm_ini > 1)	return FAIL;	//输出端的初始电平错误
	if(pwm_eni > 1)	return FAIL;	//中断使能控制错误
	if(pwm_ent2i > 1)	return FAIL;	//第二个触发点中断使能控制错误
	if(pwm_ent1i > 1)	return FAIL;	//第一个触发点中断使能控制错误
	
	pPWMxCR = (u8 *)PWMxCR[PWM_id];
	*pPWMxCR = (pwm_eno << 7) | (pwm_ini << 6) | (pwm_eni << 2)| (pwm_ent2i << 1)| pwm_ent1i;
	return SUCCESS;
}

//========================================================================
// 函数: u8 PWM15Duty(u8 PWM_id, u16 Period, u16 dutyL, u16 dutyH)
// 描述: PWM 占空比设置. 
// 参数: PWM_id:  PWM通道序号. 取值 0~57
//			 dutyL:   pwm输出低电平位置, 取值 0~0x7fff.
//			 dutyH:   pwm输出高电平位置, 取值 0~0x7fff.
// 返回: 成功返回 SUCCESS, 错误返回 FAIL.
// 版本: V1.0, 2020-09-22
//========================================================================
u8	PWM15Duty(u8 PWM_id, u16 dutyL, u16 dutyH)
{
	u16	xdata *pPWMxT1;
	u16	xdata *pPWMxT2;
	
	if(PWM_id > PWM57)	return FAIL;	//id错误
	if(dutyL > 0x7fff)	return FAIL;	//低电平时间设置错误
	if(dutyH > 0x7fff)	return FAIL;	//高电平时间设置错误
	
	pPWMxT1 = (u16 *)PWMxT1[PWM_id];
	*pPWMxT1 = dutyL & 0x7fff;

	pPWMxT2 = (u16 *)PWMxT2[PWM_id];
	*pPWMxT2 = dutyH & 0x7fff;
	return SUCCESS;
}

//========================================================================
// 函数: u8	PWMLevelSet(u8 PWM_id, u8 pwm_hldl, u8 pwm_hldh)
// 描述: PWM通道控制寄存器设置. 
// 参数: PWM_id: PWM通道序号. 取值 0~57
//			 pwm_hldl:   pwm强制输出低电平控制位, 0正常输出, 1强制输出低电平.
//			 pwm_hldh:   pwm强制输出高电平控制位, 0正常输出, 1强制输出高电平.
// 返回: 成功返回 SUCCESS, 错误返回 FAIL.
// 版本: V1.0, 2020-09-22
//========================================================================
u8	PWMLevelSet(u8 PWM_id, u8 pwm_hldl, u8 pwm_hldh)
{
	u8	xdata *pPWMxHLD;
	
	if(PWM_id > PWM57)	return FAIL;	//id错误
	if(pwm_hldh > 1)	return FAIL;	//输出使能错误
	if(pwm_hldl > 1)	return FAIL;	//输出端的初始电平错误
	
	pPWMxHLD = (u8 *)PWMxHLD[PWM_id];
	*pPWMxHLD = (pwm_hldh << 1) | pwm_hldl;
	return SUCCESS;
}

//========================================================================
// 函数: u8	PWM15_Init(u8 PWM_id, PWM15_InitTypeDef *PWMx)
// 描述: 15位增强型PWM初始化程序.
// 参数: PWM_id: PWM组号. 取值 PWM0,PWM1,PWM2,PWM3,PWM4,PWM5
//		 PWMx: 结构参数,请参考头文件里的定义.
// 返回: 成功返回 SUCCESS, 错误返回 FAIL.
// 版本: V1.0, 2020-09-22
//========================================================================
u8	PWM15_Init(u8 PWM_id, PWM15_InitTypeDef *PWMx)
{
	if(PWM_id > PWM5)	return FAIL;		//id错误

	if(PWM_id == PWM0)
	{
		PWM15_PWM0_Set(PWMx->PWM_Enable);	//使能/关闭 PWM0（包括 PWM00~PWM07）
		PWM0_PS_Set(PWMx->PWM_Clock_PS);	//系统时钟分频参数,  0~15
		PWM0_Clock_Sel(PWMx->PWM_Clock_Sel);	//PWMn时钟源选择
		PWM0C = PWMx->PWM_Period & 0x7fff;
		PWM15_Counter0Int_Set(PWMx->PWM_CInt);	//PWMn计数器归零中断使能设置
		PWM15_Counter0_Set(PWMx->PWM_Counter);	//PWMn计数器使能设置
	}
	
	if(PWM_id == PWM1)
	{
		PWM15_PWM1_Set(PWMx->PWM_Enable);	//使能/关闭 PWM1（包括 PWM10~PWM17）
		PWM1_PS_Set(PWMx->PWM_Clock_PS);	//系统时钟分频参数,  0~15
		PWM1_Clock_Sel(PWMx->PWM_Clock_Sel);	//PWMn时钟源选择
		PWM1C = PWMx->PWM_Period & 0x7fff;
		PWM15_Counter1Int_Set(PWMx->PWM_CInt);	//PWMn计数器归零中断使能设置
		PWM15_Counter1_Set(PWMx->PWM_Counter);	//PWMn计数器使能设置
	}
	
	if(PWM_id == PWM2)
	{
		PWM15_PWM2_Set(PWMx->PWM_Enable);	//使能/关闭 PWM2（包括 PWM20~PWM27）
		PWM2_PS_Set(PWMx->PWM_Clock_PS);	//系统时钟分频参数,  0~15
		PWM2_Clock_Sel(PWMx->PWM_Clock_Sel);	//PWMn时钟源选择
		PWM2C = PWMx->PWM_Period & 0x7fff;
		PWM15_Counter2Int_Set(PWMx->PWM_CInt);	//PWMn计数器归零中断使能设置
		PWM15_Counter2_Set(PWMx->PWM_Counter);	//PWMn计数器使能设置
	}
	
	if(PWM_id == PWM3)
	{
		PWM15_PWM3_Set(PWMx->PWM_Enable);	//使能/关闭 PWM3（包括 PWM30~PWM37）
		PWM3_PS_Set(PWMx->PWM_Clock_PS);	//系统时钟分频参数,  0~15
		PWM3_Clock_Sel(PWMx->PWM_Clock_Sel);	//PWMn时钟源选择
		PWM3C = PWMx->PWM_Period & 0x7fff;
		PWM15_Counter3Int_Set(PWMx->PWM_CInt);	//PWMn计数器归零中断使能设置
		PWM15_Counter3_Set(PWMx->PWM_Counter);	//PWMn计数器使能设置
	}
	
	if(PWM_id == PWM4)
	{
		PWM15_PWM4_Set(PWMx->PWM_Enable);	//使能/关闭 PWM4（包括 PWM40~PWM47）
		PWM4_PS_Set(PWMx->PWM_Clock_PS);	//系统时钟分频参数,  0~15
		PWM4_Clock_Sel(PWMx->PWM_Clock_Sel);	//PWMn时钟源选择
		PWM4C = PWMx->PWM_Period & 0x7fff;
		PWM15_Counter4Int_Set(PWMx->PWM_CInt);	//PWMn计数器归零中断使能设置
		PWM15_Counter4_Set(PWMx->PWM_Counter);	//PWMn计数器使能设置
	}
	
	if(PWM_id == PWM5)
	{
		PWM15_PWM5_Set(PWMx->PWM_Enable);	//使能/关闭 PWM5（包括 PWM50~PWM54）
		PWM5_PS_Set(PWMx->PWM_Clock_PS);	//系统时钟分频参数,  0~15
		PWM5_Clock_Sel(PWMx->PWM_Clock_Sel);	//PWMn时钟源选择
		PWM5C = PWMx->PWM_Period & 0x7fff;
		PWM15_Counter5Int_Set(PWMx->PWM_CInt);	//PWMn计数器归零中断使能设置
		PWM15_Counter5_Set(PWMx->PWM_Counter);	//PWMn计数器使能设置
	}
	return SUCCESS;
}
