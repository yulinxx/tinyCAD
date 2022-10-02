#include "Window.h"

#include <glad/glad.h> // glad.h 必须放在glfw3.h 或者glut.h文件之前。
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>
#include <functional>

#include "shader.h"
#include "DataDefine.h"

#include "MouseEvent.h"
#include "KeyEvent.h"
#include "WindowEvent.h"


Window::~Window()
{
    glfwDestroyWindow(m_pWnd);
    glfwTerminate();
}

bool Window::initWnd(int w, int h, std::string& strName)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_pWnd = glfwCreateWindow(w, h, strName.c_str(), nullptr, nullptr);
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
    
    m_wndInfo = WndInfo(w, h, strName);
    // glfwSetWindowUserPointer(m_pWnd, &m_wndInfo);  // 暂时存储窗口数据
    glfwSetWindowUserPointer(m_pWnd, this);  // 暂时存储窗口数据

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
    Shader lineShader("line.vs", "line.fs");
    unsigned int lineShaderID = lineShader.ID;

    std::default_random_engine e(unsigned int(time(0)));
    std::uniform_real_distribution<double >u(-1,1);

    Lines lineData; 
    for (size_t i = 0; i < 20; i++)
        lineData.emplace_back(Pt(u(e), u(e), 0.4));

    unsigned int lineVBO, lineVAO;
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);

    glBindVertexArray(lineVAO);  
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, lineData.size() * sizeof(Pt), &lineData[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Pt), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    // --------------------
    Shader solidShader("solids.vs", "solids.fs");
    unsigned int solidShaderID = solidShader.ID;
    float vertices[] = {
        -0.5f, -0.5f, 0.5f, // left  
         0.5f, -0.5f, 0.5f, // right 
         0.0f,  0.5f, 0.5f  // top   
        }; 

    unsigned int solidVBO, solidVAO;
    glGenVertexArrays(1, &solidVAO);
    glGenBuffers(1, &solidVBO);

    glBindVertexArray(solidVAO);  
    glBindBuffer(GL_ARRAY_BUFFER, solidVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // render loop
    while (!glfwWindowShouldClose(m_pWnd))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // std::cout<<deltaTime<<std::endl;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lineShader.use();
        glBindVertexArray(lineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, GLsizei(lineData.size()));

        solidShader.use();
        glBindVertexArray(solidVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(m_pWnd);
        glfwPollEvents();
    }
   
    glDeleteVertexArrays(1, &solidVAO);
    glDeleteBuffers(1, &solidVBO);
    glDeleteProgram(lineShaderID);
    glDeleteProgram(solidShaderID);


    return true;
}


void Window::resizeEvent(ResizeEvent& e)
{

}

void Window::closeEvent(WindowCloseEvent& e)
{

}

void Window::keyPressEvent(KeyPressEvent& e)
{
    if(e.m_nKey == GLFW_KEY_W)
    {

    }
    else if (e.m_nKey == GLFW_KEY_S)
    {
    
    }
}

void Window::keyReleaseEvent(KeyReleaseEvent& e)
{
     if(e.m_nKey == GLFW_KEY_W)
    {
        std::cout<<"Key W"<<std::endl;
    }
    else if (e.m_nKey == GLFW_KEY_S)
    {
        std::cout<<"Key S"<<std::endl;
    }
    std::cout<<"Key:"<<e.m_nKey<<" Act:"<<e.m_nAction<<" Mods:"<<e.m_nMods<<" Scancode:"<<e.m_nScancode<<std::endl;
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

    std::cout<<"Mouse Press Btn"<<e.m_nBtn<<" Act:"<<e.m_nAct<<" Mods:"<<e.m_nMods<<std::endl;
}

void Window::mouseReleaseEvent(MouseReleaseEvent& e)
{
    switch (e.m_nBtn)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        break;
    default:
        printf("Default \n");
        return;
    }
    std::cout<<"Mouse Release Btn"<<e.m_nBtn<<" Act:"<<e.m_nAct<<" Mods:"<<e.m_nMods<<std::endl;
}

void Window::mouseMoveEvent(MouseMoveEvent& e)
{
    if((int)e.m_dX % 100 == 0)
        std::cout<<"MouseMove:  X:"<<e.m_dX<<" Y:"<<e.m_dY<<std::endl;
}