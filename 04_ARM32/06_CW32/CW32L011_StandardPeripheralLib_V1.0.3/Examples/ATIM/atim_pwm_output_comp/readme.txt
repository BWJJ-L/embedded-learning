示例目的：
          演示ATIM的PWM互补输出功能

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟12分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ATIM中心对齐模式计数，计数时钟1MHz,ARR设置为99，100us溢出,
             输出比较, 使用ATIM的CH1和CH1N通道输出互补的PWM信号
          5. PA05设置为ATIM_CH1通道的输出.
          6. PA07设置为ATIM_CH1通道的互补输出

演示说明:
         PA05与PA07输出带死区的互补波形.

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
