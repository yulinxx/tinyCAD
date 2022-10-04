#ifndef _ITEM_H
#define _ITEM_H

#include "Shader.h"
#include "DataDefine.h"


class Item
{
public:
    Item() = default;

    virtual ~Item() {
    }

public:
    virtual void render() = 0;
    virtual void addPt(Pt& pt) = 0;

public:
    glm::mat4 matProj = glm::mat4(1.0f); 
    glm::mat4 matView = glm::mat4(1.0f);
    glm::mat4 matModel = glm::mat4(1.0f);
};

#endif // _ITEM_H