示例目的：
          利用I2C接口，采用中断方式读写EEPROM芯片（CW24C02）。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟24分频（默认），4MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=4MHz
          4. PA10 复用为SDA PA09复用为SCK

演示说明：
          1.采用默认时钟配置，初始化I2C为主接口，100KHz，使能I2C中断。
          2.I2C通过中断的方式，按每页8个字节向24C02中写入数据0xFF。
          3.通过查询24C02是否应答主机的命令，判断写过程结束
          4.将整个24C02写入0xFF
          5.通过中断的方式读取整片24C02的内容，如果不全为0xFF，则程序进入死循环，PA11置高，LED2亮起
          6.I2C通过中断的方式，按每页8个字节向24C02中写入数据
          7.通过查询24C02是否应答主机的命令，判断写过程结束
          8.将整个24C02写入预定数据
          9.通过中断的方式读取整片24C02的内容，如果读取的数据与写入的数据不一致，则程序进入死循环，PA11置高，LED2亮起
          10.24C02操作过程正确结束，PA11不停翻转，LED2闪烁。
          

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开i2c_cw24c02_int.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
