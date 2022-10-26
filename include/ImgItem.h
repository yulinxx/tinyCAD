#ifndef _IMG_ITEM_H_
#define _IMG_ITEM_H_

#include "Item.h"
#include "DataDefine.h"

class Render;

class ImgItem : public Item
{
public:
    ImgItem();
    virtual ~ImgItem();

public:
    virtual void render() override;
    virtual void addPt(Pt& pt) override;
    virtual void setColor(glm::vec4& vColor) override;


public:
    void SetImage(std::string strPath);
    void SetImagePath(int w, int h);

public:
    LinePts m_pts;
    LinePts m_ptsTexture{{0,0},{1,0},{1,1},{0,1}};

private:
    Render* m_pRender = nullptr;
};

#endif //_IMG_ITEM_H_