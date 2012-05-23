#ifndef _SPACE_H_
#define _SPACE_H_

#include <vector>
#include <set>
#include <cstring>
#include <ctime>

#include <boost/random/mersenne_twister.hpp>
#include <boost/unordered_set.hpp>

#include "Figure.h"

class AdjacencyGraph;

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
     * @param unsigned int Size of space for points
     * @param bool Rare set or not
     */
    Space(unsigned int,unsigned int = 10,bool = true);

    /**
     * Copying constructor.
     * Copies point vector.
     */
    Space(const Space&);

    /**
     * Creates space from given PointVector.
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

    /**
     * Method returns PointVector stored in Space.
     * @return PointVector points stored in Space.
     */
    PointVector getPointVector() const;

    AdjacencyGraph buildIntersectionGraph(const PointPairVector&, unsigned int) const;
private:

    class PointsContainer
    {
    public:
        PointVector x;
        PointVector y;

        static bool byX(const Point& a, const Point& b)
        {
            if(a.x<b.x)
            {
                return true;
            }
            else if(a.x==b.x)
            {
                return a.y<b.y;
            }
            else
            {
                return false;
            }
        }
        static bool byY(const Point& a, const Point& b)
        {
            if(a.y<b.y)
            {
                return true;
            }
            else if(a.y==b.y)
            {
                return a.x<b.x;
            }
            else
            {
                return false;
            }
        }

        std::size_t size() const
        {
            assert(x.size() == y.size());
            return x.size();
        }
    };

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
     * @param PointVector::iterator Iterator to vector at the begining of range to calculate.
     * @param PointVector::iterator Iterator to vector at the end of range to calculate.
     * @param unsigned int d - number of distance units of neighborhood to calculate points of.
     * @return PointPairVector Vector of pair of points meeting condition.
     */
    PointPairVector pointsNeighborhood(const PointsContainer&,int,int,unsigned int);

PointVector leaveFromZone(const PointVector& vec, const Point& splitPoint, unsigned int d);


PointPairVector pairsInZone(const PointVector& zone, unsigned int d);

    PointPairVector bruteNeighbors(const PointVector&, unsigned int, unsigned int, unsigned int) const;
};

#endif
