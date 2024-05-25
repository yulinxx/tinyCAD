#ifndef _TREE_INDEX_H_
#define _TREE_INDEX_H_

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/foreach.hpp>

#include <cmath>
#include <vector>
#include <iostream>

class Item;
class LineItem;
class PointItem;

#include "DataDefine.h"

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

// typedef bg::model::point<double, 2, bg::cs::cartesian> DPoint; // 双精度的点
typedef bg::model::d2::point_xy<double> DPoint;
typedef bg::model::box<DPoint> DBox;    // 矩形
// typedef bg::model::polygon<DPoint, false, false> DPolygon; // ccw, open polygon
typedef bg::model::ring<DPoint> DPolygon;
// typedef std::pair<DBox, unsigned> BoxValuePair;
typedef std::pair<DBox, Item*> BoxValuePair;


class TreeIndex
{
public:
    TreeIndex() = default;
    ~TreeIndex() = default;

public:
    void add(LineItem* pLin);
    void add(PointItem* pPt);
    // bool selTest(Pt& ptA, Pt& ptB);
    std::vector<Item*> selTest(Pt& ptA, Pt& ptB);

    bool eraseItem(Item* pLine);
    bool eraseItem(std::vector<Item*> vecLines);

public:
    //创建R树
    bgi::rtree<BoxValuePair, bgi::rstar<16, 4>> m_rtree; //最大最小

    std::vector<DPolygon> m_vecPolygons;
};

#endif //_TREE_INDEX_H_