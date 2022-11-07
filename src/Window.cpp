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

#include "PointItem.h"
#include "LineItem.h"
#include "RectSelItem.h"
#include "RulerItem.h"
#include "ImgItem.h"

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

#define PI  3.1415926 

Window::~Window()
{
    for(auto& item : m_vecItems)
        SAFE_DELETE(item);

    SAFE_DELETE(m_pSelItem);
    SAFE_DELETE(m_pRuler);

    SAFE_DELETE(m_pCamera);

    glfwDestroyWindow(m_pWnd);
    glfwTerminate();

    closeLog();
}

bool Window::initWnd(int w, int h, std::string& strName)
{
    initLog(strName);

    // 相机距离为: h/2 / tan(45/2) = 965.6854249 正好覆盖整个 1200 * 800 窗口
    m_pCamera = new Camera(glm::vec3(0.0, 0.0, 965.6954249));
    m_dWndW = w;
    m_dWndH = h;


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

    m_pWnd = glfwCreateWindow((int)m_dWndW, (int)m_dWndH, strName.c_str(), nullptr, nullptr);
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

// Loop Render
bool Window::run()
{
    // 边框測試
    LineItem* pTempRectItem = new LineItem();
    if(1)
    {
     m_vecItems.emplace_back(pTempRectItem);
     pTempRectItem->addPt(std::move(Pt(-600, -400)));
     pTempRectItem->addPt(Pt(-600, 400));
     pTempRectItem->addPt(Pt(600, 400));
     pTempRectItem->addPt(Pt(600, -400));
     // pLineItem->addPt(Pt(-600, -400));

     std::default_random_engine e((unsigned int)time(0));
     std::uniform_int_distribution<int> uW(1, 6);
     std::uniform_int_distribution<int> u(1, 0xffff);
     std::uniform_real_distribution<double> uA(0, 20);

     int nW = uW(e);
     pTempRectItem->setLineWidth(nW);

     unsigned int nP = u(e);
     pTempRectItem->setPattern(nP);

     double dF = uA(e);
    //  pTempRectItem->setFactor(dF);
     pTempRectItem->setFactor(1.0);
     pTempRectItem->setResolution(Pt(m_dWndW, m_dWndH));
     pTempRectItem->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

     std::cout << "--- line W: " << nW << " Pattern" << nP << " Factor" << dF << std::endl;
    }

    double lastFrame = 0.0f;

   auto setRender = [](Item* pItem, glm::mat4& matProj, glm::mat4 matView, glm::mat4 matModel){
        if(!pItem)
            return;

        pItem->m_matProj = matProj;
        pItem->m_matView = matView;
        pItem->m_matModel = matModel;

        pItem->render();
    };

    // 标尺
    if(!m_pRuler)
        m_pRuler = new RulerItem();

    // // 混合,带透明色
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST); // 深圳测试

    // render loop
    unsigned int nP = 0x000f;
    int i = 0;

    while (!glfwWindowShouldClose(m_pWnd))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        m_dDeltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // std::cout<<m_dDeltaTime<<std::endl;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(m_pCamera->m_dZoom), 
                    float(m_dWndW / m_dWndH), 0.1f, 9000.0f);

        glm::mat4 view = m_pCamera->GetViewMatrix();

        {
            std::lock_guard<std::mutex> guard(m_mutex);

            for (const auto &item : m_vecItems)
               setRender(item, projection, view, glm::mat4(1.0f));
        }

        // if(i++ > 60)
        // {
        //     pTempRectItem->setPattern(nP++);
        //     i = 0;
        // }

        // if(nP > 0xefff)
        //     nP = 0x000f;

        setRender(pTempRectItem, projection, view, glm::mat4(1.0f));
        setRender(m_pNewItem, projection, view, glm::mat4(1.0f));
        setRender(m_pSelItem, projection, view, glm::mat4(1.0f));
        setRender(m_pRuler, projection, view, glm::mat4(1.0f));

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
        m_pCamera->ProcessKeyboard(Camera_Movement::FORWARD, (float)m_dDeltaTime);
    }
    else if (e.m_nKey == GLFW_KEY_S)
    {
        m_pCamera->ProcessKeyboard(Camera_Movement::BACKWARD, (float)m_dDeltaTime);
    }
    else if (e.m_nKey == GLFW_KEY_A)
    {
        m_pCamera->ProcessKeyboard(Camera_Movement::LEFT, (float)m_dDeltaTime);
    }
    else if (e.m_nKey == GLFW_KEY_D)
    {
        m_pCamera->ProcessKeyboard(Camera_Movement::RIGHT, (float)m_dDeltaTime);
    }
    else if (e.m_nKey == GLFW_KEY_M)
    {
        auto ptIns = m_ptCur;
        std::cout << "Insert Image at: x" << ptIns.x << " y:" << ptIns.y << std::endl;
        ImgItem* pNewItem = new ImgItem();
        pNewItem->addPt(m_ptCur);
        m_vecItems.emplace_back(pNewItem);
    }
    else if (e.m_nKey == GLFW_KEY_N)
    {

    }
    else if(e.m_nKey == GLFW_KEY_DELETE)
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        // 从树中删除
        m_pTree->eraseItem(m_vecSelItems);

        for(int i = (int)m_vecSelItems.size() - 1; i >= 0; i--)
        {
            auto item = m_vecSelItems[i];

            auto pos = find(m_vecItems.begin(),m_vecItems.end(), item);
            if (pos != m_vecItems.end()) 
            {
                m_vecItems.erase(pos);
            } 

            SAFE_DELETE(m_vecSelItems[i]);
        }
        m_vecSelItems.clear();
    }
    else if(e.m_nKey == GLFW_KEY_ESCAPE)
    {
        if(m_pNewItem) // todo:如果线段只有一个点,则不添加 
        {
            if(!m_pTree)
                m_pTree = new TreeIndex();

            LineItem* pLineItem = static_cast<LineItem*>(m_pNewItem); 
            m_pTree->add(pLineItem);
            m_vecItems.emplace_back(m_pNewItem);
            m_pNewItem = nullptr;
        }

        // m_bSel = false;
        m_bitFlags.reset(0);
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
    // std::cout<<"XOffset:"<<e.m_dXOffset<<" YOffset:"<<e.m_dYOffset<<std::endl;
    m_pCamera->ProcessMouseScroll(static_cast<float>(e.m_dYOffset));

    double dH = m_pCamera->m_v3Position.z *  tan(m_pCamera->m_dZoom / 2 * PI / 180);
    double dW = 3.0 / 2.0 * dH;
    RulerItem* rulerItem = static_cast<RulerItem*>(m_pRuler);
    rulerItem->initPt(dW * 2 - 10, dH * 2 - 10);
}

