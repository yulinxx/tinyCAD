#include "RulerItemRender.h"

#include "RulerItem.h"

RulerItemRender::RulerItemRender(RulerItem* item)
    : m_pItem(item)
{
    if(!m_pShader)
    {
        m_pShader = new Shader("ruler.vs", "ruler.fs");
        m_nShaderID = m_pShader->ID;
    }

    glGenVertexArrays(1, &m_nVAO);
    glGenBuffers(1, &m_nVBO);
}

RulerItemRender::~RulerItemRender()
{

}

void RulerItemRender::render()
{
    Render::render(m_pItem->m_matProj, m_pItem->m_matView, m_pItem->m_matModel, m_pItem->m_v4Color);
    glLineWidth(m_pItem->m_dItemW);
    glDrawArrays(GL_LINES, 0, GLsizei(m_pItem->m_pts.size()));
}

void RulerItemRender::updateData()
{
    glBindVertexArray(m_nVAO);  
    glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    glBufferData(GL_ARRAY_BUFFER, m_pItem->m_pts.size() * sizeof(Pt), &m_pItem->m_pts[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Pt), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}