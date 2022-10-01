#include <vector>

struct Pt
{
    Pt(){}
    Pt(double x, double y, double z = 0.0){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

using Lines = std::vector<Pt>;