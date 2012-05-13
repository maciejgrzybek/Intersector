#ifndef _SPACE_H_
#define _SPACE_H_

#include <vector>
#include <cstring>
#include <ctime>

#include <boost/random/mersenne_twister.hpp>

#include "Figure.h"

class Space
{
public:
    typedef std::vector<Point> PointVector;

    /**
     * Constructor generates points with pseudo-random positions of given amount with rare or not (as given with second parameter) coverage.
     * @param unsigned int Number of points to generate
     * @param bool Rare set or not
     */
    Space(unsigned int,bool = true);

    /**
     * Copying constructor.
     * Copies point vector.
     */
    Space(const Space&);

    /**
     * Creates space from given PointVector
     */
    Space(const PointVector&);

    /**
     * Method retrieves points from neighborhood of d-distance units.
     * @param unsigned int d - number of distance units of neighborhood to calculate points of.
     * @return PointVector vector of points meeting condition.
     */
    PointVector pointsNeighborhood(const unsigned int);

    /**
     * Method returns number of points in PointVector;
     * @return size_t number of points in vector.
     */
    size_t getPointsCount() const;
private:
    PointVector points;

    /**
     * Random number generator.
     */
    static boost::random::mt19937 randomGenerator;

    /**
     * Returns random number from given range.
     * @param int begining of range (including this value)
     * @param int end of range (including this value)
     * @return random number from range
     */
    int getRandomNumber(int,int) const;
};

#endif
