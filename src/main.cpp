#include <iostream>
#include <vector>
#include "Figure.h"

int main(void)
{
    Point pointA;
    Point pointB;
    pointA.x = 0;
    pointA.y = 0;
    pointB.x = 5;
    pointB.y = 5;
    std::vector<Figure*> vec;
    vec.push_back(new Square(pointA,2));
    vec.push_back(new Square(pointB,2));
    std::cout << vec.at(0)->intersects(*vec.at(1)) << std::endl;
    return 0;
}
