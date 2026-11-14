示例目的：
          演示复位标志的产生和识别。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSIOSC，24分频，PCLK=HCLK=SysClk=4MHz。
          3. 使用版上资源LED1，KEY1，KEY2，调试串口，波特率115200。
          4. LVD

演示说明：
          J24选择DCIN供电，J1选择3V供电，J3短接UVDD，PCRXD。
          DCIN提供3.6V电源上电，LED1会闪烁3次，指示程序开始运行，并且串口打印“Welcome to use CW32L011”
          及复位原因.按下KEY1产生SYSRESETREQ复位，按下KEY2产生LOCKUP复位，按下RESET产生
          NRST Pin复位。将电源调制3.4V以下，MCU保持复位，LED1熄灭，在将电源调制3.4V以上，MCU将产生LVD复位。
          选择3.4V为LVD的门限，是需要保证板上的串口芯片能正常工作。
           

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开rcc_pclkout.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
