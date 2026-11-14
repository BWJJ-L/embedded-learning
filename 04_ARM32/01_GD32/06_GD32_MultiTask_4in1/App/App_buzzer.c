#include "App.h"

// 蜂鸣器业务状态机
// 打开后循环播放 1~8 音阶，每个音持续 4 个节拍（4 * 20ms = 80ms）
#define BUZZER_STEP_TICKS   4

static uint8_t s_buzzer_step  = 0;  // 当前节拍计数
static uint8_t s_buzzer_scale = 1;  // 当前音阶 1~8
static uint8_t s_buzzer_prev  = 1;  // 上一轮 g_buzzer_on，用于检测变化

// 关掉蜂鸣器并复位状态机
static void Buzzer_reset(void) {
	Buzzer_stop();
	s_buzzer_step  = 0;
	s_buzzer_scale = 1;
}

// ============== 蜂鸣器业务：20ms 轮询一次
void App_buzzer_task(void) {
//	// 全局暂停，或用户关掉蜂鸣器，都停止发声
//	if (g_run == 0 || g_buzzer_on == 0) {
//		// 只在"从开到关"这一刻停一次，避免每 20ms 重复写寄存器
//		if (s_buzzer_prev == 1) {
//			Buzzer_reset();
//			s_buzzer_prev = 0;
//		}
//		return;
//	}

//	// 从关到开，从头开始播
//	if (s_buzzer_prev == 0) {
//		s_buzzer_prev  = 1;
//		s_buzzer_step  = 0;
//		s_buzzer_scale = 1;
//		Buzzer_beep(s_buzzer_scale);
//		return;
//	}

//	// 播放中：数节拍，到点换下一个音阶
//	s_buzzer_step++;
//	if (s_buzzer_step < BUZZER_STEP_TICKS) return;

//	s_buzzer_step = 0;
//	s_buzzer_scale++;
//	if (s_buzzer_scale > BUZZER_SCALE_NUM) s_buzzer_scale = 1;

//	Buzzer_beep(s_buzzer_scale);
}
