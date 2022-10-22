#ifndef _POINT_ITEM_H_
#define _POINT_ITEM_H_

#include "Item.h"
#include "DataDefine.h"

class Render;

class PointItem : public Item
{
public:
    PointItem();
    PointItem(Pt& pt);
    virtual ~PointItem();

public:
    virtual void render() override;
    virtual void addPt(Pt& pt) override;
    virtual void setColor(glm::vec4& vColor) override;

public:    
    void setPtSize(float dPtSize);

public:
    Pt m_pt;

private:
    Render* m_pRender = nullptr;
};

#endif //_POINT_ITEM_H_