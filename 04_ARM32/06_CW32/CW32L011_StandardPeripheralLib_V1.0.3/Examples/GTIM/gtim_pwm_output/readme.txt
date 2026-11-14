示例目的：
          演示GTIM的PWM输出功能

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI，24分频，4MHz，PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=4MHz
          4. GTIM1的分频系数4-1，ARR设置1000-1，CCR3为250         
          5. 设置PB00为GTIM1的CH3通道

演示说明：
          程序运行后,PB00输出周期为1ms， 正脉宽为250us的方波
          
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
