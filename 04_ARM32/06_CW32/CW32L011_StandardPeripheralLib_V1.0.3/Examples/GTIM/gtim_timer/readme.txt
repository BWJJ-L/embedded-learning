示例目的：
          演示GTIM的定时器功能

硬件资源：
          1. CW32L011FxPx StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI，24分频，96MHz/12=4MHz，PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=4MHz
          4. GTIM1的分频系数1000-1，ARR设置200-1，则GTIM1每50ms溢出一次          
          5. 设置PA11为PUSH-PULL输出,驱动LED2

演示说明：
          程序运后,PA11每50ms翻转一次，驱动LED2以10HZ的频率闪烁
          
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
