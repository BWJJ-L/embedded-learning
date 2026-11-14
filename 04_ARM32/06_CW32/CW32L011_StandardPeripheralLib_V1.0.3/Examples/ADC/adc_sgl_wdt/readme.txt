示例目的：
          演示ADC模块单通道模拟看门狗功能。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟源为HSI时钟12分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ADC的时钟源为PCLK，8分频
          5. PA11设置为PUSH-PULL输出

演示说明：
          软件触发ADC模块进行单通道单次转换。LED02以0.5Hz的频率闪速
          ADC模拟看门狗通道为AIN0:PA02，下阈值为0x600, 上门限为0x700；
          外接可调直流电源到PA02，ADC转换完成，如果采样值小于下阈值，则PA11输出低，LED2灭；
          如果采样值大于上阈值，则PA11输出高，LED2亮。

使用说明：
+ EWARM
          1. 打开adc_sgl_wdt.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开adc_sgl_wdt.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)