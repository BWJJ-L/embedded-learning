示例目的：
           演示ATIM触发ADC采样的功能。。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟12分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ATIM边沿计数，上计数模式，800分频，ARR设置为9999，溢出周期1s
          5. PA11设置为PUSH-PULL输出，PB00为ADC的输入

演示说明：
          本例中，ATIM在PWM模式下配置ATIM用于触发ADC

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
