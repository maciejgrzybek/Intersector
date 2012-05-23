#include "UI.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include <boost/lexical_cast.hpp>

#include "Figure.h"
#include "FigureFactory.h"
#include "AdjacencyGraph.h"
#include "Space.h"

enum FigureType { CIRCLE, SQUARE };

FigureType figureType;

FigureFactory* getFigureFactory()
{
    if(figureType == CIRCLE)
        return &(CircleFactory::getInstance());
    else
        return &(SquareFactory::getInstance());
}

PointVector calculate(Space& s, unsigned int d)
{
    AdjacencyGraph graph = s.buildIntersectionGraph(s.pointsNeighborhood(2*d),d); // create graph of intersecting circles of radius d
    graph.solve();
    return graph.getPointVector();
}

int main(int argc,char* argv[])
{
/*    Point pointA;
    Point pointB;
    pointA.x = 0;
    pointA.y = 0;
    pointB.x = 5;
    pointB.y = 5;
    std::vector<Figure*> vec;
    vec.push_back(new Square(pointA,2));
    vec.push_back(new Square(pointB,2));
    //std::cout << vec.at(0)->intersects(*vec.at(1)) << std::endl;
    std::vector<Point> v;
    for(int i = 0;i<atoi(argv[1]);++i)
        v.push_back(Point(i,i));

    Space s(v);
    Point::counter = 0;
    std::cout << Point::counter << std::endl;
    PointPairVector neighborhood = s.pointsNeighborhood(atoi(argv[2]));
    std::cout << Point::counter << std::endl;
    return 0;*/
    UI ui;
    PointVector input;
    PointVector result;
    unsigned int size = 1;
    if(argc < 5)
    {
        std::cout << "Usage: " << argv[0] << " figure size mode [parameters]" << std::endl;
        std::cout << "Where figure can be: circle or square." << std::endl;
        std::cout << "Where size can be integer number > 0." << std::endl;
        std::cout << "Where mode can be:" << std::endl;
        std::cout << "0 - read data from input stream in format: d x1,y1 x2,y2 x3,y3\n where xN,yN are cordinates of points, d is size of space for points to generate." << std::endl;
        std::cout << "1 - randomly generated data; parameters for this mode is size of problem to solve in format: n d, where n is number of points to generate, d is size of space for points to generate." << std::endl;
        std::cout << "2 - randomly generated data with time measurement. Parameters as above." << std::endl;
        return 1;
    }
    else
    {
        std::string figureTypeStr = argv[1];
        std::transform(figureTypeStr.begin(), figureTypeStr.end(),figureTypeStr.begin(), ::tolower);
        if(figureTypeStr == "square")
        {
            figureType = SQUARE;
        }
        else
        {
            figureType = CIRCLE;
        }
        try
        {
            size = boost::lexical_cast<unsigned int>(argv[2]);
        }
        catch(boost::bad_lexical_cast&)
        {
            std::cout << "Wrong size format." << std::endl;
            return 1;
        }
        if(argv[3] == "0")
        {
            std::cout << "You have chosen mode with reading data from input. After you put data, type return (new-line character)." << std::endl;
            std::cout << "Calculations will start as soon as you finish input." << std::endl;
            unsigned int d = 1;
            try
            {
                d = boost::lexical_cast<unsigned int>(argv[4]);
            }
            catch(boost::bad_lexical_cast&)
            {
                std::cout << "Wrong format for size of figure." << std::endl;
                return 1;
            }
            input = ui.getInputData();
            Space s(input);
            ui.showPoints(s.getPointVector());
            calculate(s,d);

        }
        else if((argv[3] == "1") || (argv[3] == "2"))
        {
            std::cout << "You have chosen random generation of data." << std::endl;
            unsigned int n = 1;
            unsigned int d = 1;
            bool rare = false;
            bool timeMeasurement = false;
            try
            {
                n = boost::lexical_cast<unsigned int>(argv[4]);
            }
            catch(boost::bad_lexical_cast&)
            {
                std::cout << "Wrong format for number of points." << std::endl;
                return 1;
            }
            try
            {
                d = boost::lexical_cast<unsigned int>(argv[5]);
            }
            catch(boost::bad_lexical_cast&)
            {
                std::cout << "Wrong format for size of space." << std::endl;
                return 1;
            }
            try
            {
                rare = boost::lexical_cast<bool>(argv[6]);
            }
            catch(boost::bad_lexical_cast&)
            {
                std::cout << "Accepting values for rare modifier are only: 0 or 1." << std::endl;
                return 1;
            }
            if(argv[3] == "2")
                timeMeasurement = true;

            clock_t startTime;
            clock_t afterGenerationTime;
            clock_t endTime;
            startTime = clock();
            Space s(n,d,rare);
            afterGenerationTime = clock();
            input = s.getPointVector();
            ui.showPoints(input); // in new thread
            result = calculate(s,d);
            endTime = clock();
            ui.showPoints(result);
            std::cout << "Generation time: " << afterGenerationTime - startTime << std::endl;
            std::cout << "Calculation time: " << endTime - afterGenerationTime << std::endl;
            std::cout << "Whole time: " << endTime - startTime << std::endl;
        }
    }
    ui.joinDrawerThread();
    return 0;
}
