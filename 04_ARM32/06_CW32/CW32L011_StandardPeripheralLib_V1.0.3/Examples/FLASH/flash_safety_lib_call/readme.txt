示例目的：
          举例说明安全运行库的使用

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI，默认频率4MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=4MHz
          3. LED2

演示说明：
         演示如何调用安全运行库区的函数

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
