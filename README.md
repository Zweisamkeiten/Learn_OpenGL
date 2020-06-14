# Learn_OpenGL

使用说明

最终大作业在 根路径[./12_1Multiple_lights/Lighting_map.o](./12_1Multiple_lights/Lighting_map.o)

使用环境: Manjaro Linux

Xorg版本:1.20.8

内核版本:5.4.43-1-MANJARO

没有在windows系统中进行过测试，

首先需要安装glfw3.0以上的库，添加进系统的include头文件位置，来完成OpenGL窗口创建。之后可以直接以命令行形式

所需的管理opengl指针的glad库已包含在压缩包中

由于我对g++扫描include库添加了用户路径 工程根目录/include/，

所以如果需要其他环境再次编译，需要手动修改源代码中添加的头文件的绝对路径。

所需头文件已包含在压缩包 include文件夹下 

编译方法 g++ 源代码.cpp  /path/to/glad.c -ldl -lGL -lglfw -o 源代码.o
