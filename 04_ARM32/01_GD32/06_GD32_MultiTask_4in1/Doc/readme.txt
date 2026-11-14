GD32F407VE 裸机多任务四合一演示（独立按键 + 数码管 + LED灯组 + 蜂鸣器）
================================================================================

【一、这个工程做什么】

用一套裸机多任务框架（时间片轮询 + 函数指针任务表），同时驱动四个外设：
    1. 独立按键  KEY1~KEY4，PC0~PC3
    2. 数码管    8 位（本工程用前 4 位），74HC595 三线驱动
    3. LED 灯组  LED1~LED8，PD8~PD15，PC6 总开关
    4. 蜂鸣器    PB9，Timer1_CH1 PWM 驱动

四个任务各自有独立的执行周期，互不阻塞。数码管动态扫描是 2ms 一位、只扫 4 位，
一轮 8ms、刷新率 125Hz；
扫描期间按键和蜂鸣器照常响应 —— 这就是多任务框架的价值。


【二、接线（与天空星扩展板原理图一致）】

外设        信号        引脚      说明
----------  ----------  --------  ------------------------------------------
LED 灯组    LED_SW      PC6       总开关，经 R9(1K) 接 Q1(S8550_PNP) 基极
            LED1        PD8       低电平点亮
            LED2        PD9       低电平点亮
            LED3        PD10      低电平点亮
            LED4        PD11      低电平点亮
            LED5        PD12      低电平点亮
            LED6        PD13      低电平点亮
            LED7        PD14      低电平点亮
            LED8        PD15      低电平点亮

数码管      NIX_DI      PD0       595 的 SER（串行数据）
            NIX_RCK     PD1       595 的 RCLK（锁存）
            NIX_SCK     PD4       595 的 SRCLK（移位时钟）

独立按键    KEY1        PC0       上拉输入，按下为低
            KEY2        PC1       上拉输入，按下为低
            KEY3        PC2       上拉输入，按下为低
            KEY4        PC3       上拉输入，按下为低

蜂鸣器      BUZZER      PB9       复用 AF1，Timer1_CH1，无源蜂鸣器


【三、按键操作】

按键    功能            说明
------  --------------  ----------------------------------------------------
KEY1    切换 LED 模式   0 走马灯 -> 1 流水灯 -> 2 全灭 -> 循环
KEY2    数码管数值 +1   0~9999 循环，显示在数码管第 1~4 位
KEY3    蜂鸣器开关      打开后循环播放 1~8 音阶，每个音 80ms
KEY4    全局暂停/恢复   暂停时 LED 停、数码管灭、蜂鸣器停，按键本身仍工作


【四、上电后应该看到什么】

1. 上电：LED 全灭（PD8~PD15 初值为高电平），数码管全灭。
2. 按 KEY1：LED1 单独亮，每 200ms 往右移一位，到 LED8 后回到 LED1（走马灯）。
3. 再按 KEY1：LED 从 LED1 开始逐个累积点亮，8 个全亮后一起熄灭，重新累积（流水灯）。
4. 再按 KEY1：LED 全灭。
5. 按 KEY2：数码管第 1~4 位显示 0000、0001、0002 …… 每次 +1。
6. 按 KEY3：蜂鸣器开始循环播放音阶，再按一次停止。
7. 按 KEY4：全部停止；再按恢复。


【五、任务表（改这里就能加任务 / 改频率）】

位置：User/Task.c 的 task_list[]

    Task_t task_list[] = {
        {0,   2,    App_nixie_task},      // 数码管扫描，2ms
        {0,  10,    App_key_scan_task},   // 按键扫描，10ms
        {0,  20,    App_buzzer_task},     // 蜂鸣器节拍，20ms
        {0, 200,    App_led_task},        // LED 灯组，200ms
    };

加一个新任务：在数组里加一行 {0, 周期ms, 函数名}，
并把函数原型加到 App/App.h，函数实现写成"无参无返回"。
不要在任务函数里写 delay 或死循环，否则其它任务会被拖住
（数码管会闪、按键会失灵、蜂鸣器会卡音）。

