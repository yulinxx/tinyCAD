#ifndef _IMG_ITEM_RENDER_H
#define _IMG_ITEM_RENDER_H

#include "Render.h"
#include "DataDefine.h"
#include <string>

class ImgItem;

class ImgItemRender : public Render
{
public:
    ImgItemRender(ImgItem* item);
    virtual ~ImgItemRender();

public:
    virtual void render() override;
    virtual void updateData() override;

public:

private:
    ImgItem* m_pItem = nullptr;
    unsigned int m_nTexture = 0;

    // std::string m_strImgPath = "E:/Project/tinyCAD/res/container.jpg";
    std::string m_strImgPath = "E:/Project/tinyCAD/res/container.jpg";
};

#endif //_IMG_ITEM_RENDER_H