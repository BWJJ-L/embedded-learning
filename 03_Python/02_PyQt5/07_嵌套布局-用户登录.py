
# 导入模块
import sys
from PyQt5.QtWidgets import *   #窗口相关
from PyQt5.QtGui import *       #图形相关
from PyQt5.QtCore import *      #事件相关

# 创建应用程序
app = QApplication(sys.argv)

# 创建窗口对象
w = QWidget()

# 垂直布局
VLayOut = QVBoxLayout()
# 窗口设置布局
w.setLayout(VLayOut)

# ================================== 用户名布局
userLayOut = QHBoxLayout()
VLayOut.addLayout(userLayOut)
# 用户名标签
userLabel = QLabel('用户名:')
userLayOut.addWidget(userLabel)
# 用户名编辑框
userEdit = QLineEdit()
userLayOut.addWidget(userEdit)
# ================================== 密码布局
pwdLayOut = QHBoxLayout()
VLayOut.addLayout(pwdLayOut)
# 密码标签
pwdLabel = QLabel('密  码:')
pwdLayOut.addWidget(pwdLabel)
# 密码编辑框
pwdEdit = QLineEdit()
pwdEdit.setEchoMode(pwdEdit.Password)   # 密码模式
pwdLayOut.addWidget(pwdEdit)
# ================================== 登录 取消按钮
btnLayOut = QHBoxLayout()
VLayOut.addLayout(btnLayOut)
# 登录按钮
loginBtn = QPushButton('登录')
btnLayOut.addWidget(loginBtn)
# 取消按钮
cancelBtn = QPushButton('取消')
btnLayOut.addWidget(cancelBtn)
# ================================== 信号和槽
def cancelBtnSlot():
    print("取消按钮被点击了！")
    userEdit.clear()
    pwdEdit.clear()

def loginBtnSlot():
    print("登录按钮被点击了！")
    user = userEdit.text()
    pwd  = pwdEdit.text()
    # 如果内容为空
    if user == "" or pwd == "":
        print("用户名或密码不能为空！")
        # 弹出警告框
        #                 父窗口,标题,  正文内容
        QMessageBox.warning(w, '警告', '用户名或密码不能为空！')
        # QMessageBox.critical(w, '警告', '用户名或密码不能为空')
        return
    # 密码账户输入
    if user == "admin" and pwd == "123456":
        print("登录成功！")
        # 提示成功框
        QMessageBox.information(w, '成功', '登录成功')
    else:
        print("登陆失败！")
        QMessageBox.information(w, '失败', '用户名或密码错误')    


loginBtn.clicked.connect(loginBtnSlot)    # 登录按钮
cancelBtn.clicked.connect(cancelBtnSlot)  # 取消按钮


# 调用显示方法
w.show()

# 等待窗口停止
sys.exit(app.exec_())