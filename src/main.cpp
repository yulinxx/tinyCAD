#include <glad/glad.h> // glad.h 必须放在glfw3.h 或者glut.h文件之前。
#include <GLFW/glfw3.h>

#include "window.h"

/////////////////////////////////////////////////////////////
int main()
{
    window wnd;
    const int w = 1200;
    const int h = 800;
    std::string wndName = "tinyCAD";
    if(!wnd.initWnd(w, h, wndName))
        return -1;
    
    wnd.run();
    
    return 0;
}

