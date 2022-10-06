#ifndef _ITEM_H
#define _ITEM_H

#include "Shader.h"
#include "DataDefine.h"

#include <bitset>

enum Flag{E_Sel, E_CanSel, E_Show, E_Group, 
    Placeholder1, Placeholder2, Placeholder3, Placeholder4};

class Item
{
public:
    Item() = default;

    virtual ~Item() {
    }

public:
    virtual void render() = 0;
    virtual void addPt(Pt& pt) = 0;

    virtual void setColor(glm::vec4& vColor) = 0;

    void setIsSelect(bool bSel = true){
        bSel? m_bitFlag.set(E_Sel) : m_bitFlag.reset(E_Sel);
    }

public:
    glm::mat4 matProj = glm::mat4(1.0f); 
    glm::mat4 matView = glm::mat4(1.0f);
    glm::mat4 matModel = glm::mat4(1.0f);

    std::bitset<8> m_bitFlag;   // 标志符设置 0 是否选择 1 是否能选择 2 是否显示
};

#endif // _ITEM_H