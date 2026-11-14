#include "App.h"

extern volatile uint64_t g_t5_tick_ms;

// ============ 《两只老虎》简谱 ============
// 音阶编号 1~7 对应 do re mi fa sol la si（C 大调）
// 0 表示休止符（不发声，只停顿）
static const uint8_t s_notes[] = {
	1, 2, 3, 1,   1, 2, 3, 1,   3, 4, 5,   3, 4, 5,
	5, 6, 5, 4, 3, 1,   5, 6, 5, 4, 3, 1,
	1, 5, 1,   1, 5, 1
};

// 每个音的音长，单位是"半拍"。
// 4 表示四分音符（1 拍），8 表示八分音符（半拍），3/1 组合表示附点节奏
static const uint8_t s_durations[] = {
	4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 8,   4, 4, 8,
	3, 1, 3, 1, 4, 4,   3, 1, 3, 1, 4, 4,
	4, 4, 8,   4, 4, 8
};

#define MUSIC_NOTE_NUM   ( sizeof(s_notes) / sizeof(s_notes[0]) )

// 半拍的时长，单位 ms。60 BPM 的四分音符 = 1000ms，半拍 = 500ms 太慢，
// 这里取 250ms，即 120 BPM，是两只老虎比较常见的速度
#define MUSIC_MS_SPEED  50

// ============ 播放状态 ============
// 注意 s_music_idx 是"续播"能力的关键：KEY4 暂停时不清它，
// 再按 KEY4 就从这里继续，而不是从第一个音重来。
static uint8_t  s_music_idx = 0;         // 下一个要播的音在表里的下标
static uint8_t  s_music_started = 0;     // 0 = 从未开始过（需要从 idx 0 起）
static uint64_t s_music_note_start = 0;  // 当前这个音的起始时刻
static uint64_t s_music_next_ms = 0;     // 下一个音的切换时刻

// 取音阶对应的频率，超出 1~7 返回 0（休止）
static uint16_t Music_note_hz(uint8_t note) {
	if (note < 1 || note > 8) return 0;
	return Buzzer_get_hz(note);
}

// 播放第 idx 个音，并算出它应当在什么时候结束
static void Music_play_note(uint8_t idx, uint64_t note_start) {
	uint8_t  note = s_notes[idx];
	uint16_t dur  = s_durations[idx];

	uint16_t hz = Music_note_hz(note);
	if (hz == 0) {
		Buzzer_stop();          // 休止符
	} else {
		Buzzer_play(hz);
	}

	s_music_note_start = note_start;
	// 下一个音的切换时刻 = 本音起始时刻 + 本音时值
	s_music_next_ms = note_start + (uint64_t)dur * MUSIC_MS_SPEED;
}

// ============== 蜂鸣器业务：20ms 轮询一次
// 只负责《两只老虎》。用时间基准推进，不阻塞。
//
// 停止语义 = 暂停 + 续播：
//   暂停时 s_music_idx 保留在"当前正在播的那个音"，
//   续播时从同一个音重新开始播（不算重播整首）。
//   只有整首播完一轮，idx 才回到 0。
void App_buzzer_task(void) {
	uint64_t now = g_t5_tick_ms;

	if (g_music_running == 0) {
		// 已暂停。
		// 这里把 s_music_next_ms 清零，是"续播"的判据：
		//   next_ms != 0 -> 正在播
		//   next_ms == 0 -> 已暂停，下次进来用 now 重新定位这个音
		// 而 s_music_idx 绝对不清 —— 它就是"断点"。
		if (s_music_next_ms != 0) {
			s_music_next_ms = 0;
			Buzzer_stop();          // 保险：确保真的不响
		}
		return;
	}

	// ---- 首次启动（上电后第一次按 KEY4）----
	if (s_music_started == 0) {
		s_music_started = 1;
		Music_play_note(s_music_idx, now);   // s_music_idx 初始为 0
		return;
	}

	// ---- 从暂停状态续播 ----
	// 判据：当前没有在发声（Buzzer 已停），但 idx 还指着上次那个音。
	// 这时用 now 作为这个音的起始时刻重新播一遍。
	if (s_music_next_ms == 0) {
		Music_play_note(s_music_idx, now);
		return;
	}

	// ---- 正常播放中：到点换下一个音 ----
	if (now >= s_music_next_ms) {
		s_music_idx++;
		if (s_music_idx >= MUSIC_NOTE_NUM) {
			s_music_idx = 0;      // 整首播完，循环
		}
		// 新音的起始时刻取"上一个音算出来的切换时刻"，不是 now，
		// 这样节奏不会因 20ms 采样而一拍一拍往后漂
		Music_play_note(s_music_idx, s_music_next_ms);
	}
}
