#ifndef _SPACE_H_
#define _SPACE_H_

#include <vector>
#include <cstring>
#include <ctime>

#include <boost/random/mersenne_twister.hpp>

#include "Figure.h"

typedef std::vector<Point> PointVector;
typedef std::pair<Point,Point> PointPair;
typedef std::vector<PointPair> PointPairVector;

class Space
{
public:

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
     * @return PointPairVector vector of pairs of points meeting condition.
     */
    PointPairVector pointsNeighborhood(unsigned int);

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

    /**
     * Helper method for pointsNeighborhood from public section.
     * @param PointVector::const_iterator Iterator to vector at the begining of range to calculate.
     * @param PointVector::const_iterator Iterator to vector at the end of range to calculate.
     * @param unsigned int d - number of distance units of neighborhood to calculate points of.
     * @return PointPairVector Vector of pair of points meeting condition.
     */
    PointPairVector pointsNeighborhood(PointVector::const_iterator,PointVector::const_iterator,unsigned int);

    /**
     * Helper method for pointsNeighborhood.
     * Method does projection of points in zone of d-distance from split line onto it and checks whether there is any d-pair (points with distance less-equal d on line (after projection) from different sides of split line (left and right) and distance before projection should not be higher than d).
     * @param unsigned int d - distance
     * @param PointVector::const_iterator iterator to split line in PointVector.
     * @return PointPairVector vector of pairs of points being in d-neighborhood
     */
    PointPairVector dPairsOnLine(unsigned int,PointVector::const_iterator);
};

#endif
