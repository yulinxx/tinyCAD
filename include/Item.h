#ifndef _ITEM_H
#define _ITEM_H

#include "Shader.h"
#include "DataDefine.h"

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

class Item
{
public:
    Item() = default;

    virtual ~Item() {
        SAFE_DELETE(m_pShader);
        glDeleteVertexArrays(1, &m_nVAO);
        glDeleteBuffers(1, &m_nVBO);
        glDeleteProgram(m_nShaderID);
    }

public:
    virtual void render() = 0;
    virtual void update() = 0;
    virtual void addPt(Pt& pt) = 0;

public:
    Shader* m_pShader = nullptr;
    unsigned int m_nShaderID = -1;

    unsigned int m_nVAO = -1;
    unsigned int m_nVBO = -1;

    

};

#endif // _ITEM_H