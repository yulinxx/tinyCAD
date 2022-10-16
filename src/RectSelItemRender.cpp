#include "RectSelItemRender.h"

#include "RectSelItem.h"

RectSelItemRender::RectSelItemRender(RectSelItem* item)
    : m_pItem(item)
{
    if(!m_pShader)
    {
        m_pShader = new Shader("selRectange.vs", "selRectange.fs");
        m_nShaderID = m_pShader->ID;
    }

    glGenVertexArrays(1, &m_nVAO);
    glGenBuffers(1, &m_nVBO);
}

RectSelItemRender::~RectSelItemRender()
{

}

void RectSelItemRender::render()
{
    Render::render(m_pItem->m_matProj, m_pItem->m_matView, m_pItem->m_matModel, m_pItem->m_v4Color);
    glDrawArrays(GL_LINE_STRIP, 0, GLsizei(m_pItem->m_pts.size()));
}

void RectSelItemRender::updateData()
{
    glBindVertexArray(m_nVAO);  
    glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    glBufferData(GL_ARRAY_BUFFER, m_pItem->m_pts.size() * sizeof(Pt), &m_pItem->m_pts[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Pt), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}
