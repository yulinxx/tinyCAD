# tinyCAD
only for  opengl learn &amp; exercise

# tinyCAD
完全基于OpenGL进行绘制简单的图形,用于OpenGL练习  
以GLFW为基础,将鼠标/键盘等消息分别封装至Window类中  

### depends 环境依赖:
- Windows
  以vcpkg为 C++的包管理器。vcpkg使用可自行获取
  vcpkg中需安装:
  - glfw3 3.3.8 : GLFW is a free, Open Source, multi-platform libr...
    32位: `vcpkg install glfw3`
    64位: `vcpkg install glfw3:x64-windows`
    以下安装包均不指明位数

  - glew 2.2.0  : The OpenGL Extension Wrangler Library (GLEW)
    `vcpkg install glew`

  - glog 0.6.0  : C++ implementation of the Google logging module
    `vcpkg install glog`
    若不需要日志功能,可以CMakeLists.txt以及代码中移除

- Linux