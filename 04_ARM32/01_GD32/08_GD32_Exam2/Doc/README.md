# 08_GD32_Exam2 —— 智能空气炸锅（旋转编码器 + 188 数码管 + WS2812）

> 对应考试：`6_ARM32\exercise\exam2`
> 参考代码：`6_ARM32\arm32_code\06_interactive_board_code`（24 号工程为骨架来源）
> 交互板文档：<https://www.yuque.com/mikejiangsky/arm32_plus/692467b317fe8c18f6c0e4ca06a29c81>

---

## 一、先说结论：按什么口径做的

考试的 `考试2.md` 里有两份互相冲突的要求：前面的 **PRD** 和后面的 **「考试内容」+ 评分表**。三处直接冲突：

| 项 | PRD（md 前半） | 评分表 / 考试内容（md 后半） | **本工程采用** |
|---|---|---|---|
| 温度上限 | 200 °C | **190 °C** | **190 °C** |
| 时间单位 | 分钟 | **秒** | **秒** |
| 默认时间 | 10 分钟 | **10 秒** | **10 秒** |

**一律以评分表为准**（评分表比 md 后半还严，多了闪电/百分号标志位要求）。

要改回 PRD 口径，只改 `App/App.h` 顶部这几个宏，业务代码一行不用动：

```c
#define FRYER_TEMP_MIN        80
#define FRYER_TEMP_MAX        190     // PRD 是 200
#define FRYER_TEMP_STEP       5
#define FRYER_TEMP_DEFAULT    100

#define FRYER_TIME_MIN        1
#define FRYER_TIME_MAX        60
#define FRYER_TIME_STEP       1
#define FRYER_TIME_DEFAULT    10      // PRD 是 10 分钟
```

另有两个已拍板的决定：

- **KEY3 长按阈值 = 2000ms**（按 md 原文「长按2秒」，不是参考库默认的 1000ms）
- **编码器自带按压键（PD15）不参与模式切换**，模式切换只认 KEY1

---

## 二、评分表 20 个得分点 → 代码落点

评分表共 20 条 × 5 分 = 100 分（表头写总分 90，但逐条加起来是 100，以逐条为准）。

| # | 得分点 | 代码落点 |
|---|---|---|
| A3 | key3 长按开机，数码管点亮，ws2812 点亮（默认绿色） | `App_fryer_power_toggle()` 开机分支 → `Fryer_set_all_led(COLOR_IDLE)` |
| A4 | 长按开机显示默认值(温度100，时间10)，默认温度模式，闪电亮 | 同上 + `Fryer_reset_values()`；`App_fryer_task()` 设 `LED188_charge(1)` |
| A5 | 长按关机，数码管熄灭，ws2812 熄灭 | `App_fryer_power_toggle()` 关机分支 → `LED188_clear()` + `COLOR_OFF` |
| A6 | key1 点击切换时间/温度显示，闪电=温度模式，百分号=时间模式 | `App_fryer_mode_toggle()`；`App_fryer_task()` 里 `LED188_charge` / `LED188_percent` |
| A7 | key2 切换运行/暂停，没有运行时可以加减，开机默认未运行 | `App_fryer_run_toggle()`；`g_running` 初值 0 |
| A8 | 运行时不可以加减 | `App_fryer_value_add()` 开头 `if (g_running) return;` |
| A9 | 时间模式加减步长为 1 | `FRYER_TIME_STEP` |
| A10 | 时间模式加减最小 1，最大 60 | `FRYER_TIME_MIN` / `FRYER_TIME_MAX`（钳制） |
| A11 | 温度模式加减步长为 5 | `FRYER_TEMP_STEP` |
| A12 | 温度模式加减最小 80，最大 190 | `FRYER_TEMP_MIN` / `FRYER_TEMP_MAX`（钳制） |
| A13 | 暂停时点 key2，显示时间 | `App_fryer_run_toggle()` 启动分支强制 `g_mode = FRYER_MODE_TIME` |
| A14 | 暂停时点 key2，加热开启（ws2812 红灯） | 启动分支 `Fryer_set_all_led(COLOR_HEAT)` |
| A15 | 运行时时间模式可倒数计时 | `App_fryer_task()` 用 `(now - base) / 1000` 算剩余 |
| A16 | 运行时点 key2，倒计时暂停 | 暂停分支只清 `g_running`，`g_time_left` 保留 |
| A17 | 运行时点 key2，加热暂停（ws2812 绿灯） | 暂停分支 `Fryer_set_all_led(COLOR_IDLE)` |
| A18 | 倒计时归 0 后加热暂停（绿灯） | `App_fryer_task()` 归零分支 |
| A19 | 倒计时归 0 后时间显示默认值 | 归零分支 `Fryer_reset_values()` + 保持时间模式 → 显示 10 |
| A20 | 倒计时归 0 后温度显示默认值 | 同上，再按 KEY1 切温度模式即见 100 |

