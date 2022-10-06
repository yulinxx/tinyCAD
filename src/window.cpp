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
#include "DataDefine.h"

#include "MouseEvent.h"
#include "KeyEvent.h"
#include "WindowEvent.h"
#include "LineItem.h"

#include "Camera.h"

#include "log.h"

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

    google::ShutdownGoogleLogging();
}

bool Window::initWnd(int w, int h, std::string& strName)
{
    // log
    FLAGS_log_dir = "./"; // 日志文件保存目录，这个目录必须是已经存在的
    FLAGS_max_log_size = 10; //设置最大日志文件大小（以MB为单位）
    FLAGS_colorlogtostderr = true;  // Set log color
    FLAGS_stop_logging_if_full_disk = true;  // If disk is full
	google::SetLogFilenameExtension(".log");
    
	google::InitGoogleLogging(strName.c_str());
	WRITE_INFOLOG("initial opengl 初始化OpenGL");

	google::FlushLogFiles(google::GLOG_INFO);

    // m_pCamera = new Camera(glm::vec3(0.0, 0.0, 81.0));
    m_pCamera = new Camera(glm::vec3(0.0, 0.0, 965.8));
    m_nWndW = w;
    m_nWndH = h;

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
    // timing
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
            LineItem *pItem = static_cast<LineItem *>(item);
            pItem->matProj = projection;
            pItem->matView = view;
            pItem->matModel = glm::mat4(1.0f);

            pItem->render();
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
    // float cameraSpeed = 0.05f; 
    float cameraSpeed = static_cast<float>(2.5 * m_dDeltaTime);
    if (e.m_nKey == GLFW_KEY_W)
    {
        std::cout << "Key W" << std::endl;
        // cameraPos += cameraSpeed * cameraFront;
        m_pCamera->ProcessKeyboard(Camera_Movement::FORWARD, m_dDeltaTime);
    }
    else if (e.m_nKey == GLFW_KEY_S)
    {
        std::cout << "Key S" << std::endl;
        // cameraPos -= cameraSpeed * cameraFront;
        m_pCamera->ProcessKeyboard(Camera_Movement::BACKWARD, m_dDeltaTime);
    }
    else if (e.m_nKey == GLFW_KEY_N)
    {
        std::cout << "Key N" << std::endl;
    }
    else if (e.m_nKey == GLFW_KEY_A)
    {
        std::cout << "Key A" << std::endl;
        // cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        m_pCamera->ProcessKeyboard(Camera_Movement::LEFT, m_dDeltaTime);
    }
    else if (e.m_nKey == GLFW_KEY_D)
    {
        std::cout << "Key D" << std::endl;
        // cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        m_pCamera->ProcessKeyboard(Camera_Movement::RIGHT, m_dDeltaTime);
    }
    else if(e.m_nKey == GLFW_KEY_ESCAPE)
    {
        if(m_bNewItem)
            m_vecItems.emplace_back(m_pNewItem);

        m_bNewItem = false;
    }
    std::cout << "Key Press:" << e.m_nKey << " Act:" << e.m_nAction << " Mods:" << e.m_nMods << " Scancode:" << e.m_nScancode << std::endl;
}

void Window::keyReleaseEvent(KeyReleaseEvent &e)
{
    if (e.m_nKey == GLFW_KEY_W)
    {
        std::cout << "Key W" << std::endl;
    }
    else if (e.m_nKey == GLFW_KEY_S)
    {
        std::cout << "Key S" << std::endl;
    }
    std::cout << "Key Release:" << e.m_nKey << " Act:" << e.m_nAction << " Mods:" << e.m_nMods << " Scancode:" << e.m_nScancode << std::endl;
}

void Window::mouseScroolEvent(MouseScrolledEvent& e)
{
    std::cout<<"XOffset:"<<e.m_dXOffset<<" YOffset:"<<e.m_dYOffset<<std::endl;
}

void Window::mousePressEvent(MousePressEvent& e)
{
    switch (e.m_nBtn)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        std::cout<<"Left"<<std::endl;
        {
            if(!m_pNewItem)
            {
                m_bNewItem = true;
                m_pNewItem = new LineItem();
                m_vecItems.emplace_back(m_pNewItem);
            }
 
            double dRadio = m_pCamera->m_v3Position.z / m_pCamera->m_v3PositionOri.z;
            double dV = m_pCamera->m_v3Position.x - m_pCamera->m_v3PositionOri.x;
            double dH = m_pCamera->m_v3Position.y - m_pCamera->m_v3PositionOri.y;

            m_pNewItem->addPt( Pt(m_pt.x * dRadio - m_nWndW* dRadio  / 2 + dV, 
                            -1 * m_pt.y * dRadio + m_nWndH* dRadio / 2 - dH) ); 
            
            std::cout<<" X: "<<m_pt.x<<" / "<<(m_pt.x - m_nWndW / 2)<<std::endl
                     <<" Y: "<<m_pt.y<<" / "<<( -m_pt.y + m_nWndH / 2 )<<std::endl;
        }        
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        m_pCamera->m_v3Position = glm::vec3(glm::vec3(0.0, 0.0, 1931.6));
        std::cout<<"Middle"<<" X:"<<(m_pt.x * 2 / 1200 - 1)<<" Y:"<<( m_pt.y * 2 / 800 - 1);
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        m_bNewItem = false;
        
        m_pNewItem = nullptr;

        std::cout<<"Right"<<std::endl;
        break;
    default:
        std::cout<<"Default:"<<std::endl;
        return;
    }

    std::cout<<"Mouse Press Btn"<<e.m_nBtn<<" Act:"<<e.m_nAct<<" Mods:"<<e.m_nMods<<std::endl;
}

void Window::mouseReleaseEvent(MouseReleaseEvent& e)
{
    switch (e.m_nBtn)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        std::cout<<"Left"<<std::endl;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        std::cout<<"Middle"<<std::endl;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        std::cout<<"Right"<<std::endl;
        break;
    default:
        std::cout<<"Default:"<<std::endl;
        return;
    }
    std::cout<<"Mouse Release Btn"<<e.m_nBtn<<" Act:"<<e.m_nAct<<" Mods:"<<e.m_nMods<<std::endl;
}

void Window::mouseMoveEvent(MouseMoveEvent& e)
{
    if((int)e.m_dX % 100 == 0)
        std::cout<<"MouseMove:  X:"<<e.m_dX<<" Y:"<<e.m_dY<<std::endl;

    m_pt.x = e.m_dX;
    m_pt.y = e.m_dY;
    m_pt.z = 0.0;
}
