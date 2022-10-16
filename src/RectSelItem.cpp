#include "RectSelItem.h"

#include "RectSelItemRender.h"

RectSelItem::RectSelItem()
{
    m_pRender = new RectSelItemRender(this);
    // m_pRender->setColor(glm::vec4(0.0, 1.0, 1.0, 1.0));
}

RectSelItem::~RectSelItem()
{
    SAFE_DELETE(m_pRender);
}

void RectSelItem::render()
{
    m_pRender->render();
}


void RectSelItem::addPt(Pt& pt)
{
    if(m_pts.empty())
        m_ptFirst = pt;
    else
        m_ptSecond = pt;

    
    m_pts.emplace_back(pt);
    if(m_pts.size() < 2)
        return;

    double dMinX = std::min(m_ptFirst.x, m_ptSecond.x);
    double dMinY = std::min(m_ptFirst.y, m_ptSecond.y);

    double dMaxX = std::max(m_ptFirst.x, m_ptSecond.x);
    double dMaxY = std::max(m_ptFirst.y, m_ptSecond.y);

    m_pts.clear();
    m_pts.emplace_back(Pt(dMinX, dMinY));
    m_pts.emplace_back(Pt(dMaxX, dMinY));
    m_pts.emplace_back(Pt(dMaxX, dMaxY));
    m_pts.emplace_back(Pt(dMinX, dMaxY));
    m_pts.emplace_back(Pt(dMinX, dMinY));
    
    m_pRender->updateData();
}

void RectSelItem::clear()
{
    m_pts.clear();
    m_pts.reserve(2);
}

void RectSelItem::setColor(glm::vec4& vColor)
{
    // m_pRender->setColor(vColor);
    m_v4Color = vColor;
}
