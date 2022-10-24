#include "ImgItemRender.h"

#include "ImgItem.h"
#include <stb_image.h>

ImgItemRender::ImgItemRender(ImgItem* item)
    : m_pItem(item)
{
    if(!m_pShader)
    {
        m_pShader = new Shader("image.vs", "image.fs");
        m_nShaderID = m_pShader->ID;
    }

    glGenVertexArrays(1, &m_nVAO);
    glGenBuffers(1, &m_nVBO);
    glGenBuffers(1, &m_nEBO);
}

ImgItemRender::~ImgItemRender()
{

}

void ImgItemRender::render()
{
    // Render::render(m_pItem->m_matProj, m_pItem->m_matView, m_pItem->m_matModel, m_pItem->m_v4Color);
    m_pShader->use();

    m_pShader->setMat4("projection", m_pItem->m_matProj); 
    m_pShader->setMat4("view", m_pItem->m_matView);
    m_pShader->setMat4("model", m_pItem->m_matModel);

    glBindVertexArray(m_nVAO);
    glBindTexture(GL_TEXTURE_2D, m_nTexture);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ImgItemRender::updateData()
{
    int nW, nH, nChannels;
    unsigned char *data = stbi_load(m_strImgPath.c_str(), &nW, &nH, &nChannels, 0);
    if(!data)
        return;

    m_pItem->SetImagePath(nW, nH);
    
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glBindVertexArray(m_nVAO);  
    glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    glBufferData(GL_ARRAY_BUFFER, m_pItem->m_pts.size() * sizeof(Pt), &m_pItem->m_pts[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Pt), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

 
    glGenTextures(1, &m_nTexture);
    glBindTexture(GL_TEXTURE_2D, m_nTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nW, nH, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    stbi_image_free(data);
}