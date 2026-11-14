
# 导入模块
import sys
from PyQt5.QtWidgets import *   #窗口相关
from PyQt5.QtGui import *       #图形相关
from PyQt5.QtCore import *      #事件相关

# 创建应用程序
app = QApplication(sys.argv)

# 创建窗口对象
w = QWidget()

# 创建布局对象
# LayOut = QHBoxLayout()  # 水平布局
LayOut = QVBoxLayout()  # 垂直布局
# 窗口设置布局
w.setLayout(LayOut)
w.setLayout(LayOut)

for i in range(5):
    # 创建按钮
    btn = QPushButton()
    # 设置按钮文本
    btn.setText(f"按钮{i}")
    # 把控件添加到布局容器中
    LayOut.addWidget(btn)

# 调用显示方法
w.show()

# 等待窗口停止
sys.exit(app.exec_())