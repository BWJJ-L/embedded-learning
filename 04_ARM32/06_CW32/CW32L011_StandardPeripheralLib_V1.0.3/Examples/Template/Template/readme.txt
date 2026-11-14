示例目的：
          演示StartKit板上的LED1，LED2，KEY1，KEY2和串口的使用。

硬件资源：
          1. CW32L011K8T6 StartKit  
          2. 时钟HSI
          3. 系统时钟源为HSI时钟24分频，4MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=4MHz
          4. LED1，LED2, KEY1, KEY2
          5. 板载USB转串口配置为115200bps，作为调试打印
          6. 板上J3的VDDIN跳线短接，PCRXD跳线短接。

演示说明：
          LED1和LED2常亮，调试串口输出“Welcome to use CW32L011.”
          KEY1按下LED1熄灭，释放LED1点亮
          KEY2按下LED2熄灭，释放LED2点亮

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