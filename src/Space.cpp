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
    std::sort(points.begin(),points.end()); // O(nlogn)

    Point median = MedianSelect<Point>::selectKElement(points.size()/2,points); // find the division point
    PointVector::const_iterator start = points.begin();
    PointVector::const_iterator middle = std::find(points.begin(),points.end(),median);
    if(middle == points.end()) // if element not found, something is probably wrong with selectKElement() method
        throw std::exception(); // TODO change to proper exception
    PointVector::const_iterator end = points.end();

    PointPairVector leftResult;
    PointPairVector rightResult;
    PointPairVector additionalPoints;
    leftResult = pointsNeighborhood(start,middle,d);
    rightResult = pointsNeighborhood(middle,end,d);

    leftResult.insert(leftResult.end(),rightResult.begin(),rightResult.end()); // append rightResult to the leftResult.
    additionalPoints = dPairsOnLine(d,middle);
    leftResult.insert(leftResult.end(),additionalPoints.begin(),additionalPoints.end()); // append additional points (projected onto split line)
    return leftResult; // and return as a whole result
}

PointPairVector Space::pointsNeighborhood(PointVector::const_iterator start, PointVector::const_iterator end, unsigned int d)
{
    
    // FIXME implement this
}

PointPairVector Space::dPairsOnLine(unsigned int d, PointVector::const_iterator iter)
{
    // FIXME implement this
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
