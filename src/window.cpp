#include "window.h"

#include <glad/glad.h> // glad.h 必须放在glfw3.h 或者glut.h文件之前。
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>
#include <functional>

#include "shader.h"
#include "DataDefine.h"


bool window::initWnd(int w, int h, std::string& strName)
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
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // glfwSwapInterval(1);
    glfwMakeContextCurrent(m_pWnd);

    {
        glfwSetWindowSizeCallback(m_pWnd, [](GLFWwindow* window, int width, int height)
			{
				// auto info = (WindowInfo*)glfwGetWindowUserPointer(window);
				// info->Width = width;
				// info->Height = height;
				
				// WindowResizeEvent windowResizeEvent(info->Width, info->Height);
				// info->EventCallback(windowResizeEvent);
			});

		glfwSetCursorPosCallback(m_pWnd, [](GLFWwindow* window, double xpos, double ypos)
			{
				// auto info = (WindowInfo*)glfwGetWindowUserPointer(window);
				// MouseMoveEvent mouseMoveEvent(xpos, ypos);
				// info->EventCallback(mouseMoveEvent);
			});

		glfwSetWindowCloseCallback(m_pWnd, [](GLFWwindow* window)
			{
				// auto info = (WindowInfo*)glfwGetWindowUserPointer(window);
				// WindowCloseEvent windowCloseEvent;
				// info->EventCallback(windowCloseEvent); 
			});

		glfwSetScrollCallback(m_pWnd, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				// auto info = (WindowInfo*)glfwGetWindowUserPointer(window);
				// MouseScrolledEvent mouseScrolledEvent(xoffset, yoffset);
				// info->EventCallback(mouseScrolledEvent);
			});

		glfwSetMouseButtonCallback(m_pWnd, [](GLFWwindow* window, int button, int action, int mods)
			{
				// auto info = (WindowInfo*)glfwGetWindowUserPointer(window);
				// switch (action)
				// {
				// case GLFW_PRESS:
				// {
				// 	MousePressEvent mousePressEvent(button, action);
				// 	info->EventCallback(mousePressEvent);
				// }break;
				// case GLFW_RELEASE:
				// {
				// 	MouseReleaseEvent mouseReleaseEvent(button, action);
				// 	info->EventCallback(mouseReleaseEvent);
				// }break;
				// default:
				// 	break;
				// }
			});

		glfwSetKeyCallback(m_pWnd, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				// auto info = (WindowInfo*)glfwGetWindowUserPointer(window);
				// switch (action)
				// {
				// case GLFW_PRESS:
				// {
				// 	KeyboardPressEvent keyPressEvent(key, scancode, action);
				// 	info->EventCallback(keyPressEvent);
				// }break;
				// case GLFW_RELEASE:
				// {
				// 	KeyboardReleaseEvent keyReleaseEvent(key, scancode, action);
				// 	info->EventCallback(keyReleaseEvent);
				// }break;
				// case GLFW_REPEAT:
				// {
				// 	KeyboardPressEvent keyPressEvent(key, scancode, action);
				// 	info->EventCallback(keyPressEvent);
				// }break;
				// default:
				// 	break;
				// }
			});
    }

    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    
    return true;
}


bool window::run()
{
    Shader lineShader("line.vs", "line.fs");
    unsigned int lineShaderID = lineShader.ID;

    std::default_random_engine e;
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
        glDrawArrays(GL_LINE_STRIP, 0, lineData.size());

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

    glfwTerminate();

    return true;
}