#include "Window.h"

 // glad.h 必须放在glfw3.h 或者glut.h文件之前。
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <random>
#include <functional>

#include "LineItem.h"
#include "RectSelItem.h"

#include "DataDefine.h"

#include "MouseEvent.h"
#include "KeyEvent.h"
#include "WindowEvent.h"

#include "Camera.h"

#include "log.h"
#include "TreeIndex.h"

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=nullptr; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=nullptr; } }


Window::~Window()
{
    for(auto& item : m_vecItems)
        SAFE_DELETE(item);

    SAFE_DELETE(m_pCamera);

    glfwDestroyWindow(m_pWnd);
    glfwTerminate();

    closeLog();
}

bool Window::initWnd(int w, int h, std::string& strName)
{
    initLog(strName);

    m_pCamera = new Camera(glm::vec3(0.0, 0.0, 965.8));
    m_nWndW = w;
    m_nWndH = h;


    // for(int i = 0; i < 1100; i++)
    // {
        WRITE_INFOLOG("WRITE_INFOLOG initial opengl 初始化OpenGL");
        // LOG(INFO)<<i;
        WRITE_WARNLOG("WRITE_WARNLOG initial opengl 初始化OpenGL");
        // WRITE_WARNLOG(i);

        WRITE_ERRORLOG("WRITE_ERRORLOG initial opengl 初始化OpenGL");
        std::string str = "WRITE_ERRORLOG initial opengl 初始化OpenGL";
        WRITE_ERRORLOG(str.c_str());
        // WRITE_FATALLOG("WRITE_FATALLOG initial opengl 初始化OpenGL");
    // }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_pWnd = glfwCreateWindow(m_nWndW, m_nWndH, strName.c_str(), nullptr, nullptr);
    if (m_pWnd == nullptr)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return false;
    }

    // glfwSwapInterval(1);
    glfwMakeContextCurrent(m_pWnd);

    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    
    glfwSetWindowUserPointer(m_pWnd, this);  // 存储window类

    // callback functrions
    {
        glfwSetWindowSizeCallback(m_pWnd, [](GLFWwindow *window, int width, int height){
            auto wnd = (Window*)glfwGetWindowUserPointer(window);
            
            ResizeEvent e(width, height);
            wnd->resizeEvent(e); 
            } );

        glfwSetCursorPosCallback(m_pWnd, [](GLFWwindow *window, double xpos, double ypos){
            auto wnd = (Window*)glfwGetWindowUserPointer(window);
            MouseMoveEvent e(xpos, ypos);
            wnd->mouseMoveEvent(e);
            } );

        glfwSetWindowCloseCallback(m_pWnd, [](GLFWwindow *window){
            auto wnd = (Window*)glfwGetWindowUserPointer(window);
            WindowCloseEvent e;
            wnd->closeEvent(e);
            } );

        glfwSetScrollCallback(m_pWnd, [](GLFWwindow *window, double xoffset, double yoffset){
            auto wnd = (Window*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent e(xoffset, yoffset);
            wnd->mouseScroolEvent(e);
            } );

        glfwSetMouseButtonCallback(m_pWnd, [](GLFWwindow *window, int button, int action, int mods){
            auto wnd = (Window*)glfwGetWindowUserPointer(window);
            switch (action)
            {
            case GLFW_PRESS:
            {
                MousePressEvent e(button, action, mods);
                wnd->mousePressEvent(e);
            }
            break;
            case GLFW_RELEASE:
            {
                MouseReleaseEvent e(button, action, mods);
                wnd->mouseReleaseEvent(e);
            }
            break;
            default:
                break;
            } 
            });

        glfwSetKeyCallback(m_pWnd, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            auto wnd = (Window*)glfwGetWindowUserPointer(window);
            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyPressEvent e(key, scancode, action, mods);
                wnd->keyPressEvent(e);
            }
            break;
            case GLFW_RELEASE:
            {
                KeyReleaseEvent e(key, scancode, action, mods);
                wnd->keyReleaseEvent(e);
            }
            break;
            case GLFW_REPEAT:
            {
                KeyPressEvent e(key, scancode, action, mods);
                std::cout<<"Key Repeat"<<std::endl;
                wnd->keyPressEvent(e);
            }
            break;
            default:
                break;
            }
        });
    }
    return true;
}

bool Window::run()
{
    float lastFrame = 0.0f;

    // render loop
    while (!glfwWindowShouldClose(m_pWnd))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        m_dDeltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // std::cout<<m_dDeltaTime<<std::endl;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4& projection = glm::perspective(glm::radians(m_pCamera->m_dZoom), 
                    (float)m_nWndW / (float)m_nWndH, 0.1f, 9000.0f);
        glm::mat4& view = m_pCamera->GetViewMatrix();

        for (const auto &item : m_vecItems)
        { 
            // LineItem *pItem = static_cast<LineItem *>(item);
            item->matProj = projection;
            item->matView = view;
            item->matModel = glm::mat4(1.0f);

            item->render();
        }

        if(m_pSelItem)
        {
            m_pSelItem->matProj = projection;
            m_pSelItem->matView = view;
            m_pSelItem->matModel = glm::mat4(1.0f);

            m_pSelItem->render();
        }

        glfwSwapBuffers(m_pWnd);
        glfwPollEvents();
    } // while

    return true;
}

void Window::resizeEvent(ResizeEvent& e)
{
    glViewport(0, 0, e.m_nW, e.m_nH);
}

void Window::closeEvent(WindowCloseEvent& e)
{
    // glfwTerminate();
}

