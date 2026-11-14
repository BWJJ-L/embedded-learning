示例目的：
          演示ATIM为主设备， BTIM1为从设备。BTIM1统计ATIM更新的次数。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟12分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ATIM边沿对齐模式计数，计数时钟1MHz,ARR设置为99，100us溢出,
             输出比较, 使用ATIM的CH1通道输出互补的PWM信号
             TRGO信号设置为CH1比较匹配
          5. PA05设置为ATIM_CH1通道的输出.
          6. BTIM1设置为计数模式，在从模式下，对TRGI信号计数，TRGI信号来源配置ATIM的TRGO信号
             3个TRGI信号后产生溢出更新中断
          6. PA08设置为输出，在BTIM1的中断服务程序中翻转，用于指示BTIM溢出的间隔。

演示说明:
          观测PA08和PA05的波形，可发现每3个PA05周期波形，将引起PA08电平翻转一次。

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
