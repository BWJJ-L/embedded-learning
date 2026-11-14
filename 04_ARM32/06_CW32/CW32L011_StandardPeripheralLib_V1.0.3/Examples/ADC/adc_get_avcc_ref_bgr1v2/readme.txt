示例目的：
         演示软件启动ADC单通道转换模式,以BGR为参考，获取AVCC和其它通道的电压。

硬件资源：
          1. CW32L011K8T6 StartKit
          2. 时钟HSI
          3. 系统时钟源为HSI，48MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=48MHz
          4. ADC的时钟源为PCLK/8分频，BGR采样保持时间至少40us，故采样周期选择262个ADC时钟
          5. PA05作为模拟信号的输入
          
演示说明：
          ADC配置为8通道序列采样，前7个通过都配置为内部BGR，最后一个通道配置为AIN5:PA05。
          ADC对BGR进行7次采样，并计算平均值，通过BGR保存在芯片内部的校准值，计算出当前AVCC和
          PA05上的模拟信号电压值，并通过UART2输出转换结果。     
         

使用说明：
+ EWARM
          1. 打开adc_get_avcc_ref_bgr1v2.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开adc_get_avcc_ref_bgr1v2.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)