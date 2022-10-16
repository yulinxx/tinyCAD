#include "Render.h"
#include "Shader.h"

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=nullptr; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=nullptr; } }

Render::~Render()
{
    glDeleteVertexArrays(1, &m_nVAO);
    glDeleteBuffers(1, &m_nVBO);
    glDeleteProgram(m_nShaderID);
    SAFE_DELETE(m_pShader);
}

void Render::render(glm::mat4& proj, glm::mat4& view, glm::mat4& model, glm::vec4& color)
{
    m_pShader->use();

    m_pShader->setMat4("projection", proj); 
    m_pShader->setMat4("view", view);
    m_pShader->setMat4("model", model);

    m_pShader->setVec4("color", color);

    glBindVertexArray(m_nVAO);
}