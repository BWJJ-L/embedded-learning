示例目的：
         利用I2C接口， 实现I2C从设备功能，和I2C主机进行数据通信。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟分频到16MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=16MHz
          4. PA10复用为SDA PA09复用为SCK

演示说明：
1、初始化I2C为从机，不使能I2C中断。
2、从机先等待接收数据，接收完成主机发送的数据后，将接收的数据在发送给主机。
3、注意实际测试时需要更改I2C/GPIO口线、时钟配置以及GPIO配置都需要修改。


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