---

## 三、硬件真值表（逐项抄自参考驱动源码，不是凭记忆）

| 项 | 值 | 出处 |
|---|---|---|
| 188 数码管 5 根线 | PE7 / PE9 / PE11 / PE13 / PE15 | `bsp_led188.h:8-12` |
| 188 段码位分配 | 个位 A3~G3=bit0~6；十位 A2~G2=bit7~13；百位 B1=bit14、C1=bit15；闪电 L1=bit16；百分号 L2=bit17 | `bsp_led188.c:5-26` |
| 188 有效电平 | 位线**高**=选中该位，段线**低**=点亮 | `bsp_led188.c:57-84` |
| 188 扫描 | 5 相轮流，1 相拉高其余高阻，2ms/相 → 10ms/轮 = 100Hz | `bsp_led188.c:52-88` |
| 188 空闲态 | 全部引脚高阻（`LED188_ALL_IN`） | `bsp_led188.h:47` |
| 188 数值上限 | `set_num > 199` 直接 return，百位只能显示 "1" | `bsp_led188.c:114,132-135` |
| 闪电标志 | `LED188_charge(1)` = 温度模式 | `bsp_led188.h:57` |
| 百分号标志 | `LED188_percent(1)` = 时间模式 | `bsp_led188.h:58` |
| EC12 A 相 | PD11，EXTI11，双边沿，上拉 | `bsp_ec12.h:17-20` / `EXTI_config.h:111-117` |
| EC12 B 相 | PD13，普通输入，浮空 | `bsp_ec12.h:22-24` |
| EC12 D（按压） | PD15，EXTI15，双边沿，上拉 | `bsp_ec12.h:26-29` / `EXTI_config.h:143-149` |
| EC12 转向判定 | A 下降沿时 B 高=右转；A 上升沿时 B 低=右转 | `bsp_ec12.c:39-44` |
| ADKey 引脚 | PA2 = ADC0 通道 2 | `bsp_adkey.h:9` / `ADC0.h:8,41` |
| ADKey 阈值 | KEY1=950，KEY2=2048，KEY3=3140，±20；全松=4095 | `bsp_adkey.c:32-41` |
| ADKey 短按 | 按住 ∈[50ms, 长按阈值)，**释放时**触发 | `bsp_adkey.c:10,141-149` |
| ADKey 长按 | 按住达阈值**立即**触发，之后释放不再触发短按 | `bsp_adkey.c:124-127` |
| ADKey 采样 | 每 1ms 采 1 次，50 次去最大最小求均值 → 50ms 出帧 | `bsp_adkey.c:52-54,173-186` |
| WS2812 数据线 | SPI1 MOSI = PB15（AF5） | `SPI1_hard.h:8-16` |
| WS2812 数量 | 4 颗 | `bsp_ws2812.h:6` |
| WS2812 颜色格式 | `0x00RRGGBB`，发送顺序 G→R→B | `bsp_ws2812.c:74-100` |
| WS2812 位编码 | 1 数据位 = 10 个 SPI 位；`BIT_0=0b1110000000`，`BIT_1=0b1111111000` | `bsp_ws2812.c:33-34` |
| WS2812 SPI | PSC_4（84M/4=21M），CPOL=1/CPHA=2EDGE，MSB | `SPI1_hard.h:25-37` |
| WS2812 刷新 | 40ms 一次，DMA 发 120 字节 + 365 字节 reset | `bsp_ws2812.c:103-109` |
| **WS2812 亮度** | `WS2812_set_color_brightness(index, color, brightness)`，brightness **0~100，100=满亮度**；按 `(v*b+50)/100` 四舍五入 | `bsp_ws2812.c:115-137` |
| 时基 | TIMER5 1ms 中断 → `g_t5_tick_ms` | `Task.c:39-45` |

