示例目的：
          演示LPTIM作为定时器的基本用法。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI，LSE
          3. 系统时钟由HSI提供, 12分频到8MHz，PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=8MHz
          4. 外部低速时钟LSE提供32768Hz时钟给LPTIM定时
          5. PA11设置为PUSH-PULL输出，驱动LED2
          
演示说明：
          LPTIM的内部时钟源选择为LSE，并选择以内部时钟进行计数。
          ARR设置为32768-1，使能ARRM的中断，LPTIM将每秒产生一次ARRM的中断。
          中断服务程序中对PA11口线翻转，LED2将每秒闪烁一次。
          
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
