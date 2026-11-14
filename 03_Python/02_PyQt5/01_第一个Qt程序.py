
# 导入模块
import sys
from PyQt5.QtWidgets import *   #窗口相关
from PyQt5.QtGui import *       #图形相关
from PyQt5.QtCore import *      #事件相关

# 创建应用程序
app = QApplication(sys.argv)

# 创建窗口对象
w = QWidget()
# 设置标题
w.setWindowTitle("白马窗口")
# 设置图标
w.setWindowIcon(QIcon("../1.jpg"))
# 设置大小
w.resize(500, 500)
# 设置位置
w.move(1000, 500)

# 设置提示信息
w.setToolTip("哈哈哈哈")

# 调用显示方法
w.show()

# 等待窗口停止
sys.exit(app.exec_())