---

## 四、任务表与周期

`User/Task.c` 的 `task_list[]`：

| 周期 | 任务 | 为什么是这个值 |
|---|---|---|
| 2ms | `App_led188_scan_task` | 2ms × 5 位 = 10ms/轮 = 100Hz，人眼无闪烁 |
| 1ms | `App_adkey_scan_task` | 驱动内部「每 1ms 采 1 次、累加 50 次出一帧」，周期拉长会让按键响应变迟钝 |
| 10ms | `App_key_scan_task` | 消费编码器脉冲 |
| 10ms | `App_fryer_task` | 状态机 + 倒计时 + 数码管内容 |
| 40ms | `App_ws2812_task` | 与参考驱动注释一致 |

除 1ms 外都是 2ms 的整数倍，不会和扫描任务抢时间。

---

## 五、三个设计上的关键决定

### 5.1 中断里只计数，绝不碰显示

`EC12_on_turn_right/left` 是在 **EXTI10_15 中断**里被调用的。如果在那里直接调 `LED188_set_num()`，就会和 2ms 的扫描任务抢同一个全局 `state`。

所以：

```c
// App_key.c：中断上下文只做这一件事
void EC12_on_turn_right(void) { g_ec12_pulse++; }
void EC12_on_turn_left(void)  { g_ec12_pulse--; }

// 任务上下文（10ms）才取走并换算
uint32_t pm = __get_PRIMASK();
__disable_irq();
pulse = g_ec12_pulse; g_ec12_pulse = 0;
__set_PRIMASK(pm);
```

### 5.2 倒计时用「绝对时间差」，不用 `--`

```c
static uint64_t s_run_base_ms;      // 本段运行起点
static uint16_t s_left_at_start;    // 起点时的剩余秒数
...
uint32_t elapsed = (uint32_t)((now - s_run_base_ms) / 1000u);
g_time_left = s_left_at_start - elapsed;
```

任务周期有抖动，`g_time_left--` 会累积误差；绝对时间差不累积。暂停时把剩余值存回 `g_time_left`，恢复时重新记 base。

### 5.3 编码器判向：两套方案，宏切换（用户实测过其中一套）

`Hardware/bsp_ec12.h` 里的开关：

```c
#define EC12_USE_QUADRATURE   0     // 0 = 老方案；1 = 正交状态机
```

| | 方案 0（默认，老方案） | 方案 1（正交状态机） |
|---|---|---|
| 判向方式 | 只在 A 相边沿读 B | A、B 都进中断，按状态转移查表 |
| 转一格脉冲数 | **2** | **4** |
| `EC12_PULSES_PER_STEP` | 自动取 **1**（转一格走 2 步） | 自动取 **4**（转一格走 1 步） |
| 抖动表现 | 边沿那刻 B 在抖 → 方向判反 → 该格净 0 → **「偶尔转两次才动」** | 抖动回弹被 +1/-1 抵消，不破坏计数 |
| 真机状态 | ★ **用户 2026-11-05 实测配 1 可用** | **未上真机**（已编译通过 + 主机测试通过） |

`EC12_PULSES_PER_STEP` 在 `App/App.h` 里**按开关自动取值**，不要手填：

```c
#if EC12_USE_QUADRATURE
#define EC12_PULSES_PER_STEP  4
#else
#define EC12_PULSES_PER_STEP  1
#endif
```

**为什么老方案要配 1 而不是 2**：老方案转一格只产生 2 个 A 边沿 = 2 个脉冲。配 2 就是「转一格 = 恰好 1 步」，**余量为零**——只要抖动吃掉或反转 1 个脉冲，这格就不走步，下一格才凑满，表现就是"转两次才动一次"。配 1 等于"转一格走 2 步"，容错翻倍，所以症状消失。