void Window::keyPressEvent(KeyPressEvent& e)
{
    float cameraSpeed = static_cast<float>(2.5 * m_dDeltaTime);
    if (e.m_nKey == GLFW_KEY_W)
    {
        m_pCamera->ProcessKeyboard(Camera_Movement::FORWARD, m_dDeltaTime);
    }
    else if (e.m_nKey == GLFW_KEY_S)
    {
        m_pCamera->ProcessKeyboard(Camera_Movement::BACKWARD, m_dDeltaTime);
    }
    else if (e.m_nKey == GLFW_KEY_A)
    {
        m_pCamera->ProcessKeyboard(Camera_Movement::LEFT, m_dDeltaTime);
    }
    else if (e.m_nKey == GLFW_KEY_D)
    {
        m_pCamera->ProcessKeyboard(Camera_Movement::RIGHT, m_dDeltaTime);
    }
    else if (e.m_nKey == GLFW_KEY_M)
    {
    }
    else if (e.m_nKey == GLFW_KEY_N)
    {
        m_bSel = true;

        if(!m_pTree)
            m_pTree = new TreeIndex();
        
        for(const auto& item : m_vecItems)
        {
            LineItem* pLineItem = static_cast<LineItem*>(item);
            m_pTree->add(pLineItem);
        }
    }
    else if(e.m_nKey == GLFW_KEY_ESCAPE)
    {
        if(m_bNewItem)
            m_vecItems.emplace_back(m_pNewItem);

        m_bNewItem = false;
        m_bSel = false;
    }
}

void Window::keyReleaseEvent(KeyReleaseEvent &e)
{
    if (e.m_nKey == GLFW_KEY_W)
    {
    }
    else if (e.m_nKey == GLFW_KEY_S)
    {
    }
}

void Window::mouseScroolEvent(MouseScrolledEvent& e)
{
    std::cout<<"XOffset:"<<e.m_dXOffset<<" YOffset:"<<e.m_dYOffset<<std::endl;
    m_pCamera->ProcessMouseScroll(static_cast<float>(e.m_dYOffset));
}

void Window::mousePressEvent(MousePressEvent& e)
{
    switch (e.m_nBtn)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        {
            if(!m_pNewItem)
            {
                m_bNewItem = true;
                m_pNewItem = new LineItem();
                m_vecItems.emplace_back(m_pNewItem);
            }
            m_pNewItem->addPt( screen2GLPt(m_pt) ); 
            
            std::cout<<" Left X: "<<m_pt.x<<" / "<<(m_pt.x - m_nWndW / 2)<< " \t"
                     <<" Y: "<<m_pt.y<<" / "<<( -m_pt.y + m_nWndH / 2 )<<std::endl;
        }        
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        m_ptFirst = screen2GLPt(m_pt); 
        
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        {
            m_bNewItem = false;
            m_pNewItem = nullptr;

            m_bSel = true;
            if(m_bSel)
                m_ptFirst = screen2GLPt(m_pt);

            // SAFE_DELETE(m_pNewItem);
            if(!m_pSelItem)
            {
                std::cout<<"new RectSelItem"<<std::endl;
                m_pSelItem = new RectSelItem();
            }

            m_pSelItem->addPt(m_ptFirst);
            // m_pNewItem->addPt(m_ptFirst);
            
        }
        break;
    default:
        return;
    }
}

void Window::mouseReleaseEvent(MouseReleaseEvent& e)
{
    switch (e.m_nBtn)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        std::cout << "Middle" << std::endl;
        {
            Pt pt = screen2GLPt(m_pt);
            double dXMove = m_ptFirst.x - pt.x;
            double dYMove = m_ptFirst.y - pt.y;
            m_pCamera->m_v3Position.x += (int)dXMove;
            m_pCamera->m_v3Position.y += (int)dYMove;
        }
        m_bSel = false;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        std::cout << "Right" << std::endl;

        Pt pt = screen2GLPt(m_pt);
        if (m_bSel && m_pTree && m_pTree->selTest(m_ptFirst, pt))
        {
            std::default_random_engine e((unsigned int)time(0));
            for (const auto &item : m_vecItems)
            {
                std::uniform_real_distribution<double> u(0, 1);

                LineItem *pLineItem = static_cast<LineItem *>(item);
                glm::vec4 vColor = glm::vec4(u(e), u(e), u(e), 1.0);
                pLineItem->setColor(vColor);
                std::cout << vColor.x << "t" << vColor.y << "\t" << vColor.z << std::endl;
                pLineItem->render();
            }
            // m_bSel = false;
        }
        m_pSelItem->clear();
        m_bSel = false;
    }
    break;
    default:
        std::cout << "Default:" << std::endl;
        break;
    }
}

void Window::mouseMoveEvent(MouseMoveEvent& e)
{
    m_pt.x = e.m_dX;
    m_pt.y = e.m_dY;
    m_pt.z = 0.0;
    
    if(m_bSel)
    {
        std::cout<<"render RectSelItem"<<std::endl;
        m_pSelItem->addPt(screen2GLPt(m_pt));
        // m_pNewItem->render();
    }
}


Pt Window::screen2GLPt(Pt& ptS)
{
    double dRadio = m_pCamera->m_v3Position.z / m_pCamera->m_v3PositionOri.z;
    double dV = m_pCamera->m_v3Position.x - m_pCamera->m_v3PositionOri.x;
    double dH = m_pCamera->m_v3Position.y - m_pCamera->m_v3PositionOri.y;
    return Pt(ptS.x * dRadio - m_nWndW* dRadio  / 2 + dV, 
                    -1 * ptS.y * dRadio + m_nWndH* dRadio / 2 - dH);
}
