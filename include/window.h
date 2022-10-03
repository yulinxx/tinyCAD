#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include <functional>

#include "DataDefine.h"
#include "Event.h"

#include "Shader.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
    int m_nWndW = 800;
    int m_nWndH = 600;

    GLFWwindow* m_pWnd = nullptr;

   std::vector<Item*> m_vecItems; 
   bool m_bNewItem = false;

    Item* m_pNewItem = nullptr;

    Pt      m_pt;


    float deltaTime = 0.0f;
    
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
};

#endif //_WINDOW_H_