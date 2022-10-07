#ifndef _LINE_ITEM_RENDER_H
#define _LINE_ITEM_RENDER_H

#include "Render.h"
#include "DataDefine.h"

class RectSelItem;

class RectSelItemRender : public Render
{
public:
    RectSelItemRender(RectSelItem* item);
    virtual ~RectSelItemRender();

public:
    virtual void render() override;
    virtual void updateData() override;
    virtual void setColor(glm::vec4& vColor) override;

public:
    // void setData(LinePts* pts);

private:
    RectSelItem* m_pItem = nullptr;
};

#endif //_LINE_ITEM_RENDER_H