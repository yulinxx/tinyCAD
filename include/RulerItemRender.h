#ifndef _RULER_ITEM_RENDER_H
#define _RULER_ITEM_RENDER_H

#include "Render.h"
#include "DataDefine.h"

class RulerItem;

class RulerItemRender : public Render
{
public:
    RulerItemRender(RulerItem* item);
    virtual ~RulerItemRender();

public:
    virtual void render() override;
    virtual void updateData() override;
    virtual void setColor(glm::vec4& vColor) override;

public:
    // void setData(LinePts* pts);

private:
    RulerItem* m_pItem = nullptr;
};

#endif //_RULER_ITEM_RENDER_H