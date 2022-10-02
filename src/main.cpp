#include <glad/glad.h> // glad.h 必须放在glfw3.h 或者glut.h文件之前。
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>
#include <functional>

#include "shader.h"
#include "DataDefine.h"

#include "window.h"

/////////////////////////////////////////////////////////////
int main()
{
    window w;
    const int w = 1200;
    const int h = 800;
    std::string wndName = "tinyCAD";
    if(!w.initWnd(1200, 800, wndName))
        return -1;
    
    w.run();
    
    return 0;
}

