#ifndef _RECT_SEL_ITEM_H_
#define _RECT_SEL_ITEM_H_

#include "Item.h"
#include "DataDefine.h"

class Render;

class RectSelItem : public Item
{
public:
    RectSelItem();
    virtual ~RectSelItem();

public:
    virtual void render() override;
    virtual void addPt(Pt pt) override;
    virtual void clear() override;
    virtual void setColor(glm::vec4 vColor) override;

public:
    Pt  m_ptFirst;
    Pt  m_ptSecond;
    LinePts m_pts;

private:
    Render* m_pRender = nullptr;
};

#endif // _RECT_SEL_ITEM_H_