**但配 1 是绕过不是修复**：转一格温度跳 10 度（评分表 A11「步长为 5」会扣分），抖动本身没解决，只是被更大的步数掩盖。想真正解决就切 `EC12_USE_QUADRATURE = 1`。

**切到方案 1 要注意**：`Library/EXTI_config.h` 的 `USE_EXTI_13` 必须为 1（B 相接 PD13）。当前已是 1，且方案 0 下提供了一个空的 `EXTI13_on_trig()` 避免链接报错。

### 5.3.1 方向约定（两套方案共用，已用穷举复核）

本驱动的判向规则是「A 下降沿时 B 高 = 右转；A 上升沿时 B 低 = 右转」。用穷举求与该规则一致的四态闭环（state = (A<<1)|B）：

```
右转  0 -> 2 -> 3 -> 1 -> 0
左转  0 -> 1 -> 3 -> 2 -> 0
```

正交状态机的 `quad_table[16]` 就是按「右转每步 +1、左转每步 -1」填出来的。**这张表曾经填反过一次，导致左右完全颠倒**，已用穷举 + 主机测试复核。

## 六、对参考驱动的 4 处改动

| 文件 | 改动 | 为什么 |
|---|---|---|
| `bsp_adkey.c` | `ADKEY_LONG_MS` 1000u → **2000u** | md 要求「长按2秒」 |
| `bsp_adkey.c` | 两个回调加 `__attribute__((weak))` | 允许 App 层覆盖，与 `07_GD32_Exam1` 的 `Hardware/bsp_keys.c` 惯例一致（那是另一个工程的文件，不在本工程内） |
| `bsp_ec12.c` | 两个旋转回调加 `__attribute__((weak))` | 同上 |
| `bsp_ec12.c/.h` | 判向做成两套并存（`EC12_USE_QUADRATURE` 切换） | 见 §5.3。默认老方案（用户实测可用），可选正交状态机 |
| `EXTI_config.h` | `USE_EXTI_13` 0→1，EXTI13 端口 GPIOC→**GPIOD** | B 相接 PD13，供方案 1 使用 |
| `bsp_ws2812.c` | `if (index > WS2812_NUM)` → `>=` | **原代码越界**：`WS2812_NUM=4` 时 index=4 会写 `colors[4][...]`。注意这条用户重写文件时又写回 `>` 过，已再次修正 |
| `bsp_ws2812.c/.h` | **用户新增** `WS2812_set_color_brightness()` | 亮度 0~100。业务层统一走这个接口，按 `FRYER_LED_BRIGHTNESS` 缩放 |
| `bsp_led188.h` | 补 `void LED188_clear(void);` 声明 | `.c:90` 有定义但 `.h` 漏了，不补就编不过 |

另：`bsp_voltage.c` / `bsp_current.c` **已删除**（连同 .h）。原因是 `ADC0_CHNS_LEN` 只配了 1 个通道，而它们读 `ADC0_get(1)` / `ADC0_get(2)`，是**越界读**。考试用不到这两个驱动，且它们不在 `uvprojx` 的编译列表里。

删后重编译体积**一字未变**（当时 Code=10556 RO=452 RW=104 ZI=1368，两次完全一致），证明删的确实是死文件。

---

## 六之二、WS2812 亮度控制

用户给驱动加了带亮度的接口：

```c
void WS2812_set_color_brightness(uint16_t index, uint32_t color, uint8_t brightness);
// brightness: 0~100，100 = 满亮度
// 内部按 (v * brightness + 50) / 100 缩放 RGB，+50 是四舍五入
```

业务层统一走这个接口，亮度值在 `App/App.h` 里：

```c
#define FRYER_LED_BRIGHTNESS  10      // 满亮度 100 时取 10
```

**实际换算结果**（已用主机测试写死断言验证）：

| 逻辑色 | 原始值 | 亮度 10 后 | 说明 |
|---|---|---|---|
| 红（加热中） | `0xff0000` | `0x1A0000` | 255→26 |
| 绿（停止/待机） | `0x00ff00` | `0x001A00` | 255→26 |
| 灭（关机） | `0x000000` | `0x000000` | 黑色缩放后仍是黑 |

满亮度对照：`scale(0xff0000, 100) == 0xff0000`，证明换算函数本身没写错。

