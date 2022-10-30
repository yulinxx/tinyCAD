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
    if(m_pts.size() > 0)
        m_pRender->render();
}


void LineItem::addPt(Pt pt)
{
    m_pts.emplace_back(pt);

    if(m_pts.size() % 2)
    {
        auto c = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);
        setColor(c);
    }
    else
    {
        auto c = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);
        setColor(c);
    }
    
    m_pRender->updateData();
}

void LineItem::setLineWidth(double dW)
{
    m_dItemW = dW;
    // m_pRender->updateData();
}
void LineItem::setColor(glm::vec4 vColor)
{
    // m_pRender->setColor(vColor);
    m_v4Color = vColor;
}

void LineItem::setResolution(Pt pt)
{
    m_ptResolution = pt;
}

void LineItem::setPattern(unsigned int nPattern)
{
    m_nPattern = nPattern;
}

void LineItem::setFactor(float dFactor)
{
    m_dFactor = dFactor;
}
