
# 导入模块
import sys
from PyQt5.QtWidgets import *   #窗口相关
from PyQt5.QtGui import *       #图形相关
from PyQt5.QtCore import *      #事件相关

# 创建应用程序
app = QApplication(sys.argv)

# 创建窗口对象
w = QWidget()


# ========================================== 创建按钮对象
btn = QPushButton()
# 指定父窗口(父对象) 按钮放在窗口上
btn.setParent(w)
# 设置内容
btn.setText("<^￥^>")


# 调用显示方法
w.show()

# ========================================== 信号和槽处理
# btn按钮产生了按下抬起的信号，自动调用xxx函数
# btn.clicked.connect(btnSlot)

# 按下了，就打印按下    lambda表达式
btn.pressed.connect(lambda:print("按下"))
# 抬起了，就打印抬起    lambda表达式
btn.released.connect(lambda:print("抬起"))


# 等待窗口停止
sys.exit(app.exec_())