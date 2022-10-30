#include "ImgItem.h"

#include "ImgItemRender.h"

// #include <stb_image.h>

ImgItem::ImgItem()
{
    m_pRender = new ImgItemRender(this);
}

ImgItem::~ImgItem()
{
    SAFE_DELETE(m_pRender);
}

void ImgItem::render()
{
    m_pRender->render();
}

void ImgItem::addPt(Pt pt)
{
    m_pts.clear();
    m_pts.emplace_back(pt);
    m_pRender->updateData();
}

void ImgItem::setColor(glm::vec4 vColor)
{
    m_v4Color = vColor;
}

void ImgItem::SetImage(std::string strPath)
{
    
}

void ImgItem::SetImagePath(int w, int h)
{
    if(m_pts.size() < 1)
        m_pts.emplace_back(Pt{0.0, 0.0});
    
    double nX = m_pts[0].x;
    double nY = m_pts[0].y;

    m_pts.emplace_back(Pt{nX + w, nY});
    m_pts.emplace_back(Pt{nX + w, nY + h});
    m_pts.emplace_back(Pt{nX, nY + h});
}

// void ImgItem::setPtSize(float dPtSize)
// {
//     m_dItemW = dPtSize;
// }