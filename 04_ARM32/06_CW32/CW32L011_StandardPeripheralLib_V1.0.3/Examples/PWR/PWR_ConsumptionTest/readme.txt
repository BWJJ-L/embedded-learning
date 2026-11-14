示例目的：
          演示MCU通过按键进入深度休眠模式，再通过按键切换唤醒方式。

硬件资源：
          1. CW32L011 StartKit
          2. 时钟HSI，LSI
          3. 板上按键KEY1，KEY2
          4. 板上USB转串口
          5. PA04 输出MCO

演示说明：
        上电后，MCU以最高频率96MHz运行，每按一次KEY1，主频将按48MH，32MHz， 16MHz，8MHz，4MHz依次改变。PA04输出当前HCLK的频率，调试串口输出当前的运行频率。
        切换到4MHz运行后，进入deepsleep状态。
        按下KEY2可唤醒MCU，首次唤醒MCU启动IWDT然后进入deepsleep，第二次唤醒关闭IWDT，启动LPTIM，然后进入deepsleep，
        第三次唤醒关闭LPTIM，启动uart1，然后进入deepsleep，第四次唤醒，关闭uart1，启动RTC，然后进入deepsleep，第5次唤醒
        将同时启动IWDT，LPTIM，Uart1和RTC，然后进入deepsleep。第6次唤醒后，不进入deepsleep，程序等待约4s后，IWDT发生超时
        复位MCU。
        MCU在休眠时，如果被LPTIM，RTC和UART的中断唤醒，将打印出相应的唤醒源。 
         
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
