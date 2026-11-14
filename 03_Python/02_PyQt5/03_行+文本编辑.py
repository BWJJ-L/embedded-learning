
# 导入模块
import sys
from PyQt5.QtWidgets import *   #窗口相关
from PyQt5.QtGui import *       #图形相关
from PyQt5.QtCore import *      #事件相关

# 创建应用程序
app = QApplication(sys.argv)

# 创建窗口对象
w = QWidget()

# ==========================================创建行编辑对象
LineEdit = QLineEdit()
# 指定父窗口（父对象）  标签放在窗口上
LineEdit.setParent(w)
#设置内容
LineEdit.setText("hello world!")
LineEdit.setText("123456")  # 只会显示最后一个
# 获取内容
msg = LineEdit.text()
print(f"行编辑内容：{msg}")
# 设置显示模式：密码模式
LineEdit.setEchoMode(LineEdit.Password)


# ==========================================创建文本编辑对象
TextEdit = QTextEdit()
# 指定父窗口（父对象）  标签放在窗口上
TextEdit.setParent(w)
# 设置内容
TextEdit.setText("lyfnihao")
# 调整“文本编辑”位置
TextEdit.move(0, 100)
msg = TextEdit.toPlainText()
print(f"文本编辑内容：{msg}")


# 调用显示方法
w.show()

# 等待窗口停止
sys.exit(app.exec_())