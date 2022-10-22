#ifndef _LINE_ITEM_H
#define _LINE_ITEM_H

#include "Item.h"
#include "DataDefine.h"

class Render;

class LineItem : public Item
{
public:
    LineItem();
    virtual ~LineItem();

public:
    virtual void render() override;
    virtual void addPt(Pt& pt) override;
    virtual void setColor(glm::vec4& vColor) override;

public:
    void setLineWidth(int nW);
    void setResolution(Pt& pt);
    void setPattern(unsigned int nPattern);
    void setFactor(float dFactor);

public:
    LinePts m_pts;

    Pt m_ptResolution{1200, 800};
    unsigned int m_nPattern = 0x18ff;
    float m_dFactor = 1.0f;

private:
    Render* m_pRender = nullptr;
};

#endif //_LINE_ITEM_H