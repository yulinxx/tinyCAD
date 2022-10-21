#ifndef _ITEM_H
#define _ITEM_H

#include "Shader.h"
#include "DataDefine.h"

#include <bitset>

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=nullptr; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=nullptr; } }


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
    virtual void addPt(Pt& pt) {}
    virtual void clear(){}

    virtual void setColor(glm::vec4& vColor) = 0;

    void setIsSelect(bool bSel = true){
        bSel? m_bitFlag.set(E_Sel) : m_bitFlag.reset(E_Sel);
    }

public:
    glm::mat4 m_matProj{1.0f}; 
    glm::mat4 m_matView{1.0f};
    glm::mat4 m_matModel{1.0f};
    glm::vec4 m_v4Color{1.0, 0.0, 0.0, 1.0};

    std::bitset<8> m_bitFlag;   // 标志符设置 0 是否选择 1 是否能选择 2 是否显示
};

#endif // _ITEM_H