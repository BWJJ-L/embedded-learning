示例目的：
        演示LPTIM在休眠模式下，使用LSE作为时钟源，输出PWM波形。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI
          3. 系统时钟由HSI提供,12分频到8MHz，PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=8MHz
		  4. LSE提供32768Hz时钟
          5. PA04设置为PUSH-PULL输出，复用为LPTIM_OUT
		  6. PA06设置为输入，下降沿产生中断
          7. PA11设置为PUSH-PULL输出，驱动LED2，用于LED1指示
          
演示说明：
        LPTIM的选择为定时模式，计数来源为LSE。ARR设置为99，则输出频率为：
        输出频率 = 计数时钟频率 / （ARR +1）
		         = 32768 / 100
				 = 327.68Hz
        设置CMP的值为49，则占空比为：
        占空比 = 1 - （CMP）/ （ARR + 1）
               = 50%
        当LPTIM产生PWM后，系统进入休眠，以极低功耗运行。当KEY2按键后，产生中断，将系统从休眠中唤醒，PWM停止输出。
        按键中断产生后，LED2点亮，指示系统退出休眠状态。
          
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
