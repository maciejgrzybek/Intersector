#include "Space.h"
#include "MedianSelect.hpp"

#include <algorithm>
#include <cmath>

#include <boost/random/uniform_int_distribution.hpp>

boost::random::mt19937 Space::randomGenerator;

Space::Space(unsigned int amount, bool rare)
{
    if(rare)
    {
        for(unsigned int i = 0;i<amount;++i)
        {
            /* produce points for square size 20*amount x 20*amount */
            int x = getRandomNumber(-amount*10,amount*10);
            int y = getRandomNumber(-amount*10,amount*10);
            Point randomPoint(x,y);
            points.push_back(randomPoint);
        }
    }
    else
    {
        // FIXME: implement this
        // Split into smaller pieces and localy in one generate not-rare points set.
    }
}

Space::Space(const Space& spaceToCopy) : points(spaceToCopy.points)
{}

Space::Space(const PointVector& pointVector) : points(pointVector)
{}

PointPairVector Space::pointsNeighborhood(unsigned int d)
{
    std::sort(points.begin(),points.end()); // because of representation points in vector, we need sort them by x-cordinate, to be able to find d-pairs on line, after projection
    return pointsNeighborhood(points.begin(),points.end(),d);
}

PointPairVector Space::bruteNeighbors(PointVector::iterator start, PointVector::iterator end, unsigned int d) const
{
    PointPairVector result;
    PointVector::iterator i = start;
    PointVector::iterator j = start;
    for(;i<end;++i)
    {
        for(j=i+1;j<end;++j)
        {
            if(j->distanceTo(*i)<=d)
            {
                result.insert(PointPair(*i,*j));
            }
        }
    }
    return result;
}

Space::PointdPairVector Space::bruteNeighbors(std::vector<dPoint>::iterator start, std::vector<dPoint>::iterator end, unsigned int d) const
{
    PointdPairVector result;
    std::vector<dPoint>::iterator i = start;
    std::vector<dPoint>::iterator j = start;
    for(;i<end;++i)
    {
        for(j=i+1;j<end;++j)
        {
            if(j->distanceTo(*i)<=d)
            {
                result.push_back(std::make_pair(*i,*j));
            }
        }
    }
    return result;
}

PointPairVector Space::pointsNeighborhood(PointVector::iterator start, PointVector::iterator end, unsigned int d)
{
    PointPairVector leftResult;
    PointPairVector rightResult;
    PointPairVector additionalPoints;
    if(end-start<=3)
    {
        return bruteNeighbors(start,end,d);
    }
    Point median = MedianSelect<Point>::selectKElement((end-start)/2,start,end);
    PointVector::iterator middle = std::find(start,end,median);
    if(middle == end) // if element not found, something is probably wrong with selectKElement() method
        throw std::exception(); // TODO change to proper exception

    leftResult = pointsNeighborhood(start,middle,d);
    rightResult = pointsNeighborhood(middle,end,d);
    leftResult.insert(rightResult.begin(),rightResult.end()); // append rightResult to the leftResult.
    additionalPoints = dPairsOnLine(d,middle);
    leftResult.insert(additionalPoints.begin(),additionalPoints.end()); // append additional points (projected onto split line)
    return leftResult;
}

PointPairVector Space::dPairsOnLine(unsigned int d, PointVector::const_iterator iter)
{
    std::vector<dPoint> dPoints;
    PointVector::const_iterator midIter = iter;
    Point diffPoint;
    // search the zone in d-distance from split-line
    // on the left side
    while(abs((diffPoint = (*midIter - *iter)).x) <= d)
    {
        dPoint tempPoint = *midIter;
        //tempPoint.x = iter->x; // projection onto split line
        tempPoint.isLeft = true;
        dPoints.push_back(tempPoint);
        if(midIter == points.begin())
            break;
        --midIter; // go to the left (sorted vector)
    }

    // on the right side
    midIter = iter+1;
    while(abs((diffPoint = (*midIter - *iter)).x) <= d)
    {
        if(midIter == points.end())
            break;
        dPoint tempPoint = *midIter;
        //tempPoint.x = iter->x; // projection onto split line
        tempPoint.isLeft = false;
        dPoints.push_back(tempPoint);
        ++midIter; // go to the right (sorted vector)
    }

    // calculating d-pairs on the line
    std::sort(dPoints.begin(),dPoints.end()); // O(nlogn)
    PointdPairVector neighbors = bruteNeighbors(dPoints.begin(),dPoints.end(),d);
/*    std::vector<dPoint>::iterator dp_i = dPoints.begin();
    std::vector<dPoint>::iterator dp_e = dPoints.end();
    for(;dp_i!=dp_e && (dp_i+1)!=dp_e;++dp_i)
    {
        if(abs((dp_i->y) - (dp_i+1)->y) <= d) // |point1.y - point2.y| <= d - condition for d-neighborhood on the line
        {
            dp_i->x = dp_i->originalX;
            (dp_i+1)->x = (dp_i+1)->originalX;
            PointdPair pp(*dp_i,*(dp_i+1));
            neighbors.push_back(pp);
        }
    }*/
    PointdPairVector::iterator pp_i = neighbors.begin();
    PointdPairVector::iterator pp_e = neighbors.end();

    PointPairVector result;

    for(;pp_i!=pp_e;++pp_i)
    {
        // TODO: check whether it is good condition:
        //if((pp_i->first).isLeft ^ (pp_i->second).isLeft) // possible neighbors have to be from different sides (left and right).
        {
            if(pp_i->first.distanceTo(pp_i->second) <= d) // if the real distance between points are not higher than d - they are real d-neighbors
            {
                result.insert(PointPair(pp_i->first,pp_i->second));
            }
        }
    }
    return result;
}

size_t Space::getPointsCount() const
{
    return points.size();
}

int Space::getRandomNumber(int start, int end) const
{
    boost::random::uniform_int_distribution<> dist(start, end);
    return dist(randomGenerator);
}

Space::dPoint::dPoint(const Point& point) : Point(point.x,point.y), originalX(point.x)
{}

bool Space::dPoint::operator<(const Space::dPoint& point) const
{
    return y<point.y;
}
