示例目的：
          演示GPIO的位翻转

硬件资源：
          1. CW32L011FxPx StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟12分频，4MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=4MHz
          4. PA11设置为PUSH-PULL输出，驱动LED2

演示说明：
          LED2闪烁。

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
