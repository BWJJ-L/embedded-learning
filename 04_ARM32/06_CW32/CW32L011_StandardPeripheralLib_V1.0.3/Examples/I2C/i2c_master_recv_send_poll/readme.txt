示例目的：
         利用I2C接口， 实现I2C主设备功能，和I2C从机进行数据通信。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟分频到16MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=16MHz
          4. PA10复用为SDA PA09复用为SCK

演示说明：
1、时钟配置16MHz，初始化I2C为主接口，1MHz，不使能I2C中断。（1MHz波特率，PCLK和HCLK需16MHz以上）
2、程序流程：程序完成I2C主设备配置后，先将u8Senddata数组中的内容发给从机。延时一段时间后，启动数据接收流程，将接受的数据存入u8Recdata数组中。
3、注意实际测试时需要更改I2C/GPIO口线、时钟配置以及GPIO配置都需要修改。


使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开i2c_master_recv_send.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
