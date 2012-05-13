#include "Space.h"

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

size_t Space::getPointsCount() const
{
    return points.size();
}

int Space::getRandomNumber(int start, int end) const
{
    boost::random::uniform_int_distribution<> dist(start, end);
    return dist(randomGenerator);
}
