#include <glad/glad.h> // glad.h 必须放在glfw3.h 或者glut.h文件之前。
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.h"


// 回调函数
void keyFun(GLFWwindow *pWnd, int nKey, int nScanCode, int nAction, int nMode);
void mouseFunc(GLFWwindow *pWnd, int, int, int);
void cursorFunc(GLFWwindow *pWnd, double x, double y);
void scrollFunc(GLFWwindow *pWnd, double x, double y);

// settings
const unsigned int WND_WIDTH = 1200;
const unsigned int WND_HEIGHT = 800;

/////////////////////////////////////////////////////////////
int main()
{
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // --------------------
    GLFWwindow* pWnd = glfwCreateWindow(WND_WIDTH, WND_HEIGHT, "tinyCAD", NULL, NULL);
    if (pWnd == NULL)
    {
        std::cout << "Failed to create GLFW pWnd" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(pWnd);
    glfwSetKeyCallback(pWnd, keyFun);
    glfwSetCursorPosCallback(pWnd, cursorFunc);
    glfwSetMouseButtonCallback(pWnd, mouseFunc);
    glfwSetScrollCallback(pWnd, scrollFunc);

    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ------------------------------------
    Shader lineShader("line.vs", "line.fs");
    unsigned int shaderProgram = lineShader.ID;

    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
        }; 

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO); // 生成一个VAO对象
    glGenBuffers(1, &VBO);  // 建VBO的第一步需要开辟（声明/获得）显存空间并分配VBO的ID,通过这个ID可以对特定的VBO内的数据进行存取操作。

    glBindVertexArray(VAO);  
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // render loop
    while (!glfwWindowShouldClose(pWnd))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // std::cout<<deltaTime<<std::endl;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        lineShader.use();
        glBindVertexArray(VAO);
    
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(pWnd);
        glfwPollEvents();
    }

    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

/////////////////////////////////////////////////////////////

void keyFun(GLFWwindow *pWnd, int nKey, int nScanCode, int nAction, int nMode)
{
    if (nAction == GLFW_PRESS)
    {
        std::cout<<"Key:"<<nKey<<std::endl;
    //     if (nKey == GLFW_KEY_W)
    //     {
    //         glm::vec3 direction = glm::normalize(cameraTarg - cameraPos);
    //         if (nMode == GLFW_MOD_SHIFT)
    //             direction *= 0.1;
    //         else if (nMode == GLFW_MOD_CONTROL)
    //             direction *= 0.001;
    //         else
    //             direction *= 0.01;
    //         cameraPos += direction;
    //     }
    //     else if (nKey == GLFW_KEY_S)
    //     {
    //         glm::vec3 direction = glm::normalize(cameraTarg - cameraPos);
    //         if (nMode == GLFW_MOD_SHIFT)
    //             direction *= 0.1;
    //         else if (nMode == GLFW_MOD_CONTROL)
    //             direction *= 0.001;
    //         else
    //             direction *= 0.01;
    //         cameraPos -= direction;
    //     }
    //     else if (nKey == GLFW_KEY_A)
    //     {
    //         glm::vec3 direction = glm::normalize(cameraTarg - cameraPos);
    //         glm::vec3 vertical = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    //         if (nMode == GLFW_MOD_SHIFT)
    //             vertical *= 0.1;
    //         else if (nMode == GLFW_MOD_CONTROL)
    //             vertical *= 0.001;
    //         else
    //             vertical *= 0.01;
    //         cameraPos -= vertical;
    //         cameraTarg -= vertical;
    //     }
    //     else if (nKey == GLFW_KEY_D)
    //     {
    //         glm::vec3 direction = glm::normalize(cameraTarg - cameraPos);
    //         glm::vec3 vertical = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    //         if (nMode == GLFW_MOD_SHIFT)
    //             vertical *= 0.1;
    //         else if (nMode == GLFW_MOD_CONTROL)
    //             vertical *= 0.001;
    //         else
    //             vertical *= 0.01;
    //         cameraPos += vertical;
    //         cameraTarg += vertical;
    //     }
    //     else if (nKey == GLFW_KEY_H)
    //     {
    //         glm::vec3 direction = glm::normalize(cameraTarg - cameraPos);
    //         glm::vec3 vertical = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    //         if (nMode == GLFW_MOD_SHIFT)
    //             vertical *= 10;
    //         else if (nMode == GLFW_MOD_CONTROL)
    //             vertical *= 1;
    //         else
    //             vertical *= 0.01;
    //         cameraPos -= vertical;
    //         cameraTarg -= vertical;
    //     }
    //     else if (nKey == GLFW_KEY_J)
    //     {
    //         glm::vec3 direction = glm::normalize(cameraTarg - cameraPos);
    //         glm::vec3 vertical = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    //         if (nMode == GLFW_MOD_SHIFT)
    //             vertical *= 10;
    //         else if (nMode == GLFW_MOD_CONTROL)
    //             vertical *= 1;
    //         else
    //             vertical *= 0.01;
    //         cameraPos += vertical;
    //         cameraTarg += vertical;
    //     }
    //     else if (nKey == GLFW_KEY_Q)
    //     {
    //         GLfloat radius = glm::distance(cameraPos, cameraTarg);
    //         fRotateAngle += 0.00001f;

    //         GLfloat camX = sin(fRotateAngle) * radius + cameraTarg.x;
    //         GLfloat camZ = cos(fRotateAngle) * radius + cameraTarg.z;

    //         cameraPos = glm::vec3(camX, 0.0, camZ);
    //     }
    //     else if (nKey == GLFW_KEY_E)
    //     {
    //         GLfloat radius = glm::distance(cameraPos, cameraTarg);
    //         fRotateAngle -= 0.00001f;

    //         GLfloat camX = sin(fRotateAngle) * radius + cameraTarg.x;
    //         GLfloat camZ = cos(fRotateAngle) * radius + cameraTarg.z;

    //         cameraPos = glm::vec3(camX, 0.0, camZ);
    //     }
    //     else if (nKey == GLFW_KEY_Z)
    //     {
    //         glm::vec3 direction = glm::normalize(cameraTarg - cameraPos);
    //         glm::vec3 vertical = glm::normalize(glm::cross(direction, glm::vec3(1.0f, 0.0f, 0.0f)));
    //         if (nMode == GLFW_MOD_SHIFT)
    //             vertical *= -0.1;
    //         else if (nMode == GLFW_MOD_CONTROL)
    //             vertical *= -0.001;
    //         else
    //             vertical *= -0.01;
    //         cameraPos += vertical;
    //         cameraTarg += vertical;
    //     }
    //     else if (nKey == GLFW_KEY_C)
    //     {
    //         glm::vec3 direction = glm::normalize(cameraTarg - cameraPos);
    //         glm::vec3 vertical = glm::normalize(glm::cross(direction, glm::vec3(1.0f, 0.0f, 0.0f)));
    //         if (nMode == GLFW_MOD_SHIFT)
    //             vertical *= 0.1;
    //         else if (nMode == GLFW_MOD_CONTROL)
    //             vertical *= 0.001;
    //         else
    //             vertical *= 0.01;
    //         cameraPos += vertical;
    //         cameraTarg += vertical;
    //     }
    //     else if (nKey == GLFW_KEY_N)
    //     {
    //         glm::vec3 direction = glm::normalize(cameraTarg - cameraPos);
    //         glm::vec3 vertical = glm::normalize(glm::cross(direction, glm::vec3(1.0f, 0.0f, 0.0f)));
    //         if (nMode == GLFW_MOD_SHIFT)
    //             vertical *= -10;
    //         else if (nMode == GLFW_MOD_CONTROL)
    //             vertical *= -1;
    //         else
    //             vertical *= -0.01;
    //         cameraPos += vertical;
    //         cameraTarg += vertical;
    //     }
    //     else if (nKey == GLFW_KEY_M)
    //     {
    //         glm::vec3 direction = glm::normalize(cameraTarg - cameraPos);
    //         glm::vec3 vertical = glm::normalize(glm::cross(direction, glm::vec3(1.0f, 0.0f, 0.0f)));
    //         if (nMode == GLFW_MOD_SHIFT)
    //             vertical *= 10;
    //         else if (nMode == GLFW_MOD_CONTROL)
    //             vertical *= 1;
    //         else
    //             vertical *= 0.01;
    //         cameraPos += vertical;
    //         cameraTarg += vertical;
    //     }
    //     else if (nKey == GLFW_KEY_U) // 还原视图
    //     {
    //         cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    //         cameraTarg = glm::vec3(0.0f, 0.0f, 0.0f);
    //     }
    }
}


