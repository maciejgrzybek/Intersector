#ifndef _SPACE_H_
#define _SPACE_H_

#include <vector>
#include <set>
#include <cstring>
#include <ctime>

#include <boost/random/mersenne_twister.hpp>
#include <boost/unordered_set.hpp>

#include "Figure.h"

typedef std::vector<Point> PointVector;
struct PointPair
{
    Point first;
    Point second;
    PointPair(Point f, Point s) : first(f), second(s)
    {}
    bool operator==(const PointPair& pp) const
    {
        return (((this->first == pp.first) && (this->second == pp.second)) || ((this->first == pp.second) && (this->second == pp.first)));
    }
};
//typedef std::pair<Point,Point> PointPair;

struct PointHash
        : std::unary_function<Point, std::size_t>
{
    std::size_t operator()(const Point& p) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, p.x);
        boost::hash_combine(seed, p.y);
        return seed;
        /*std::hash<int> hashf;
        size_t resultHash;
        resultHash = hashf(p.x) ^ hashf(p.y);
        return resultHash;*/
    }
};

struct PointPairHash
        : std::unary_function<PointPair, std::size_t>
{
    /*std::size_t operator()(const PointPair& p) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, p.first);
        boost::hash_combine(seed, p.second);
        return seed;
    }*/
    size_t operator()(const PointPair& pp) const
    {
        PointHash hashf;
        size_t resultHash;
        Point p1 = pp.first;
        Point p2 = pp.second;
        resultHash = hashf(p1) ^ hashf(p2);
        return resultHash;
    }
};

namespace std
{

template <>
struct equal_to<PointPair>
{
    bool operator()(const PointPair& pp,const PointPair& pp2) const
    {
        return ((pp.first == pp2.first && pp.second == pp2.second) || (pp.first == pp2.second && pp.second == pp2.first));
    }
};

} // namespace std

typedef boost::unordered_set<PointPair,PointPairHash,std::equal_to<PointPair>> PointPairVector;

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

    struct dPoint : public Point
    {
        bool isLeft;
        int originalX;
        dPoint(const Point&);
        bool operator<(const dPoint&) const;
    };

    typedef std::pair<dPoint,dPoint> PointdPair;
    typedef std::vector<PointdPair> PointdPairVector;

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
     * @param PointVector::iterator Iterator to vector at the begining of range to calculate.
     * @param PointVector::iterator Iterator to vector at the end of range to calculate.
     * @param unsigned int d - number of distance units of neighborhood to calculate points of.
     * @return PointPairVector Vector of pair of points meeting condition.
     */
    PointPairVector pointsNeighborhood(PointVector::iterator,PointVector::iterator,unsigned int);

    /**
     * Helper method for pointsNeighborhood.
     * Method does projection of points in zone of d-distance from split line onto it and checks whether there is any d-pair (points with distance less-equal d on line (after projection) from different sides of split line (left and right) and distance before projection should not be higher than d).
     * @param unsigned int d - distance
     * @param PointVector::const_iterator iterator to split line in PointVector.
     * @return PointPairVector vector of pairs of points being in d-neighborhood
     */
    PointPairVector dPairsOnLine(unsigned int,PointVector::const_iterator);

    PointPairVector bruteNeighbors(PointVector::iterator,PointVector::iterator,unsigned int) const;

    PointdPairVector bruteNeighbors(std::vector<dPoint>::iterator start, std::vector<dPoint>::iterator end, unsigned int d) const;
};

#endif
