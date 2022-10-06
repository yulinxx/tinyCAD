#ifndef _LINE_ITEM_RENDER_H
#define _LINE_ITEM_RENDER_H

#include "Render.h"
#include "DataDefine.h"

class LineItem;

class LineItemRender : public Render
{
public:
    LineItemRender(LineItem* item);
    virtual ~LineItemRender();

public:
    virtual void render() override;
    virtual void updateData() override;
    virtual void setColor(glm::vec4& vColor) override;

public:
    // void setData(LinePts* pts);

private:
    LineItem* m_pItem = nullptr;
};

#endif //_LINE_ITEM_RENDER_H