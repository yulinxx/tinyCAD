#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include <functional>
#include <vector>

#include "DataDefine.h"
#include "Event.h"

#include "Shader.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <mutex>

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

class Camera;
class TreeIndex;

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

    Pt screen2GLPt(Pt& ptS); // 屏幕坐标转OpenGL坐标

private:
    double m_dWndW = 800;
    double m_dWndH = 600;

    GLFWwindow* m_pWnd = nullptr;

   std::vector<Item*> m_vecItems; 
   bool m_bNewItem = false;

    Item* m_pNewItem = nullptr;
    Item* m_pSelItem = nullptr;

    Item* m_pRuler = nullptr;  // 标尺

    std::vector<Item*> m_vecSelItems; // 选中的图元

    Pt      m_pt;
    Pt      m_ptFirst; // 记录鼠标的第一点(GL坐标点)

    float m_dDeltaTime = 0.0f;

    Camera* m_pCamera = nullptr;

    TreeIndex* m_pTree = nullptr;

    bool m_bSel = false;


    std::mutex m_mutex;
};

#endif //_WINDOW_H_