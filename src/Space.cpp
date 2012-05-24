#include "Space.h"
#include "MedianSelect.hpp"
#include "FigureFactory.h"
#include "AdjacencyGraph.h"

#include <algorithm>
#include <cmath>

#include <boost/random/uniform_int_distribution.hpp>

boost::random::mt19937 Space::randomGenerator(std::time(0));

Space::Space(unsigned int amount, unsigned int size, bool rare)
{
    if(rare)
    {
        for(unsigned int i = 0;i<amount;++i)
        {
            /* produce points for square size 2*size*amount x 2*size*amount */
            int x = getRandomNumber(0,amount*size);
            int y = getRandomNumber(0,amount*size);
            Point randomPoint(x,y);
            points.push_back(randomPoint);
        }
    }
    else
    {
        int partChooser = getRandomNumber(0,3);
        Point offset;
        switch(partChooser)
        {
            case 0:
                offset.x = 0;
                offset.y = 0;
            break;
            case 1:
                offset.x = amount*size/2;
                offset.y = 0;
            break;
            case 2:
                offset.x = 0;
                offset.y = amount*size/2;
            break;
            case 3:
                offset.x = amount*size/2;
                offset.y = amount*size/2;
            break;
        }
        std::cout << "part: " << partChooser << std::endl;
        int notRareAmount = amount/4;
        int x = getRandomNumber(0,notRareAmount*size);
        for(unsigned int i = 0;i<notRareAmount;++i)
        {
            int y = getRandomNumber(0,notRareAmount*size);
            Point randomPoint(x,y);
            randomPoint = randomPoint + offset;
            points.push_back(randomPoint);
        }
        int restAmount = amount-notRareAmount;
        for(unsigned int i = 0;i<restAmount;++i)
        {
            x = getRandomNumber(notRareAmount*size,restAmount*size);
            int y = getRandomNumber(notRareAmount*size,restAmount*size);
            Point randomPoint(x,y);
            points.push_back(randomPoint);
        }
    }
}

Space::Space(const Space& spaceToCopy) : points(spaceToCopy.points)
{}

Space::Space(const PointVector& pointVector) : points(pointVector)
{}

PointPairVector Space::pointsNeighborhood(unsigned int d)
{
    std::sort(points.begin(),points.end()); // because of representation points in vector, we need sort them by x-cordinate, to be able to find d-pairs on line, after projection
    PointsContainer pc; // prepare X and Y vectors which are: points sorted by X coord and points sorted by Y coord
    pc.x = points;
    pc.y = points;
    std::sort(pc.x.begin(),pc.x.end(),pc.byX); // prepare points sorted by X coordinate
    std::sort(pc.y.begin(),pc.y.end(),pc.byY); // prepare points sorted by Y coordinate
    return pointsNeighborhood(pc,0,static_cast<int>(pc.size()),d);
}

PointPairVector Space::pointsNeighborhood(const Space::PointsContainer& pc, int start, int end, unsigned int d)
{
    if(end-start<=3)
    {
        return bruteNeighbors(pc.x,start,end,d);
    }
    Point median = pc.x[start+(end-start)/2];
    PointPairVector resultLeft = pointsNeighborhood(pc,start,start+(end-start)/2,d);
    PointPairVector resultRight = pointsNeighborhood(pc,start+(end-start)/2,end,d); // TODO: check if it should be +1 in START argument
    
    PointVector zone = leaveFromZone(pc.y,median,d); // vector of points within +-d zone from median
    PointPairVector zonePairs = pairsInZone(zone,d); // look throught vector and find d-pairs

    resultLeft.insert(resultRight.begin(),resultRight.end());
    resultLeft.insert(zonePairs.begin(),zonePairs.end());

    return resultLeft;
}

PointVector Space::leaveFromZone(const PointVector& vec, const Point& splitPoint, unsigned int d)
{
    PointVector result;
    PointVector::const_iterator iter = vec.begin();
    PointVector::const_iterator endIter = vec.end();
    for(;iter!=endIter;++iter)
    {
        if(iter->distanceXTo(splitPoint)<=d)
            result.push_back(*iter);
    }
    return result;
}

PointPairVector Space::pairsInZone(const PointVector& zone, unsigned int d)
{
    PointPairVector result;
    PointVector::const_iterator iter = zone.begin();
    PointVector::const_iterator endIter = zone.end();
    for(;iter!=endIter;++iter)
    {
        for(unsigned int i = 1;(iter+i)!=endIter;++i)
        {
            // optimization
            if(iter->distanceYTo(*(iter+i))>d) // if distance by Y is higher than d
            {
                if(iter->y < (iter+i)->y) // if current point is before second one
                    break; // there won't be more interesting points in d-neighborhood
                else
                    continue; // we could not reach interesting points yet
            }
            if(iter->distanceTo(*(iter+i))<=d) // if distance is lower than d
            {
                result.insert(PointPair(*iter,*(iter+i))); // add pair of d-neighbors
            }
        }
    }
    return result;
}

PointPairVector Space::bruteNeighbors(const PointVector& vec, unsigned int start, unsigned int end, unsigned int d) const
{
    PointPairVector result;
    PointVector::const_iterator i = vec.begin()+start;
    PointVector::const_iterator j = vec.begin()+start+1;
    PointVector::const_iterator endIter = (end>=vec.size()) ? vec.end() : vec.begin()+end+1;
    for(;i<endIter;++i)
    {
        for(j=i+1;j<endIter;++j)
        {
            if(j->distanceTo(*i)<=d)
            {
                result.insert(PointPair(*i,*j));
            }
        }
    }
    return result;
}

size_t Space::getPointsCount() const
{
    return points.size();
}

PointVector Space::getPointVector() const
{
    return points;
}

int Space::getRandomNumber(int start, int end) const
{
    boost::random::uniform_int_distribution<> dist(start, end);
    return dist(randomGenerator);
}

// GRAPH
AdjacencyGraph Space::buildIntersectionGraph(const PointPairVector& vec, unsigned int d) const
{
    return AdjacencyGraph(points,vec,d);
}

