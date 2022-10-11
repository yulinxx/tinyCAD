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