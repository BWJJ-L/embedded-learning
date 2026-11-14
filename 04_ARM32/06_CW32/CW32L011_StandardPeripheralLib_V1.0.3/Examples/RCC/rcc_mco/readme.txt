示例目的：
          利用MCO时钟输出功能将各种所需要观测的时钟从PB03引脚输出，用户可根据需要修改时钟源和分频比。

硬件资源：
          1. CW32L01 StartKit  
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟4分频，24MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=24MHz
          4. PA15配置为浮空输入作为按键KEY2使用

演示说明：
        采用默认时钟配置，并设置MCO时钟输出，用户按下KEY2后，程序循环修改时钟源和分频比，用示波器观测PA04引脚的波形，默认程序为24MHz时钟信号。

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开rcc_mco.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)