时基来源：Timer5 每 1ms 中断一次，累加 g_t5_tick_ms。
调度判据：当前 tick - 上次 tick >= 周期，就执行一次并更新时间基准。


【六、目录说明】

User       系统文件：main.c、Task.c/h（任务调度）、systick、中断向量
App        业务层：App_key / App_led / App_nixie / App_buzzer
Hardware   驱动层：bsp_leds、bsp_keys、bsp_buzzer、Nixie、gpio_cfg.h
Library    定时器封装：TIMER.c/h、TIMER_config.h（Timer5 开关在这里）
Firmware   GD32F4xx 标准外设库 + CMSIS
Project    Keil 工程与编译产物
Doc        本说明


【七、和课程原工程的差异（移植时改了什么）】

1. 数码管驱动来自 exercise_day10/02_GD32_Nixie，改动：
   - Nixie_output 的循环变量从 char 改成 int8_t。
     原写法依赖"char 是有符号"这个编译器设置，换个编译器会死循环。
   - Nixie_display 增加了 num / idx 的越界保护。
   - 增加了 NIX_TABLE_MAX 宏，避免段码表读穿。

2. 蜂鸣器驱动来自 05_Timer/07_GD32_Timer1_Buzzer2，改动：
   - 去掉 Buzzer_demo()（里面用 delay_1ms 阻塞，会破坏多任务）。
   - Buzzer_play 不再每次 deinit/init 定时器，只改周期和占空比寄存器，
     避免换音时产生毛刺。
   - TIMER_CAR 必须按 32 位写。TIMER1 是 32 位定时器，强转 uint16_t
     会把超过 65535 的周期截断，音调直接跑偏。

3. 按键驱动增加了计数器式消抖（KEY_DEBOUNCE_CNT = 3，即 30ms）。
   原版没有消抖，机械按键抖动会让一次按下触发多次回调，
   在多任务里表现为 LED 模式乱跳、数码管数字乱涨。

4. gpio_cfg.h 补上了 GPIO_output_af()。本工程原先没有这个函数，
   而蜂鸣器需要把 PB9 配成复用输出。

5. Task_exec_handler 的循环变量从 uint8_t 改成 uint32_t。
   任务数超过 255 时 uint8_t 会回绕成死循环。


【八、编译与下载】

编译：Keil 打开 Project/GD32F407.uvprojx，F7 编译。
      命令行：UV4.exe -b Project\GD32F407.uvprojx -j0 -o build.log
已验证：0 Error(s), 0 Warning(s)，Code=4494 字节。
下载：JLink / DAPLink / STLink 均可，SWD 接口。


【九、如果现象不对，按症状查】

症状                        先查什么
--------------------------  ------------------------------------------------
LED 全不亮                   PC6 是否为低电平（PNP 基极低电平才导通）
                             用万用表量 PC6 对地电压，应该接近 0V
某个 LED 不亮                对应的 PDx 是否被别的功能占用（Timer3 CH 复用）
数码管全灭                    PD0/PD1/PD4 是否有波形；595 的 VCC(16脚) 是否上电
数码管有残影或很暗            Task 里数码管周期是否被改成 >5ms
数码管显示乱码                char 是否有符号的设置（本工程已改用 int8_t）
蜂鸣器不响                    PB9 是否配成 AF1；Timer1 是否被 TIMER_config.h 打开
                             （本工程 USE_TIMER_1 = 0，Timer1 归蜂鸣器独占）
蜂鸣器音调不对                rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4)
                             是否只调用了一次（在 TIMER_init 里），且早于 Buzzer_init
按键一次触发多次              消抖次数 KEY_DEBOUNCE_CNT 是否够（默认 3 = 30ms）
按键完全没反应                PC0~PC3 是否配成上拉输入；按下时对地是否导通
整体卡死                      任务函数里是否误加了 delay 或死循环
