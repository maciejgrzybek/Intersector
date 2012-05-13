#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SpaceSelection test
#include <boost/test/unit_test.hpp>

#include "../src/Space.h"

BOOST_AUTO_TEST_SUITE( s1 )

BOOST_AUTO_TEST_CASE( constructors_test ) 
{
    Space::PointVector vec01;
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

    Space space05(10); // should generate 10-rare points
    BOOST_CHECK_EQUAL(space05.getPointsCount(),10);
}

BOOST_AUTO_TEST_SUITE_END()
