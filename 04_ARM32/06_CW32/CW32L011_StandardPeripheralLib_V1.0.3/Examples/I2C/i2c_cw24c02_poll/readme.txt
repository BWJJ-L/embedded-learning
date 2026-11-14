示例目的：
          利用I2C接口，采用查询方式读写EEPROM芯片（CW24C02）。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟24分频，4MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=4MHz
          4. PA10 复用为SDA PA09复用为SCK

演示说明：
          1、采用默认时钟配置，初始化I2C为主接口，100KHz，不使能I2C中断。
          2、PA11上电后输出高，指示程序开始运行
          3、读取24C02的全部内容，不全为0xFF，则擦除为0xFF，然后从0地址依次写入0~255，写入后
          读出，进行校验，校验通过PA11约500ms翻转一次，指示写入成功。         

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
