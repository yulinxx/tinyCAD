#ifndef _RULER_ITEM_H
#define _RULER_ITEM_H

#include "Item.h"
#include "DataDefine.h"

class Render;

class RulerItem: public Item
{
public:
    RulerItem();
    virtual ~RulerItem();

public:
    virtual void render() override;
    virtual void setColor(glm::vec4 vColor) override;

    void initPt(double dWndW = 1200, double dWndH = 800);

public:
    LinePts m_pts;

private:
    Render* m_pRender = nullptr;
};

#endif //_RULER_ITEM_H_