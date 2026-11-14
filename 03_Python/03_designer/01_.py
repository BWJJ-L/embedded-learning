
# 导入模块
import sys
from PyQt5.QtWidgets import *   # 窗口相关
from PyQt5.QtGui import *       # 图形相关
from PyQt5.QtCore import *      # 事件相关

# 1、导入包中模块
from UI.Ui_01_测试 import Ui_Dialog

class MyWidget(QWidget):
    def __init__(self):
        super().__init__() # 调用QWidget的__init__方法
        # 2、通过类创建对象
        self.ui = Ui_Dialog()
        # 3、上面的对象调用setupUi
        self.ui.setupUi(self)
        
        self.init_ui()  
        
    def init_ui(self):
        self.ui.btnLogin.clicked.connect(lambda:print("登录"))
        self.ui.btnCancel.clicked.connect(lambda:print("取消"))


if __name__ == '__main__':
    # 创建应用程序
    app = QApplication(sys.argv)

    # 创建窗口对象，不是QWidget类，是MyWidget类
    w = MyWidget()
    
    # 调用显示方法
    w.show()

    # 等待窗口停止
    sys.exit(app.exec_())