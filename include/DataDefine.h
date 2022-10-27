#ifndef _DATA_DEFINE_H_
#define _DATA_DEFINE_H_

#include <vector>
#include <math.h>
struct Pt
{
    Pt(){}
    Pt(double x, double y, double z = 0.0){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Pt& operator=(const Pt& pt)
    {
        this->x = pt.x;
        this->y = pt.y;
        this->z = pt.z;
        return *this;
    }
    
    bool operator==(const Pt& pt)
    {
        if(fabs(this->x - x) < 0.0001
        && fabs(this->y - y) < 0.0001
        && fabs(this->z - z) < 0.0001 )
        {
            return true;
        }
        return false;
    }
    
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

using LinePts = std::vector<Pt>;


#endif //_DATA_DEFINE_H_