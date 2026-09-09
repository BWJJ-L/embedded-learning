# embedded-learning

嵌入式学习个人练习

## 目录结构

- 按学习阶段分文件夹，当前阶段：`STC8/`
  - 工程按进度编号（`demoNN_主题`）

## 环境

- IDE：Keil C51（STC8 增强型系列）
- 烧录：STC-ISP

## 硬件（STC8 阶段）

- STC8H8K64U芯片
- 为最小开发板，包含了最小系统，以及部分外设，所有引脚都已经引出，方便后续扩展使用

## 说明

- 仓库只保留源代码与工程配置，编译产物（`Objects/`、`Listings/` 等）由 `.gitignore` 文件排除
- `.hex` 烧录文件已保留跟踪，新电脑 clone 后可直接 STC-ISP 烧录
- "demo00_C语言基础练习题"题目大部分基于"浙大翁恺"B站课程，这是一个可以离线的刷题网页，所有说明都在 `STC8/demo00_C语言基础练习题/pta-c-practice/README.md` 内，使用前请阅读一遍，感谢使用