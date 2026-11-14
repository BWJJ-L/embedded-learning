
# 导入模块
import sys
from PyQt5.QtWidgets import *   #窗口相关
from PyQt5.QtGui import *       #图形相关
from PyQt5.QtCore import *      #事件相关

# 创建应用程序
app = QApplication(sys.argv)

# 创建窗口对象
w = QWidget()


# 窗口设置大小
w.resize(400, 300)
# 创建按钮，内容为“start”，指定父对象
btn = QPushButton("Start", w)
# ========================================== 信号和槽
def btnSlot():
    # 获取按钮上面的内容
    msg = btn.text()
    if msg == "Start":
        btn.setText("Stop")
    else:
        btn.setText("Start")
# 点击后切换按钮显示内容
btn.clicked.connect(btnSlot)
# 点击后关闭窗口
# btn.clicked.connect(w.close)
# 点击后最大化窗口
# btn.clicked.connect(w.showMaximized)
# 点击后窗口恢复正常大小
# btn.clicked.connect(w.showNormal)


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