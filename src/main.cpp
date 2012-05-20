#include <iostream>
#include <vector>
#include "Figure.h"
#include "FigureFactory.h"
#include "AdjacencyGraph.h"
#include "Space.h"
#include <cstdlib>

FigureFactory* getFigureFactory()
{
    return &(CircleFactory::getInstance());
}

int main(int argc,char* argv[])
{
/*    Point pointA;
    Point pointB;
    pointA.x = 0;
    pointA.y = 0;
    pointB.x = 5;
    pointB.y = 5;
    std::vector<Figure*> vec;
    vec.push_back(new Square(pointA,2));
    vec.push_back(new Square(pointB,2));
    //std::cout << vec.at(0)->intersects(*vec.at(1)) << std::endl;
    std::vector<Point> v;
    for(int i = 0;i<atoi(argv[1]);++i)
        v.push_back(Point(i,i));

    Space s(v);
    Point::counter = 0;
    std::cout << Point::counter << std::endl;
    PointPairVector neighborhood = s.pointsNeighborhood(atoi(argv[2]));
    std::cout << Point::counter << std::endl;
    return 0;*/
    std::cout << "Welcome to Intersector" << std::endl;
    Space s(100); // create 100 points
    AdjacencyGraph graph = s.buildIntersectionGraph(s.pointsNeighborhood(1),1); // create graph of intersecting circles of radius 1
    graph.solve();
    PointVector points = graph.getPointVector();
    for(PointVector::const_iterator iter = points.begin();iter!=points.end();++iter)
    {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
    return 0;
}
