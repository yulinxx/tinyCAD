#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. 
// Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  100.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

//////////////////////////////////////////////////////////////////
// An abstract camera class that processes input and calculates the corresponding Euler Angles, 
// Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), 
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
        float yaw = YAW, 
        float pitch = PITCH);

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, 
        float upX, float upY, float upZ, 
        float yaw, float pitch);

    // camera Attributes
    glm::vec3 m_v3Position;
    glm::vec3 m_v3Front;
    glm::vec3 m_v3Up;
    glm::vec3 m_v3Right;
    glm::vec3 m_v3WorldUp;

    glm::vec3 m_v3PositionOri; // 保存原始位置,用于和现在位置进行对比偏移

    // euler Angles
    float m_dYaw = 0.0f;
    float m_dPitch = 0.0f;

    // camera options
    float m_dMoveSpeed = 0.0f;
    float m_dMouseSensitivity = 0.0f;
    float m_dZoom = 0.0f;


    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // processes input received from any keyboard-like input system. 
    // Accepts input parameter in the form of camera defined ENUM 
    // (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // processes input received from a mouse input system. 
    // Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. 
    // Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

};

#endif // _CAMERA_H_