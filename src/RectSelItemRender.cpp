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
    m_pShader->use();

    m_pShader->setMat4("projection", m_pItem->matProj); 
    m_pShader->setMat4("view", m_pItem->matView);
    m_pShader->setMat4("model", m_pItem->matModel);

    glBindVertexArray(m_nVAO);
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

void RectSelItemRender::setColor(glm::vec4& vColor)
{
   m_pShader->setVec4("color", vColor); 
}