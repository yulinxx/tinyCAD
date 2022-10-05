#include "Camera.h"

#include <iostream>


Camera::Camera(glm::vec3 position /*= glm::vec3(0.0f, 0.0f, 3.0f)*/, 
        glm::vec3 up /*= glm::vec3(0.0f, 1.0f, 0.0f)*/, 
        float yaw /*= YAW*/, 
        float pitch /*= PITCH*/) 
    : m_v3Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_dMoveSpeed(SPEED), 
    m_dMouseSensitivity(SENSITIVITY), m_dZoom(ZOOM)
{
    m_v3Position = position;
    m_v3WorldUp = up;
    m_dYaw = yaw;
    m_dPitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, 
            float upY, float upZ, float yaw, float pitch)
    : m_v3Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_dMoveSpeed(SPEED), 
    m_dMouseSensitivity(SENSITIVITY), m_dZoom(ZOOM)
{
    m_v3Position = glm::vec3(posX, posY, posZ);
    m_v3WorldUp = glm::vec3(upX, upY, upZ);
    m_dYaw = yaw;
    m_dPitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_v3Position, m_v3Position + m_v3Front, m_v3Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = m_dMoveSpeed * deltaTime;
    velocity = 2;
    if (direction == FORWARD)
        m_v3Position += m_v3Front * velocity;
    if (direction == BACKWARD)
        m_v3Position -= m_v3Front * velocity;
    if (direction == LEFT)
        m_v3Position -= m_v3Right * velocity;
    if (direction == RIGHT)
        m_v3Position += m_v3Right * velocity;

    std::cout<<m_v3Position.x<<" "<<m_v3Position.y<<" "<<m_v3Position.z<<std::endl;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch /*= true*/)
{
    xoffset *= m_dMouseSensitivity;
    yoffset *= m_dMouseSensitivity;

    m_dYaw += xoffset;
    m_dPitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (m_dPitch > 89.0f)
            m_dPitch = 89.0f;
        if (m_dPitch < -89.0f)
            m_dPitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    m_dZoom -= (float)yoffset;
    if (m_dZoom < 1.0f)
        m_dZoom = 1.0f;
    if (m_dZoom > 45.0f)
        m_dZoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_dYaw)) * cos(glm::radians(m_dPitch));
    front.y = sin(glm::radians(m_dPitch));
    front.z = sin(glm::radians(m_dYaw)) * cos(glm::radians(m_dPitch));
    m_v3Front = glm::normalize(front);
    // also re-calculate the Right and Up vector

    // normalize the vectors, because their length gets closer to 0 
    // the more you look up or down which results in slower movement.
    m_v3Right = glm::normalize(glm::cross(m_v3Front, m_v3WorldUp)); 
    m_v3Up = glm::normalize(glm::cross(m_v3Right, m_v3Front));
}
