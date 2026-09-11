#include "PCF8563.h"

static void GPIO_config(void) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.Pin  = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.Mode = GPIO_OUT_OD;
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);
}

void PCF8563_init(void) {
    EAXSFR();
    GPIO_config();
}

void PCF8563_set_Clock(Clock_t c) {
    u8 p[TIME_NUMBER] = {0};
    u8 C;
    
    p[0] = ((c.seconds / 10) << 4) | (c.seconds % 10);
    p[1] = ((c.minutes / 10) << 4) | (c.minutes % 10);
    p[2] = ((c.hours / 10) << 4)   | (c.hours % 10);
    p[3] = ((c.days / 10) << 4)    | (c.days % 10);
    p[4] = c.weekdays;
    
    C = (c.years >= 2100) ? 1 : 0;
    
    p[5] = (C << 7) |((c.months / 10) << 4)  | (c.months % 10);
    p[6] = ((c.years % 100 / 10) << 4) | (c.years % 10);
    
    SI2C_WriteNbyte(PCF8563_ADDR, PCF8563_REG, p, TIME_NUMBER);
}

void PCF8563_get_Clock(Clock_t * c) {
    u8 p[TIME_NUMBER] = {0};
    u8 C;
    
    SI2C_ReadNbyte(PCF8563_ADDR, PCF8563_REG, p, TIME_NUMBER);
    
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

void PCF8563_set_Alarm(Alarm_t a) {
    //默认所有时间单位都禁用
    u8 alarm[ALARM_NUMBER] = {0x80, 0x80, 0x80, 0x80};
    
    // 设置闹铃 --------------------------------------------------------------
    // 设置闹铃时间: 09h分钟, 0Ah小时, 0Bh天, 0Ch周 (最高0: 启动)
    // 分:  M 1 1 1 - 0 0 0 0 十进制数 -> BCD 最高位 启用 &(~0x10), 禁用 |0x80
    if(a.alarm_minute >= 0) {
        alarm[0] = (~0x10) & ((a.alarm_minute / 10) << 4) | (a.alarm_minute % 10);
    }
    
    if(a.alarm_hour >= 0) {
        alarm[1] = (~0x10) & ((a.alarm_hour / 10) << 4) | (a.alarm_hour % 10);
    }
    
    if(a.alarm_day >= 0) {
        alarm[2] = (~0x10) & ((a.alarm_day / 10) << 4) | (a.alarm_day % 10);
    }
    
    if(a.alarm_week >= 0) {
        alarm[3] = (~0x10) & a.alarm_week;
    }
    
    SI2C_WriteNbyte(PCF8563_ADDR, PCF8563_ALARM, alarm, ALARM_NUMBER);
}

void PCF8563_enable_Alarm(u8 enable) {
    u8 enAlarm;
    
    SI2C_ReadNbyte(PCF8563_ADDR, 0x01, &enAlarm, 1);
    
    // AF -> 清理Alarm中断标记 Alarm Flag Bit3清0, 确保闹铃中断触发
    enAlarm &= ~(1 << 3);
    // AIE-> 开启Alarm中断 Bit1置1, Alarm Interrupt Enable
    if(enable) {
        enAlarm |=  (1 << 1);
    } else {
        enAlarm &= ~(1 << 1);
    }
    
    SI2C_WriteNbyte(PCF8563_ADDR, 0x01, &enAlarm, 1);
}

void PCF8563_set_Timer(u8 frequency,u8 countdown) {
    u8 p;
    // 设置Timer定时器 -------------------------------------------------------
    // 3. 设置Timer运行频率 & 启用Timer
    p = (1 << 7) | frequency; //4.096kHz(0x00) 64Hz(0x01) 1Hz(0x02) 1/60Hz(0x03)
    SI2C_WriteNbyte(PCF8563_ADDR, TIMER_HZREG, &p, 1);
    
    // 4.设置Timer计数值n
    p = countdown;
    SI2C_WriteNbyte(PCF8563_ADDR, TIMER_REG, &p, 1);
}

void PCF8563_enable_Timer(u8 enable) {
    u8 enTimer;
    
    // 5.设置cs2,TIE置1启用,清理TF标记
    // 配置控制寄存器2 (CS2)  AF=0, AIE=1 启用闹钟-----------------
    SI2C_ReadNbyte(PCF8563_ADDR, 0x01, &enTimer, 1);
    
    // TF -> 清理Timer中断标记 Timer Flag Bit2清0, 确保定时器中断触发
    enTimer &= ~(1 << 2);
    
    // TIM -> 开启TIMER中断标记 Bit1 置1，Timer Interrupt Enable
    if(enable) {
        enTimer |=  (1 << 0);
    } else {
        enTimer &= ~(1 << 0);
    }
    
    SI2C_WriteNbyte(PCF8563_ADDR, 0x01, &enTimer, 1);
}

void exti_int3_call(void) {
    u8 reg_Status;
    // 读取cs2控制寄存器的值, 查看AF和TF标记
    SI2C_ReadNbyte(PCF8563_ADDR, 0x01, &reg_Status, 1);
    
    if((reg_Status >> 3) & 0x01){
        PCF8563_on_Alarm();
        // AF -> 清理Alarm中断标记 Alarm Flag Bit3清0, 确保闹铃中断触发
        reg_Status &= ~(1 << 3);
    }
    if(reg_Status & (1 << 2)) {
        PCF8563_on_Timer();
        // TF -> 清理Timer中断标记 Timer Flag Bit2清0, 确保定时器中断触发
        reg_Status &= ~(1 << 2);
    }
    
    // 一次性把修改后的信息写回0x01寄存器
    SI2C_WriteNbyte(PCF8563_ADDR, 0x01, &reg_Status, 1);
}