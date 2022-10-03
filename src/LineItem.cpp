#include "LineItem.h"

LineItem::LineItem()
{
    if(!m_pShader)
    {
        m_pShader = new Shader("line.vs", "line.fs");
        m_nShaderID = m_pShader->ID;
    }

    glGenVertexArrays(1, &m_nVAO);
    glGenBuffers(1, &m_nVBO);

    glBindVertexArray(m_nVAO);  
    glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    // glBufferData(GL_ARRAY_BUFFER, m_pts.size() * sizeof(Pt), &m_pts[0], GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Pt), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void LineItem::render()
{
    m_pShader->use();

    m_pShader->setMat4("projection", glm::mat4(1.0f));
    m_pShader->setMat4("view", glm::mat4(1.0f));
    m_pShader->setMat4("model", glm::mat4(1.0f));

    glBindVertexArray(m_nVAO);
    glDrawArrays(GL_LINE_STRIP, 0, GLsizei(m_pts.size()));
}

void LineItem::update()
{
    glBindVertexArray(m_nVAO);  
    glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    glBufferData(GL_ARRAY_BUFFER, m_pts.size() * sizeof(Pt), &m_pts[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Pt), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void LineItem::addPt(Pt& pt)
{
    m_pts.emplace_back(pt);
    update();
}