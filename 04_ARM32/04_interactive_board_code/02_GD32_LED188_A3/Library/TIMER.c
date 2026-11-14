#include "TIMER.h"
#include "gpio_cfg.h"

/**
 * @brief Timer定时器初始化配置
 *
 * @param rcu   TIMER的RCU
 * @param timer_periph  TIMERx(x=0,1,2,3,4,5,6,7,8,9,10,11,12,13)
 * @param t_perscaler   分频系数
 * @param t_period      周期
 */
static void timer_init_config(rcu_periph_enum rcu, uint32_t timer_periph,
                              uint16_t t_perscaler, uint32_t t_period) {
	// 时钟使能
	rcu_periph_clock_enable(rcu);

	// 重置
	timer_deinit(timer_periph);
	// 定义结构体变量
	timer_parameter_struct initpara;
	// 给结构体成员配置默认值
	timer_struct_para_init(&initpara);
	// 修改关键成员
	// SystemCoreClock/用户指定的频率      用户指定的频率 >= 2564
	initpara.period = t_period - 1; // 周期
	initpara.prescaler = t_perscaler - 1;  // 分频  
	
	// 初始化定时器
	timer_init(timer_periph, &initpara);

	// 启动定时器
	timer_enable(timer_periph);
}

/**
 * @brief 输出通道配置
 * 
 * @param timer_periph  TIMERx(x=0,1,2,3,4,5,6,7,8,9,10,11,12,13)
 * @param channel   	TIMER_CH_0, TIMER_CH_1, TIMER_CH_2, TIMER_CH_3
 * @param flag   		1 正极, 2 负极, 3 正负极
 */
static void timer_channel_config(uint32_t timer_periph, uint16_t channel, uint8_t flag) {
	// ====================== PWM相关
	timer_oc_parameter_struct ocpara;
	// 初始化结构体默认参数
	timer_channel_output_struct_para_init(&ocpara);
	// 修改关键成员
	if (flag == 1) {
		ocpara.outputstate  = TIMER_CCX_ENABLE; // 正极
	} else if (flag == 2) { // 负极
		ocpara.outputnstate = TIMER_CCXN_ENABLE; // 负极
	} else { // 正负极
		ocpara.outputstate  = TIMER_CCX_ENABLE; // 正极
		ocpara.outputnstate = TIMER_CCXN_ENABLE; // 负极
	}
	// 输出通道配置
	timer_channel_output_config(timer_periph, channel, &ocpara);
	// 配置模式  TIMER_OC_MODE_PWM0 和  TIMER_OC_MODE_PWM1 相反
	timer_channel_output_mode_config(timer_periph, channel, TIMER_OC_MODE_PWM0);
}

/**
 * @brief 更新指定Timer的channel的占空比
 *
 * @param timer_periph  TIMERx(x=0,1,2,3,4,5,6,7,8,9,10,11,12,13)
 * @param channel   TIMER_CH_0, TIMER_CH_1, TIMER_CH_2, TIMER_CH_3
 * @param t_period      周期 [1, 65536]
 * @param duty  		占空比 [0.0, 100.0]
 */
void TIMER_channel_duty_update(uint32_t timer_periph, uint16_t channel, uint32_t t_period, float duty) {
	// 占空比
	timer_channel_output_pulse_value_config(timer_periph, channel,  (duty / 100.0) * (t_period));
}

