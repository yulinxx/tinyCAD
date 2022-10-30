#include "LineItemRender.h"

#include "LineItem.h"

LineItemRender::LineItemRender(LineItem* item)
    : m_pItem(item)
{
    if(!m_pShader)
    {
        m_pShader = new Shader("line.vs", "line.fs");
        m_nShaderID = m_pShader->ID;
    }

    glGenVertexArrays(1, &m_nVAO);
    glGenBuffers(1, &m_nVBO);
}

LineItemRender::~LineItemRender()
{

}

void LineItemRender::render()
{
    Render::render(m_pItem->m_matProj, m_pItem->m_matView, m_pItem->m_matModel, m_pItem->m_v4Color);

    m_pShader->setVec2("v2Resolution", (float)m_pItem->m_ptResolution.x, (float)m_pItem->m_ptResolution.y); 
    m_pShader->setUInt("nPattern", m_pItem->m_nPattern);
    m_pShader->setFloat("dFactor", m_pItem->m_dFactor);
    
    glLineWidth(m_pItem->m_dItemW);

    glDrawArrays(GL_LINE_STRIP, 0, GLsizei(m_pItem->m_pts.size()));
}

void LineItemRender::updateData()
{
    if(m_pItem->m_pts.size() <= 0)
        return;
        
    glBindVertexArray(m_nVAO);  
    glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    glBufferData(GL_ARRAY_BUFFER, m_pItem->m_pts.size() * sizeof(Pt), &m_pItem->m_pts[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Pt), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}
