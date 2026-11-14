示例目的：
          演示ATIM输入捕获内部信号。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI， LSI
          3. 系统时钟设置为HSI时钟2分频，48MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=48MHz
          4. ATIM边沿计数，上计数模式，1分频，ARR设置为0xFFFF.
          

演示说明：
          ATIM以48MHz频率计数，配置IC1捕获LSI, 上升沿有效,配置IC2捕获LSI,下降沿有效.
          使能CC1和CC2的中断,在中断中处理LSI信号的周期和脉宽。

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