**为什么颜色宏搬到了 `App.h`**：`App_init()` 里也要用 `COLOR_OFF` 清场，`static` 函数搬不动，所以 `Fryer_set_all_led` 改成了公开的 `App_fryer_led_set`，颜色宏一并移到 `App.h`。

## 七、验证做到了什么程度

### 7.1 固件三层验证（全部通过）

| 层 | 结果 | 证据 |
|---|---|---|
| 编译 | **0 Error, 0 Warning** | UV4 命令行，`build4.log` |
| 链接 | **未解析符号 = 0** | `.map` 里 `Undefined Symbol` 条数 0（`Undefined Weak Reference` 是正常的） |
| 产物 | Code=10816 RO=452 RW=104 ZI=1368；hex 实测 **11376 字节**，占 512KB Flash 的 **2.12%** | 解析 hex 记录逐条累加 |

`.map` 里确认 `TIMER_channel_duty_update` 被链接器 **Removing（80 bytes）** 回收了，说明 `OneElfS=1` 的函数级回收是生效的。

### 7.2 业务逻辑主机侧验证

#### 7.2.1 主业务测试（`Test/test_fryer.c`）



把 `App_fryer.c` / `App_key.c` **原样 include** 进一个主机测试程序（不改它们一个字节），硬件驱动换成记录用的桩函数，逐条断言评分表的每个得分点。

```
===== 结果：通过 85，失败 0 =====
```

覆盖：开机/关机/默认值/关机时功能无效/模式切换/温度步长与范围/时间步长与范围/
运行中禁止加减/启动强制时间模式/倒计时/暂停与恢复/归零复位/编码器脉冲换算/关机清残余/亮度。

#### 7.2.2 编码器判向专项测试（`Test/test_ec12.c`）

编译**真实的 `Hardware/bsp_ec12.c`**（单独编译，不 include），两套方案各跑一遍：

```
gcc -std=c11 -I Test/stub -I Hardware -I Library     -o Test/test_ec12.exe Test/test_ec12.c Hardware/bsp_ec12.c
./Test/test_ec12.exe
```

| 断言 | 方案 0 | 方案 1 |
|---|---|---|
| 右转一格 | right=2 left=0 | right=4 left=0 |
| 右转三格 | 6/0 | 12/0 |
| 左转一格 | 0/2 | 0/4 |
| 左转三格 | 0/6 | 0/12 |
| 右转一格再左转一格 | 2/2 净值 0 | 4/4 净值 0 |
| **结果** | **10/10 通过** | **10/10 通过** |

★ **不能把 `bsp_ec12.c` include 进测试**：`weak` 只在跨翻译单元链接时生效，include 会变成同一个 TU，直接报重定义。用宏改名绕过也不行——会把 `bsp_ec12.c` 内部对回调的调用一起改掉，计数恒为 0（这两个坑都踩过）。

### 7.3 测试本身也做了变异验证（21/21 抓到）

「测试通过」不等于「测试有效」。所以故意把产品代码改坏 21 次，看测试能不能抓到：

```
温度上限 190->200        抓到      默认温度 100->120   抓到
时间上限 60->50          抓到      温度步长 5->10      抓到
默认时间 10->20          抓到      时间步长 1->2       抓到
运行中允许加减            抓到      归零不清默认值       抓到
启动不强制时间模式         抓到      启动不点红灯        抓到
暂停不点绿灯              抓到      开机不点绿灯        抓到
闪电标志反了              抓到      百分号标志不亮       抓到
倒计时算错(减半)          抓到      归零不停加热        抓到
KEY1关机时仍切换          抓到      KEY2关机时仍启动     抓到
关机不关红灯              抓到

抓到 21/21
```

过程中发现并修好了**两个测试自身的漏洞**（这两个都是"测试假通过"，不是产品 bug）：

1. **[12] 运行中禁止加减**：测试时温度正好 190 上限、时间正好 60 上限，即使代码允许加减，值也被范围钳制住"看起来没变"→ 变异漏掉。修法：先把值调到区间中部再启动。
2. **[11] 启动显示时间**：测试时已经在时间模式，删掉"强制切时间模式"也看不出来。修法：补一个**从温度模式直接按启动**的用例（这才是评分表的真实场景）。

