示例目的：
          演示ATIM的PWM移相功能

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟12分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ATIM中心对齐模式计数，计数时钟1MHz,ARR设置为1000, 使用ATIM的CH1和CH2通道输出PWM信号
	     开启移相后，通道1输出的 OC1REFC的为OC1REF 和 OC5REF 的逻辑与。
          5. PA05设置为ATIM_CH1通道的输出.
          6. PA09设置为ATIM_CH2通道的输出

演示说明:
         PA05与PA09输出PWM波形.,按下按键后对通道1的波形进行移相。

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开project.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
