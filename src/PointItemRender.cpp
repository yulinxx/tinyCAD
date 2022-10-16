#include "PointItemRender.h"

#include "PointItem.h"

PointItemRender::PointItemRender(PointItem* item)
    : m_pItem(item)
{
    if(!m_pShader)
    {
        m_pShader = new Shader("point.vs", "point.fs");
        m_nShaderID = m_pShader->ID;
    }

    glGenVertexArrays(1, &m_nVAO);
    glGenBuffers(1, &m_nVBO);
}

PointItemRender::~PointItemRender()
{

}

void PointItemRender::render()
{
    Render::render(m_pItem->m_matProj, m_pItem->m_matView, m_pItem->m_matModel, m_pItem->m_v4Color);

    m_pShader->setInt("ptsize", m_pItem->m_nPtSize);

    glDrawArrays(GL_POINTS, 0, GLsizei(1));
}

void PointItemRender::updateData()
{
    glBindVertexArray(m_nVAO);  
    glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Pt), &m_pItem->m_pt, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Pt), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    glEnable(GL_PROGRAM_POINT_SIZE);
}
