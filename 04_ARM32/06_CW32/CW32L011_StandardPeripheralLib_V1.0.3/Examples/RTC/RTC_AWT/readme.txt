示例目的：
          RTC时钟AWT功能测试，AWT可以在不开启RTC模式下工作

硬件资源：
          1. CW32L011FxPx StartKit
          2. 时钟HSI， LSE使能
          3. 系统时钟设置为HSI时钟12分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. RTC使用LSE作为时钟输入
          5. 串口UART1作为调试串口,打印调试信息,波特率115200          

演示说明：
          每2秒通过串口输出AWT中断请求信息.

硬件连接：
          UART1_TXD (PA02) -- PCRX
          UART1_RXD (PA03) -- PCTX  

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
