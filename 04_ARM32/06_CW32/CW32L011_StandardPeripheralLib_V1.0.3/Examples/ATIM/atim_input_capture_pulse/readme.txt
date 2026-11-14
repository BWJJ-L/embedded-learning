示例目的：
          演示ATIM输入捕获。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟12分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ATIM边沿计数，上计数模式，1分频，ARR设置为0xFFFF.
          5. PA11设置为PUSH-PULL输出，驱动LED2
          6. PA15设置为输入，检查KEY2的输入
          7. PA05设置为输入，复用为BTIM2TOGN
          8. PA08设置为输入，复用为ATIMCH1

演示说明：
          短接PA05和PA08，将BTIM2的TOGN信号引入到ATIM的CH1
          配置BTIM2以2MHz频率计数，使能TOG功能，通过PA05输出当前BTIM2的溢出周期，模拟脉冲信号。
          KEY2用于改变BTIM2的溢出周期，范围为1~13kHz，步长2kHz。
          LED2闪烁指示KEY2按下有效。
          ATIM以8MHz频率计数，配置IC1从端口CH1上捕获，升沿有效,配置IC2从端口CH1上捕获,下降沿有效.
          使能CC1和CC2的中断,在中断中处理BTIM2_TOG信号的周期和脉宽

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
