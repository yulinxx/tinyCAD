#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>

#include <functional>

#include "Event.h"

using EventFunc = std::function<void(Event&)>;

class GLFWwindow;


// 集中数据的同时方便后面lambda表达式的书写
// typedef struct _WindowInfo
// {
//     std::string Name;
//     int Width;
//     int Height;

//     // EventFunc EventCallback;

//     _WindowInfo(int width, int height, std::string name) 
//     {
//         Width = width;
//         Height = height;
//         Name = name;
//     }
// }WndInfo;


struct WndInfo
{
    int W = 800;
    int H = 600;
    std::string Name = "tinyCAD";

    EventFunc EventCallback;

    WndInfo(){}

    WndInfo(int w, int h, std::string name)
    : W(w), H(h), Name(name){}
};

//////////////////////////////////////////////////////

class window
{
public:
    window() = default;
    ~window() = default;

public:
    bool initWnd(int w, int h, std::string& strName);

    GLFWwindow* getWnd(){return m_pWnd;}

    bool run();
    
private:
    GLFWwindow* m_pWnd = nullptr;

    WndInfo info;
    
};

#endif //_WINDOW_H_