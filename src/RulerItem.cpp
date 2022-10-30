#include "RulerItem.h"

#include "RulerItemRender.h"


RulerItem::RulerItem()
{
    m_pRender = new RulerItemRender(this);

    initPt();
}

RulerItem::~RulerItem()
{
    SAFE_DELETE(m_pRender);
}

void RulerItem::render()
{
    m_pRender->render();
}


void RulerItem::initPt(double dWndW /*=1200*/, double dWndH /*=800*/)
{
    m_pts.clear();

    // 横向尺子
    for(int i = -dWndW/2; i < dWndW/2; i += 100)
    {
        m_pts.emplace_back(Pt{double(i), double(dWndH / 2)});
        m_pts.emplace_back(Pt{double(i), double(dWndH / 2 - 15)});

        for(int j = i + 10; j < (i + 100); j += 10)
        {
            m_pts.emplace_back(Pt{double(j), double(dWndH / 2)});
            m_pts.emplace_back(Pt{double(j), double(dWndH / 2 - 10)});
        }
    }
    m_pts.emplace_back(Pt{-dWndW / 2, dWndH / 2});
    m_pts.emplace_back(Pt{dWndW / 2, dWndH / 2});

    // 竖向尺子
    for(int i = -dWndH/2; i < dWndH/2; i += 100)
    {
        m_pts.emplace_back(Pt{double(-dWndW / 2), double(i)});
        m_pts.emplace_back(Pt{double(-dWndW / 2 + 15), double(i)});

        for(int j = i + 10; j < (i + 100); j += 10)
        {
            m_pts.emplace_back(Pt{double(-dWndW / 2), double(j), });
            m_pts.emplace_back(Pt{double(-dWndW / 2 + 10), double(j)});
        }
    }
    m_pts.emplace_back(Pt{-dWndW / 2, -dWndH / 2});
    m_pts.emplace_back(Pt{-dWndW / 2, dWndH / 2});

    m_pRender->updateData();
}

void RulerItem::setColor(glm::vec4 vColor)
{
    // m_pRender->setColor(vColor);
    m_v4Color = vColor;
}
