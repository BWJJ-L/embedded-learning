import PyInstaller.__main__

# 这里的是不需要打包进去的三方模块，可以减少软件包的体积，这里只是举个例子
excluded_modules = [
    "pytest",
    "selenium",
]

append_string = []
for mod in excluded_modules:
    append_string += [f'--exclude-module={mod}']

PyInstaller.__main__.run([
    '-y',  # 如果dist文件夹内已经存在生成文件，则不询问用户，直接覆盖
    '01_.py',  # 主程序入口

    '--onedir',  # -D 文件夹
    # '--onefile', # -F 单文件

    # '--nowindowed', # -c 无窗口
    '--windowed',  # -w 有窗口

    '-n', 'utils',  # -n 生成的文件名，默认是程序的名称
    '-i', './1.jpg',  # -i 生成的图标，默认是程序的图标
    '--add-data=res;res',  # 第一个 res 是当前工作目录下的资源文件夹路径
    # 第二个 res 是打包后资源文件在应用程序中的相对路径
    *append_string
])