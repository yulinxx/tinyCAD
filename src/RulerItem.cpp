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


void RulerItem::initPt()
{
    m_pts.clear();

    int w = 800;
    int h = 600;
    for(int i = -w/2; i < w/2; i += 100)
    {
        m_pts.emplace_back(Pt{double(i), double(h / 2)});
        m_pts.emplace_back(Pt{double(i), double(h / 2 - 15)});

        for(int j = i + 10; j < (i + 100); j += 10)
        {
            m_pts.emplace_back(Pt{double(j), double(h / 2)});
            m_pts.emplace_back(Pt{double(j), double(h / 2 - 10)});
        }
    }
    m_pRender->updateData();
}

void RulerItem::setColor(glm::vec4& vColor)
{
    m_pRender->setColor(vColor);
}