### 7.4 编码与公开内容自查

- 我写/改过的文件全部 **GBK + CRLF**，无混行尾。5 个 UTF-8 文件是 GD 官方库原件（`systick.c` 等），我没动，保持原样。
- 品牌字样扫描 59 个文件：**0 命中**。

---

## 八、本文没验证的部分（重要）

| 未验证项 | 为什么没验证 | 怎么验 |
|---|---|---|
| **方案 1（正交状态机）的手感** | 未上真机 | 切 `EC12_USE_QUADRATURE=1` 后烧录，转一格应走 1 步；若走 2 步或方向反，见 §5.3 调 `EC12_PULSES_PER_STEP` |
| **老方案转一格跳 2 步是否影响评分** | 用户实测可用但未确认老师口径 | 转一格温度跳 10 度（步长 5 的两倍），评分表 A11 可能扣分。若在意就切方案 1 |
| **ADKey 三个键的实际 ADC 值** | 需要真机读 `printf` | 串口看 `key1/key2/key3 click` 是否对应按下的键 |
| **WS2812 颜色是否真的是红/绿** | 需要真机看灯 | 若红绿颠倒，是 GRB 顺序问题，改 `bsp_ws2812.c` 里 G/R 的发送顺序 |
| **亮度 10 是否合适** | 需要真机看 | 觉得太暗改 `FRYER_LED_BRIGHTNESS`（0~100）；注意 **亮度 0 = 全灭**，会看不出红绿区分 |
| **长按 2000ms 手感是否合适** | 需要真人按键 | 若觉得太钝，改 `bsp_adkey.c` 的 `ADKEY_LONG_MS` |
| **数码管亮度/闪烁** | 需要真机看 | 2ms/相 = 100Hz 理论无闪烁；若闪，检查是否被长任务阻塞 |
| **上电瞬间是否有残留显示** | 需要真机看 | `App_init()` 里已调 `LED188_clear()` 清掉驱动自检的 123，但上电到 `App_init` 之间仍可能闪一下 |
| **倒计时 1 秒的边界** | 主机测试用整数毫秒推进，真机有 ±10ms 抖动 | 真机数 10 秒看是否准 |

**没有上真机跑过一次。** 上面 7.1～7.4 全部是编译产物与主机仿真的证据，硬件行为一律未验证。

---

## 九、怎么编译

```bash
"D:/Develop/Keil_v5/UV4/UV4.exe" -b \
  "D:/Develop/embedded-learning/04_ARM32/01_GD32/08_GD32_Exam2/Project/GD32F407.uvprojx" \
  -j0 -o build.log
```

**`-b` 后面的路径必须是绝对路径**，否则 UV4 退出码 2 且不产生日志。

产物：`Project/Objects/GD32F407.hex`（内含 11376 字节程序数据）。

---

## 十、文件结构

```
08_GD32_Exam2/
├─ App/
│  ├─ App.h            考试参数宏 + 状态声明 + 任务声明
│  ├─ App_fryer.c      ★ 核心业务：状态机 / 倒计时 / 数码管内容 / WS2812 颜色
│  ├─ App_key.c        ★ 按键与编码器回调 + 脉冲消费
│  └─ App_debug.c      串口接收回调（只为满足 USART0_RECV_CALLBACK=1）
├─ User/
│  ├─ main.c           外设初始化 + 任务循环
│  ├─ Task.c           任务表 + 1ms 时基
│  ├─ systick.c/.h     GD 官方（未改）
│  └─ gd32f4xx_it.c/.h GD 官方（未改）
├─ Hardware/
│  ├─ bsp_led188.c/.h  188 数码管（补了 LED188_clear 声明）
│  ├─ bsp_adkey.c/.h   ADKey 按键（长按改 2000ms，回调加 weak）
│  ├─ bsp_ec12.c/.h    旋转编码器（回调加 weak）
│  └─ bsp_ws2812.c/.h  WS2812（修了越界判断；亮度接口为用户新增）
├─ Library/            EXTI / TIMER / ADC / SPI1 / USART0 / gpio_cfg
└─ Project/            GD32F407.uvprojx
```

★ 标记的是本次考试新写的文件，其余来自参考工程。
