#ifndef _LINE_ITEM_H
#define _LINE_ITEM_H

#include "Item.h"
#include "DataDefine.h"

class Render;

class LineItem : public Item
{
public:
    LineItem();
    virtual ~LineItem() = default;

public:
    // virtual void update() override;
    virtual void render() override;
    virtual void addPt(Pt& pt) override;

public:
    LinePts m_pts;

private:
    Render* m_pRender = nullptr;
};

#endif //_LINE_ITEM_H