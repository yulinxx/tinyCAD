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
    virtual void render(){}
    virtual void updateData() = 0;

private:
public:
    void render(glm::mat4& proj, glm::mat4& view, glm::mat4& model, glm::vec4& color);

public:
    Shader* m_pShader = nullptr;
    unsigned int m_nShaderID = -1;

    unsigned int m_nVAO = -1;
    unsigned int m_nVBO = -1;
};

#endif //_RENDER_H