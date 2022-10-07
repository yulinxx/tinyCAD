#include "TreeIndex.h"

void TreeIndex::add(LineItem* pLine)
{
    //创建多边形
    DPolygon polg;
    if(pLine->m_pts.size() < 3)
        return;

    // for (int i = 0; i < pLine->m_pts.size(); i++)
    //     polg.outer().push_back(DPoint(pLine->m_pts[i].x, pLine->m_pts[i].y));

    for (int i = 0; i < pLine->m_pts.size(); i++)
        polg.push_back(DPoint(pLine->m_pts[i].x, pLine->m_pts[i].y));

    m_vecPolygons.push_back(polg);

    //计算多边形包围矩形并插入R树
    DBox b = bg::return_envelope<DBox>(polg);
    //插入R树
    m_rtree.insert(std::make_pair(b, m_rtree.size())); //r树插入外包围矩形 i为索引
}

bool TreeIndex::selTest(Pt& ptA, Pt& ptB)
{
    // 按矩形范围查找 查询与矩形相交的矩形索引
    DBox queryBox(DPoint(std::min(ptA.x, ptB.x), std::min(ptA.y, ptB.y)), 
                  DPoint(std::max(ptA.x, ptB.x), std::max(ptA.y, ptB.y)));
    std::cout << "-box  x : " << queryBox.min_corner().x() << " "  << queryBox.min_corner().y()
              << " x: " << queryBox.max_corner().x()
              << " y: " << queryBox.max_corner().y() << std::endl;

    std::vector<BoxValuePair> vecBoxRes;
    m_rtree.query(bgi::intersects(queryBox), std::back_inserter(vecBoxRes));

    bool bFind = false;
    if(vecBoxRes.size() > 0)
    {
        std::cout << "--- spatial query result:"<< vecBoxRes.size() << std::endl;
        bFind = true;
    }

    // BOOST_FOREACH (BoxValuePair const &v, vecBoxRes)
    for(BoxValuePair const &v : vecBoxRes)
    {
        DPoint ptMin = v.first.min_corner();
        DPoint ptMax = v.first.max_corner();
        std::cout << "Index:"<<v.second<<"\t"<<bg::wkt<DPoint>(ptMin) 
            << "\t"<<bg::wkt<DPoint>(ptMax) << std::endl;

        std::cout << bg::wkt<DPolygon>(m_vecPolygons[v.second]) << std::endl;
    }

    std::cout << std::endl;
    return bFind;
}