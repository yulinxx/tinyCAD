#ifndef _RENDER_H
#define _RENDER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader;

class Render
{
public:
    Render() = default;
    virtual ~Render();

public:
    virtual void render() = 0;
    virtual void updateData() = 0;

    virtual void setColor(glm::vec4& vColor) = 0;

    Shader* m_pShader = nullptr;
    unsigned int m_nShaderID = -1;

    unsigned int m_nVAO = -1;
    unsigned int m_nVBO = -1;
};

#endif //_RENDER_H