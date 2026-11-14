#include "ADC0.h"
#include "systick.h"

static uint16_t g_recv_buff[ADC0_CHNS_LEN] = {0};
static uint8_t	adc_chns[ADC0_CHNS_LEN] = ADC0_CHNS;
static uint32_t adc_chns_samples[ADC0_CHNS_LEN] = ADC0_CHNS_SAMPLES;

#define ADC0_DMA_CH  			DMA1, DMA_CH0
#define ADC0_DMA_PERIPH_ADDR   (uint32_t)(&ADC_RDATA(ADC0))
#define ADC0_DMA_SUB_PERIPH    DMA_SUBPERI0
#define CALIB_TIME			   1 	// 校准延时

static void DMA_config() { // 外设到内存  读取外设数据，放在内存
	rcu_periph_clock_enable(RCU_DMA1); // 时钟使能
	dma_deinit(ADC0_DMA_CH);	// 重置
	// 结构体配置默认参数
	dma_single_data_parameter_struct init_struct;
	dma_single_data_para_struct_init(&init_struct);
	// 修改结构体成员
	// 1. 方向： 外设到内存
	init_struct.direction = DMA_PERIPH_TO_MEMORY;
	// 2. 源头：外设
	init_struct.periph_addr = ADC0_DMA_PERIPH_ADDR; // 外设地址
	init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE; // 是否自增长(外设不增长)
	// 3. 目标：内存
	init_struct.memory0_addr = (uint32_t)g_recv_buff; // 内存地址
	init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE; // 是否自增长(内存自增长)
	// 4. 搬运一个元素的大小
	init_struct.periph_memory_width = DMA_PERIPH_WIDTH_16BIT; // 16位
	// 5.  搬运最大元素个数
	init_struct.number = ADC0_CHNS_LEN;  // 指定搬运最大个数，实际有可能没有那么多
	// 6. 优先级
	init_struct.priority = DMA_PRIORITY_HIGH;
	// 7. 循环模式  开启
	init_struct.circular_mode = DMA_CIRCULAR_MODE_ENABLE;
	// DMA初始化
	dma_single_data_mode_init(ADC0_DMA_CH, &init_struct);
	// 子外设选择
	dma_channel_subperipheral_select(ADC0_DMA_CH, ADC0_DMA_SUB_PERIPH);
	
	// 清除标志位(可选)
	dma_flag_clear(ADC0_DMA_CH, DMA_FLAG_FTF);
	// 配置完了，就要DMA工作，搬运数据
	dma_channel_enable(ADC0_DMA_CH);
}

void ADC0_init() {
	 // 重置
    adc_deinit();
    // 配置主频  
    adc_clock_config(ADC0_CLK);
	
	/****************** GPIO config 模拟输入 *********************/
	for(uint8_t i = 0; i < ADC0_CHNS_LEN; i++) { 
		if(adc_chns[i] == ADC_CHANNEL_0) {
			GPIO_analog(CH0_PIN);
		} else if(adc_chns[i] == ADC_CHANNEL_1) {
			GPIO_analog(CH1_PIN);
		} else if(adc_chns[i] == ADC_CHANNEL_2) {
			GPIO_analog(CH2_PIN);
		} else if(adc_chns[i] == ADC_CHANNEL_3) {
			GPIO_analog(CH3_PIN);
		} else if(adc_chns[i] == ADC_CHANNEL_4) {
			GPIO_analog(CH4_PIN);
		} else if(adc_chns[i] == ADC_CHANNEL_5) {
			GPIO_analog(CH5_PIN);
		} else if(adc_chns[i] == ADC_CHANNEL_6) {
			GPIO_analog(CH6_PIN);
		} else if(adc_chns[i] == ADC_CHANNEL_7) {
			GPIO_analog(CH7_PIN);
		}else if(adc_chns[i] == ADC_CHANNEL_14) {
			GPIO_analog(CH14_PIN);
		}
		else if(adc_chns[i] == ADC_CHANNEL_16) {
			// 使能16到18内部通道(温度和参考电压通道) adc_channel_16_to_18
			adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH,ENABLE);
		}
	}
	
	/******************** ADC config **********************/
    // 配置时钟
    rcu_periph_clock_enable(RCU_ADC0);
   
    ////////// 采样配置
    adc_resolution_config(ADC0, ADC0_RESOLUTION); // 分辨率
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);// 数据右对齐

    //////////// 规则配置
    // 配置通道和通道数量  
	adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, ADC0_CHNS_LEN); // 常规
	for(uint8_t i = 0; i < ADC0_CHNS_LEN; i++) {
		adc_routine_channel_config(ADC0, i, adc_chns[i], adc_chns_samples[i]);
	}
   
    /////////// 模式配置
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);// 扫描模式
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);//非连续模式
	
	// DMA使能
    adc_dma_request_after_last_enable(ADC0);
    adc_dma_mode_enable(ADC0);

    // 打开ADC
    adc_enable(ADC0);
	
	// 校准ADC
    adc_calibration_enable(ADC0);
    delay_1ms(CALIB_TIME);
	
	// DMA配置
	DMA_config();
}

uint16_t ADC0_get(uint8_t index) {
	// 使能 ADC 的软件触发 adc_software_trigger_enable
	adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
	
	// 等待 DMA 传输完毕
	while(RESET == dma_flag_get(ADC0_DMA_CH, DMA_FLAG_FTF));
	// 清除 DMA 传输完毕标志位
	dma_flag_clear(ADC0_DMA_CH, DMA_FLAG_FTF);
	
	return g_recv_buff[index];
}