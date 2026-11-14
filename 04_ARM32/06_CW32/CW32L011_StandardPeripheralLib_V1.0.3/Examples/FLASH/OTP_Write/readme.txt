示例目的：
        对MCU内置的OTP区写/读操作，并对比写入和读取的数据是否一致。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI，默认频率4MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=4MHz
          3. LED2

演示说明：
        采用默认时钟配置，开启FLASH的操作时钟。对MCU内置OTP区写以及读取，
        然后对比写入和读取的数据是否相同。然后再此对同一地址的OTP进行写入操作，
        执行应不成功。执行完毕后，LED2闪烁。

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
