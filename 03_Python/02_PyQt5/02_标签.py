
# 导入模块
import sys
from PyQt5.QtWidgets import *   #窗口相关
from PyQt5.QtGui import *       #图形相关
from PyQt5.QtCore import *      #事件相关

# 创建应用程序
app = QApplication(sys.argv)

# 创建窗口对象
w = QWidget()
# =========================================显示文本
# 创建QLabel标签对象
LabelText = QLabel()
LabelText.setText("这不是标签<^$^>")
# 指定父窗口(父对象)  标签放在窗口上
LabelText.setParent(w)
# 设置位置
LabelText.move(0, 0)

# =========================================显示图片
# 创建QLabel标签
LabelImg = QLabel()
LabelImg.setPixmap(QPixmap("./img/face.png"))
# 指定父窗口(父对象)  标签放在窗口上
LabelImg.setParent(w)
# 设置位置
LabelImg.move(0,50)

# =========================================显示gif动画
# 创建动画对象
gif = QMovie("./img/mario.gif")
# 创建标签对象
gifLabel = QLabel()
# 标签对象设置动画
gifLabel.setMovie(gif)
# 动画启动
gif.start()
# 标签指定父窗口
gifLabel.setParent(w)
# 标签所在位置
gifLabel.move(200, 0)
# 设置标签大小
# gifLabel.resize(1000, 1000)
# 标签内容设置自动适应
gifLabel.setScaledContents(True)



# 调用显示方法
w.show()

# 等待窗口停止
sys.exit(app.exec_())