示例目的：
          演示VC模块的输出信号上升沿/下降沿触发中断功能。

硬件资源：
          1. CW32L011F8P6 StartKit
          2. 时钟HSI
          3. 系统时钟源为HSI时钟24分频，4MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=4MHz
          4. VC的滤波时钟源为PCLK
          5. PA11设置为VC输出端口

演示说明：
          VC输入P通道为PA00，N通道为BGR基准电压1.2V，VC输出端口为PA11。
          外接可调直流电源，当VC输入P通道电压低于N通道电压时刻，PA11输出高（LED2亮）。
		  调节P通道电压高于N通道电压时，PA11输出低（LED2灭）；
          
使用说明：
+ EWARM
          1. 打开vc_detect_falling_irq.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开vc_detect_falling_irq.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)