void Window::mousePressEvent(MousePressEvent& e)
{
    auto addNewItem = [&](){
        if(m_pNewItem) // todo:如果线段只有一个点,则不添加 
        {
            if(!m_pTree)
                m_pTree = new TreeIndex();

            LineItem* pLineItem = static_cast<LineItem*>(m_pNewItem);
            m_vecItems.emplace_back(m_pNewItem);
            m_pTree->add(pLineItem);
            
            m_pNewItem = nullptr;
        }
    };

    switch (e.m_nBtn)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        {
            if(!m_pNewItem)
            {
                std::cout << "New LineItem" << std::endl;
                m_pNewItem = new LineItem();
    
                LineItem *pLineItem = static_cast<LineItem *>(m_pNewItem);
                std::default_random_engine e((unsigned int)time(0));
                std::uniform_int_distribution<int> uW(1, 10);
                std::uniform_int_distribution<int> u(1, 0x0fff);
                std::uniform_real_distribution<double> uA(0, 20);

                int nW = uW(e);
                pLineItem->setLineWidth(nW);

                unsigned int nP = u(e);
                pLineItem->setPattern(nP);

                double dF = uA(e);
                pLineItem->setFactor((float)dF);
                pLineItem->setResolution(Pt(m_dWndW, m_dWndH));
                pLineItem->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

                std::cout << "--- line W: " << nW << " Pattern" << nP << " Factor" << dF << std::endl;
            }
            m_pNewItem->addPt( m_ptCur );

            std::cout << " Left X: " << m_ptCur.x << " / " << (m_ptCur.x - m_dWndW / 2) << " \t"
                      << " Y: " << m_ptCur.y << " / " << (-m_ptCur.y + m_dWndH / 2) << std::endl;
        }        
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        m_ptFirst = m_ptCur; 
        m_ptFirstX = m_ptCurX; 
        m_bitFlags.set(1);
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        {
            addNewItem();

            m_bitFlags.set(0);
            m_ptFirst = m_ptCur;

            if(!m_pSelItem)
            {
                std::cout<<"new RectSelItem"<<std::endl;
                m_pSelItem = new RectSelItem();
            }
            m_pSelItem->addPt(m_ptFirst);
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
        m_bitFlags.reset(0);
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        // std::cout << "Middle" << std::endl;
        // if(m_bitFlags.test(1))
        // {
            m_bitFlags.reset(1);
        //     Pt pt = screen2GLPt(m_ptCur);
        //     double dXMove = m_ptFirst.x - pt.x;
        //     double dYMove = m_ptFirst.y - pt.y;
        //     if(fabs(dXMove) < 20 && fabs(dYMove) < 20)
        //     {
        //         PointItem* ptItem = new PointItem(pt);

        //         std::default_random_engine e((unsigned int)time(0));
        //         std::uniform_real_distribution<double >uA(1,10);
        //         std::uniform_real_distribution<double >uB(0,1);

        //         double dSz = uA(e);
        //         ptItem->setPtSize((float)dSz);
        //         std::cout<<" Pt Size:"<< dSz << std::endl;
        //         ptItem->setColor(glm::vec4(uB(e), uB(e), uB(e), 1.0));

        //         m_vecItems.emplace_back(ptItem);

        //         if(!m_pTree)
        //             m_pTree = new TreeIndex();
        //         m_pTree->add(ptItem);
        //     }
        //     else
        //     {
        //         m_pCamera->m_v3Position.x += (int)dXMove;
        //         m_pCamera->m_v3Position.y += (int)dYMove;
        //     }
        // }
        // m_bitFlags.reset(0);
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        m_pSelItem->clear();
        m_bitFlags.reset(0);
    }
    break;
    default:
        std::cout << "Default:" << std::endl;
        break;
    }
}

