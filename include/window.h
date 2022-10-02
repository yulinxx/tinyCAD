#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include <functional>

#include "Event.h"

class GLFWwindow;

// using EventFunc = std::function< void(Event&) >;
// std::function< void(Event&) > EventFunc = f;

struct WndInfo
{
    int W = 800;
    int H = 600;
    std::string Name = "tinyCAD";

    // EventFunc EventCallback;
    Event* e;

    WndInfo(){}

    WndInfo(int w, int h, std::string name)
    : W(w), H(h), Name(name){}
};

//////////////////////////////////////////////////////

class window
{
public:
    window() = default;
    ~window();

public:
    bool initWnd(int w, int h, std::string& strName);

    GLFWwindow* getWnd(){return m_pWnd;}

    bool run();
    
private:
    GLFWwindow* m_pWnd = nullptr;

    WndInfo m_wndInfo;
    
};

#endif //_WINDOW_H_