void mouseFunc(GLFWwindow *pWnd, int button, int action, int mods)
{
    // static int i = 0;
    if (action == GLFW_PRESS)
    {
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
        {
            std::cout<<"mouse: Left Btn:"<<button<<std::endl;
    //         printf("%.3f %.3f\t\t%.3f, %.3f\t\t\tLeft button clicked!\n", mousePos.x, mousePos.y, mousePos.z, mousePos.w);
        }
        break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            std::cout<<"mouse: Mind Btn:"<<button<<std::endl;
    //         printf("Middle button clicked\n%.3f\n%.3f\n", mousePos.z, mousePos.w);
    //         i = 0;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            std::cout<<"mouse: Right Btn:"<<button<<std::endl;
    //         printf("%.3f, %.3f, // %d \n", mousePos.z, mousePos.w, i++);
    //         // printf("%.3f, %.3f, // %d \n", mousePos.z, mousePos.w, i++);
            break;
        default:
            std::cout<<"Default Btn:"<<button<<std::endl;
            return;
        }
    }
    return;
}


void cursorFunc(GLFWwindow *pWnd, double x, double y)
{
    // std::cout<<"cursor x:"<<x<<" y:"<<y<<std::endl;
    // float xpos = float((x - WIDTH / 2) / WIDTH) * 2;
    // float ypos = float(0 - (y - HEIGH / 2) / HEIGH) * 2;
    // // printf("Mouse position move to [ %.3f : %.3f ]\n", xpos, ypos);
    // mousePos = glm::vec4(x, y, xpos, ypos);
    return;
}


void scrollFunc(GLFWwindow *pWnd, double x, double y)
{
    std::cout<<"scroll x:"<<x <<" y:"<<y<<std::endl;
    // if (fov >= 1.0f && fov <= 45.0f) {
    //   fov -= y;
    // }

    // fov = fov <= 1.0f ? 1.0f : fov;
    // fov = fov >= 45.0f ? 45.0f : fov;
    // dFov -= (float)y;
    // if (dFov >= 180)
    //     dFov = 179;
}
