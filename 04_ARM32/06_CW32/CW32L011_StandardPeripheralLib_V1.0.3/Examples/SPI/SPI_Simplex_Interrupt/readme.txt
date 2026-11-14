示例目的：
         SPI单工模式主从通信（中断方式）。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟12分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. SPI，波特率为PCLK的8分频，1Mbps
		  5. PA11（LED2）设置为PUSH-PULL输出	  
		
演示说明：
           - 
		  -主机模式
		  Preprocessor Symbols Define:  SPI_MASTER 
          主机中断方式发送TxBuffer缓冲区中的数据。
		  
		  -从机模式
		  Preprocessor Symbols Define:  SPI_SLAVE  
          从机中断方式接收数据，并存储到RxBuffer缓冲区。
          比较TxBuffer和RxBuffer，
          如果数据一致，LED2亮，否则LED2灭。


硬件连接：
          SPI_MASTER SCK  (PA05)-- SPI_SLAVE SCK  (PA05)
		  SPI_MASTER MOSI (PA07)-- SPI_SLAVE MOSI (PA07)
          SPI_MASTER CS   (PA04)-- SPI_SLAVE CS   (PA04)

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