// 初始化
void TIMER_init() {
	// 4倍频，最大值为168M
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
	
#if USE_TIMER_0   // ====== 定时器0
	// 公共的
	// 定时器初始化配置
	timer_init_config(RCU_TIMER0, TIMER0, TM0_PRESCALER, TM0_PERIOD); 
	
	// ====== 通道0 ==========
	// 如果有定义TM0_CH0_ON 和 TM0_CH0_OP 两个宏，不关心定义
	#if defined (TM0_CH0_ON) &&  defined (TM0_CH0_OP)
	// 引脚IO
	GPIO_output_af(TM0_CH0_ON);
	GPIO_output_af(TM0_CH0_OP);
	

	// 通道配置
	timer_channel_config(TIMER0, TIMER_CH_0, 3); // 3为正负极
	#endif
	// ====== 通道1 ==========
	#if defined (TM0_CH1_ON) &&  defined (TM0_CH1_OP)
	// 引脚IO
	GPIO_output_af(TM0_CH1_ON);
	GPIO_output_af(TM0_CH1_OP);
	

	// 通道配置
	timer_channel_config(TIMER0, TIMER_CH_1, 3); // 3为正负极
	#endif
	
	
	// 保护电路也是公共的
	// break 只针对高级定时器TIMER0 & TIMER7，需要打开互补保护电路
	/* TIMER通道互补保护电路 */
	timer_break_parameter_struct breakpara;
	/* 初始化TIMER break参数结构体 */
	timer_break_struct_para_init(&breakpara);
	/* break输入的极性 HIGH */
	breakpara.breakpolarity   = TIMER_BREAK_POLARITY_HIGH;
	/* 输出自动的启用 */
	breakpara.outputautostate = TIMER_OUTAUTO_ENABLE;
	/* bread输入的启用*/
	breakpara.breakstate     = TIMER_BREAK_ENABLE;
	/* 死区时间 [0, 255] */
	// breakpara.deadtime  = 128;
	/* 配置TIMER break */
	timer_break_config(TIMER0, &breakpara);
	/* 启用TIMER break */
	timer_break_enable(TIMER0);
	
#endif

#if USE_TIMER_1   // ====== 定时器1
	// 公共的 定时器初始化配置
	timer_init_config(RCU_TIMER1, TIMER1, TM1_PRESCALER, TM1_PERIOD); 
	// ============ 通道
	#if defined (TM1_CH0)
	GPIO_output_af(TM1_CH0);
	timer_channel_config(TIMER1, TIMER_CH_0, 1); // 1为正极
	#endif
#endif
	
#if USE_TIMER_3   // ====== 定时器3
	// 公共的 定时器初始化配置
	timer_init_config(RCU_TIMER3, TIMER3, TM3_PRESCALER, TM3_PERIOD); 
	// ============ 通道
	#if defined (TM3_CH0)
	GPIO_output_af(TM3_CH0);
	timer_channel_config(TIMER3, TIMER_CH_0, 1); // 1为正极
	#endif
	
	#if defined (TM3_CH1)
	GPIO_output_af(TM3_CH1);
	timer_channel_config(TIMER3, TIMER_CH_1, 1); // 1为正极
	#endif
	
	#if defined (TM3_CH2)
	GPIO_output_af(TM3_CH2);
	timer_channel_config(TIMER3, TIMER_CH_2, 1); // 1为正极
	#endif
	
	#if defined (TM3_CH3)
	GPIO_output_af(TM3_CH3);
	timer_channel_config(TIMER3, TIMER_CH_3, 1); // 1为正极
	#endif
	
#endif

#if USE_TIMER_5 // 基本定时器，和引脚没有关系，时间到，触发中断
	// 定时器初始化配置
	timer_init_config(RCU_TIMER5, TIMER5, TM5_PRESCALER, TM5_PERIOD);
	// 中断优先级
	// ============= NVIC ================
	// 配置优先级
	nvic_irq_enable(TIMER5_DAC_IRQn, TM5_PRIORITY);
	// 标志位中断能使
	timer_interrupt_enable(TIMER5, TIMER_INT_UP);
	
#endif

#if USE_TIMER_6 // 基本定时器，和引脚没有关系，时间到，触发中断
	// 定时器初始化配置
	timer_init_config(RCU_TIMER6, TIMER6, TM6_PRESCALER, TM6_PERIOD);
	// 中断优先级
	// ============= NVIC ================
	// 配置优先级
	nvic_irq_enable(TIMER6_IRQn, TM6_PRIORITY);
	// 标志位中断能使
	timer_interrupt_enable(TIMER6, TIMER_INT_UP);
	
#endif

#if USE_TIMER_8   // ====== 定时器8
	// 公共的 定时器初始化配置
	timer_init_config(RCU_TIMER8, TIMER8, TM8_PRESCALER, TM8_PERIOD); 
	// ============ 通道
	#if defined (TM8_CH0)
	GPIO_output_af(TM8_CH0);
	timer_channel_config(TIMER8, TIMER_CH_0, 1); // 1为正极
	#endif
#endif

#if USE_TIMER_9   // ====== 定时器9
	// 公共的 定时器初始化配置
	timer_init_config(RCU_TIMER9, TIMER9, TM9_PRESCALER, TM9_PERIOD); 
	// ============ 通道
	#if defined (TM9_CH0)
	GPIO_output_af(TM9_CH0);
	timer_channel_config(TIMER9, TIMER_CH_0, 1); // 1为正极
	#endif
#endif
	
}

#if USE_TIMER_5
// 定时器5中断函数
void TIMER5_DAC_IRQHandler() { 
	if (SET == timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP)) {
		timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
		
		TIMER5_irq_callback(); // 中断函数回调函数
	}
}
#endif

#if USE_TIMER_6
// 定时器6中断函数
void TIMER6_IRQHandler() { 
	if (SET == timer_interrupt_flag_get(TIMER6, TIMER_INT_FLAG_UP)) {
		timer_interrupt_flag_clear(TIMER6, TIMER_INT_FLAG_UP);
		
		TIMER6_irq_callback(); // 中断函数回调函数
	}
}
#endif