#ifndef _POINT_ITEM_RENDER_H
#define _POINT_ITEM_RENDER_H

#include "Render.h"
#include "DataDefine.h"

class PointItem;

class PointItemRender : public Render
{
public:
    PointItemRender(PointItem* item);
    virtual ~PointItemRender();

public:
    virtual void render() override;
    virtual void updateData() override;
    virtual void setColor(glm::vec4& vColor) override;

public:
    // void setData(LinePts* pts);

private:
    PointItem* m_pItem = nullptr;
};

#endif //_POINT_ITEM_RENDER_H