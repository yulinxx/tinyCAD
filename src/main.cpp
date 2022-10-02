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

