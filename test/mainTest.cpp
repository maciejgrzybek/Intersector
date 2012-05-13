#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SpaceSelection test
#include <boost/test/unit_test.hpp>

#include "../src/Space.h"
#include "../src/MedianSelect.hpp"

BOOST_AUTO_TEST_SUITE( Space01 )

BOOST_AUTO_TEST_CASE( constructors_test ) 
{
    PointVector vec01;
    Point p01;
    Point p02(1,1);
    Point p03(2,2);
    vec01.push_back(p01);
    vec01.push_back(p02);
    vec01.push_back(p03);

    Space space01(vec01);
    BOOST_CHECK_EQUAL(space01.getPointsCount(),3);

    Space space02(space01);
    BOOST_CHECK_EQUAL(space02.getPointsCount(),space01.getPointsCount());

    Space *space03 = new Space(vec01);
    Space space04(*space03);
    delete space03;
    BOOST_CHECK_EQUAL(space04.getPointsCount(),3); // check for cutting-off protection (copying constructor should perform deep copy, not shallow one)

    Space space05(10); // should generate rare set of 10 points
    BOOST_CHECK_EQUAL(space05.getPointsCount(),10);
}

BOOST_AUTO_TEST_CASE( neighborhood_test )
{
    PointVector vec01;
    Point p01;
    Point p02(1,1);
    Point p03(2,2);
    vec01.push_back(p01);
    vec01.push_back(p02);
    vec01.push_back(p03);
    Space space01(vec01);
    PointPairVector neighborhood_01 = space01.pointsNeighborhood(1);
    // what happens when vector is empty?
    PointPair pp01(p01,p02);
    PointPair pp01r(p02,p01);
    PointPair pp02(p02,p03);
    PointPair pp02r(p03,p02);
    PointPairVector::const_iterator begin = neighborhood_01.begin();
    PointPairVector::const_iterator end = neighborhood_01.end();
    BOOST_CHECK( (std::find(begin,end,pp01) != end) ^ (std::find(begin,end,pp01r) != end) ); // there should be found as d-neighborhood pair of p01 and p02, does not matter in what order
    BOOST_CHECK( (std::find(begin,end,pp02) != end) ^ (std::find(begin,end,pp02r) != end) ); // the same with p02 and p03
    BOOST_CHECK_EQUAL(neighborhood_01.size(),2); // there should be only 2 pairs found, so if two above tests pass and this fails, that means there are found more points than should be
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( MedianSelect01 )

BOOST_AUTO_TEST_CASE( selection_test )
{
    // int part
    std::vector<int> vec01;
    vec01.push_back(18);
    vec01.push_back(2);
    vec01.push_back(4);
    vec01.push_back(12);
    vec01.push_back(28);
    
    int resultTab_01[vec01.size()];

    /* get k-th elements from 0 to count of elements */
    for(unsigned int i = 0;i<vec01.size();++i)
    {
        resultTab_01[i] = MedianSelect<int>::selectKElement(i,vec01);
    }

    /* sort whole vector */
    std::sort(vec01.begin(),vec01.end());

    /* and compare each value with MedianSelect'ed one */
    for(unsigned int i = 0;i<vec01.size();++i)
    {
        BOOST_CHECK_EQUAL(resultTab_01[i],vec01[i]);
    }

    // double part
    std::vector<double> vec02;
    vec02.push_back(3.06);
    vec02.push_back(1.02);
    vec02.push_back(10.01);
    vec02.push_back(52.0);
    vec02.push_back(0.0);
    double resultTab_02[vec02.size()];

    /* get k-th elements from 0 to count of elements */
    for(unsigned int i = 0;i<vec02.size();++i)
    {
        resultTab_02[i] = MedianSelect<double>::selectKElement(i,vec02);
    }

    /* sort whole vector */
    std::sort(vec02.begin(),vec02.end());

    /* and compare each value with MedianSelect'ed one */
    for(unsigned int i = 0;i<vec02.size();++i)
    {
        BOOST_CHECK_EQUAL(resultTab_02[i],vec02[i]);
    }
}

BOOST_AUTO_TEST_SUITE_END()


