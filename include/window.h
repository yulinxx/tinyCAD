#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include <functional>

#include "DataDefine.h"
#include "Event.h"

#include "Shader.h"

class GLFWwindow;
class Item;

class ResizeEvent;
class WindowCloseEvent;

class KeyPressEvent;
class KeyReleaseEvent;
class MouseScrolledEvent;
class MousePressEvent;
class MouseReleaseEvent;
class MouseMoveEvent;

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

class Window
{
public:
    Window() = default;
    ~Window();

public:
    bool initWnd(int w, int h, std::string& strName);

    GLFWwindow* getWnd(){return m_pWnd;}

    bool run();
    
private:
    void resizeEvent(ResizeEvent& e);
    void closeEvent(WindowCloseEvent& e);

    void keyPressEvent(KeyPressEvent& e);
    void keyReleaseEvent(KeyReleaseEvent& e);
    void mouseScroolEvent(MouseScrolledEvent& e);
    void mousePressEvent(MousePressEvent& e);
    void mouseReleaseEvent(MouseReleaseEvent& e);
    void mouseMoveEvent(MouseMoveEvent& e);

private:
    void createLineGL();

private:
    GLFWwindow* m_pWnd = nullptr;

    WndInfo m_wndInfo;
    

    Item* m_pNewItem = nullptr;
    Lines m_linesPt;
    Pt      m_pt;
    unsigned int m_nItemVAO;
    Shader* m_pItemShader = nullptr;
    unsigned int m_nItemShaderID = 0;
};

#endif //_WINDOW_H_