void Window::mouseMoveEvent(MouseMoveEvent& e)
{
    m_ptCur = screen2GLPt(Pt{e.m_dX, e.m_dY, 0.0});
    m_ptCurX = Pt{e.m_dX, e.m_dY, 0.0};
    
    if(m_bitFlags.test(0))
    {
        m_pSelItem->addPt(m_ptCur);

        if (m_pTree)
        {
           std::cout<<"Select "<<m_vecSelItems.size()<<" Items"<<std::endl;
            m_vecSelItems = m_pTree->selTest(m_ptFirst, m_ptCur);
            std::default_random_engine e((unsigned int)time(0));
            for (const auto &item : m_vecSelItems)
            {
                std::uniform_real_distribution<double> u(0, 1);

                LineItem *pLineItem = static_cast<LineItem *>(item);
                glm::vec4 vColor = glm::vec4(u(e), u(e), u(e), 1.0);
                pLineItem->setColor(vColor);
                pLineItem->render();
            }
        }
    }

    if(m_bitFlags.test(1))
    {
        // 相机 导致移动坐标有问题
        // double dXMove = m_ptCur.x - m_ptPrev.x;
        // double dYMove = m_ptCur.y - m_ptPrev.y;
        // std::cout << "\n----Prev X:" << m_ptPrev.x << " Y:" << m_ptPrev.y << std::endl;
        // std::cout << "Cur X:" << m_ptCur.x << " Y:" << m_ptCur.y << std::endl;
        // std::cout << "Move X:" << dXMove << " Y:" << dYMove << std::endl;
        // m_pCamera->m_v3Position.x += (int)dXMove;
        // m_pCamera->m_v3Position.y += (int)dYMove;
        // m_ptPrev = m_ptCur;
        double dXMove = m_ptCurX.x - m_ptFirstX.x;
        double dYMove = m_ptCurX.y - m_ptFirstX.y;
        std::cout << "\n----Prev X:" << m_ptPrev.x << " Y:" << m_ptPrev.y << std::endl;
        std::cout << "Cur X:" << m_ptCur.x << " Y:" << m_ptCur.y << std::endl;
        std::cout << "Move X:" << dXMove << " Y:" << dYMove << std::endl;
        m_pCamera->m_v3Position.x += int(dXMove * 0.1);
        m_pCamera->m_v3Position.y += int(dYMove  * -0.1);
    }
}


Pt Window::screen2GLPt(Pt& ptS)
{
    double dRadio = m_pCamera->m_v3Position.z / m_pCamera->m_v3PositionOri.z;
    double dV = m_pCamera->m_v3Position.x - m_pCamera->m_v3PositionOri.x;
    double dH = m_pCamera->m_v3Position.y - m_pCamera->m_v3PositionOri.y;
    return Pt(ptS.x * dRadio - m_dWndW* dRadio  / 2 + dV, 
                    -1 * ptS.y * dRadio + m_dWndH* dRadio / 2 - dH);
}
