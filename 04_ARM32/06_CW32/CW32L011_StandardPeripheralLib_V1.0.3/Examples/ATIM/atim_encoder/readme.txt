示例目的：
          演示ATIM编码器模式。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ATIM为编码器模式，重载值为1000
          5. PA05为ATIM的CH1、PA09为ATIM的CH2
          6. GTIM边沿计数，上计数模式，800分频，ARR设置为100

演示说明：
          PB04、PA11两通道连接编码器，ATIM在编码器模式下根据正转和反转使计数值递增还是递减
          GTIM定时10ms获取计数值，编码器线数计算转数。

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
