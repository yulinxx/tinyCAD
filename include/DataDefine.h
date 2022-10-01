#include <vector>

struct Pt
{
    Pt(){}
    Pt(double x, double y){
        this->x = x;
        this->y = y;
    }

    double x = 0.0;
    double y = 0.0;
};


using Lines = std::vector<Pt>;

using PtVecs = std::vector<double>;