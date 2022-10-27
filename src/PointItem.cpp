#include "PointItem.h"

#include "PointItemRender.h"

PointItem::PointItem()
{
    m_pRender = new PointItemRender(this);
}

PointItem::PointItem(Pt& pt)
{
    m_pt = pt;
    m_pRender = new PointItemRender(this);
    m_pRender->updateData();
}

PointItem::~PointItem()
{
    SAFE_DELETE(m_pRender);
}

void PointItem::render()
{
    m_pRender->render();
}

void PointItem::addPt(Pt pt)
{
    m_pt = pt;
    m_pRender->updateData();
}

void PointItem::setColor(glm::vec4 vColor)
{
    m_v4Color = vColor;
}

void PointItem::setPtSize(float dPtSize)
{
    m_dItemW = dPtSize;
}