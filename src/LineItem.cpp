#include "LineItem.h"

#include "LineItemRender.h"

LineItem::LineItem()
{
    m_pRender = new LineItemRender(this);
}

LineItem::~LineItem()
{
    SAFE_DELETE(m_pRender);
}

void LineItem::render()
{
    m_pRender->render();
}


void LineItem::addPt(Pt& pt)
{
    m_pts.emplace_back(pt);

    if(m_pts.size() % 2)
        setColor(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
    else
        setColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    
    m_pRender->updateData();
}

void LineItem::setColor(glm::vec4& vColor)
{
    // m_pRender->setColor(vColor);
    m_v4Color = vColor;
}
