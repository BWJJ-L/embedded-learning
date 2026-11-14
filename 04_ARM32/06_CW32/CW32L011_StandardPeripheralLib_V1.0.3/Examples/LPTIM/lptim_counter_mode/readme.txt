示例目的：
          演示LPTIM作为计数器的基本用法，对来自CH1的信号的脉冲个数进行计数。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟由HSI提供，12分频到8MHz， PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=8MHz
	      4. PA02设置为LPTIM_CH1的复用功能
          5. PA11设置为PUSH-PULL输出，驱动LED2
          
演示说明：
          系统时钟由HSI提供，频率为8 MHz。LPTIM的选择为计数模式，计数来源为CH1。ARR设置为6，使能ARRM的中断。
		  LPTIM设置为单次运行模式，运行前LED2点亮，当计数满10个脉冲后，将产生一次ARRM的中断。
          中断服务程序中将LED2熄灭。
          
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
