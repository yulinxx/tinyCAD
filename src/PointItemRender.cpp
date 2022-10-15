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
    m_pShader->use();

    m_pShader->setMat4("projection", m_pItem->matProj); 
    m_pShader->setMat4("view", m_pItem->matView);
    m_pShader->setMat4("model", m_pItem->matModel);

    glBindVertexArray(m_nVAO);
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

void PointItemRender::setColor(glm::vec4& vColor)
{
   m_pShader->setVec4("color", vColor); 
}