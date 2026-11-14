示例目的：
         利用I2C接口， 实现I2C从设备功能，和I2C主机进行数据通信。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟分频到16MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=16MHz
          4. PA10复用为SDA PA09复用为SCK
          5. UART1, I2C
          6. PB05复用为TXD ，PB06复用为RXD
          

演示说明：
1、初始化I2C为从机，使能I2C中断。
2、从机通过中断接收和发送数据。
3、从机接收到数据后通过UART2打印出来。
4、注意实际测试时需要更改I2C/GPIO口线、时钟配置以及GPIO配置都需要修改。


使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开i2c_slaver_recv_send.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
