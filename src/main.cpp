#include "UI.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include <GL/gl.h>
#include <GL/glut.h>
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

struct CalcThread
{
    CalcThread(const AdjacencyGraph& adj, PointVector* vec, unsigned int size) : graph(adj), result(vec)
    {}
    void operator()()
    {
        std::cout << "punkty:" << std::endl;
        PointVector points = graph.getPointVector();
        for(auto& p : points)
        {
            std::cout << p << " ";
        }
        std::cout << std::endl;
        graph.solve();
        std::cout << "Problem solved." << std::endl;
        for(auto& p : *result)
        {
            std::cout << p << " ";
        }
        std::cout << std::endl;
        *result = graph.getPointVector(); // ugly solvation of drawing problem
        // FIXME: should be right mutexed!!!
        for(auto& p : *result)
        {
            std::cout << p << " ";
        }
        std::cout << std::endl;
        std::cout << "Reseted canvas feeder" << std::endl;
    }
    AdjacencyGraph graph;
    PointVector* result;
};

CalcThread calculate(Space& s, unsigned int d)
{
//    AdjacencyGraph graph = s.buildIntersectionGraph(s.pointsNeighborhood(2*d),d); // create graph of intersecting circles of radius d
//    CalcThread ct(graph);
//    boost::thread thread = boost::thread(ct);
//    return ct;
}

int main(int argc,char* argv[])
{
    /* glut initialization before UI instantiation */
	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (800, 600);
	glutInitWindowPosition (400, 100);
	glutCreateWindow ("Intersector");

    UI ui;
    PointVector input;
    PointVector result;
    unsigned int size = 1;
    if(argc < 3)
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

        if(boost::lexical_cast<std::string>(argv[3]) == "0")
        {
            std::cout << "You have chosen mode with reading data from input. After you put data, type return (new-line character)." << std::endl;
            std::cout << "Calculations will start as soon as you finish input." << std::endl;
            input = ui.getInputData();
            std::cout << "wczytano wszystkie dane" << std::endl;
            Space s(input);

            PointVector toDraw = s.getPointVector();
            for(auto& p : input)
            {
                std::cout << p << " ";
            }
            std::cout << std::endl;
     int d = size;
            AdjacencyGraph graph = s.buildIntersectionGraph(s.pointsNeighborhood(2*d),d); // create graph of intersecting circles of radius d
            PointVector resultPoints;
            CalcThread ct(graph,&resultPoints,size);
            std::cout << "Calculating..." << std::endl;
            //boost::thread thread = boost::thread(ct);
            ct();
            std::cout << "Rendering..." << std::endl;
            ui.showPoints(toDraw,resultPoints,size);
            std::cout << "Finished rendering." << std::endl; // never appears
        }
        else if((boost::lexical_cast<std::string>(argv[3]) == "1") || (argv[3] == "2")) // FIXME (lexical_cast on argv[3] == "2")
        {
            std::cout << "You have chosen random generation of data." << std::endl;
            unsigned int n = 1;
            unsigned int d = 1;
            bool rare = false;
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

            clock_t startTime;
            clock_t afterGenerationTime;
            clock_t endTime;
            PointVector resultPoints;
            startTime = clock();
            Space s(n,d,rare);
            afterGenerationTime = clock();
            input = s.getPointVector();
            for(auto& p : input)
            {
                std::cout << p << " ";
            }
            std::cout << std::endl;
            AdjacencyGraph graph = s.buildIntersectionGraph(s.pointsNeighborhood(2*d),d); // create graph of intersecting circles of radius d
            CalcThread ct(graph,&resultPoints,size);
            //boost::thread thread = boost::thread(ct);
            ct();
            endTime = clock();
            std::cout << "Generation time: " << afterGenerationTime - startTime << std::endl;
            std::cout << "Calculation time: " << endTime - afterGenerationTime << std::endl;
            std::cout << "Whole time: " << endTime - startTime << std::endl;
            if(boost::lexical_cast<std::string>(argv[3]) == "1")
                ui.showPoints(input,resultPoints,size);
        }
    }
//    ui.joinDrawerThread();

    //sleep(3);
    return 0;
}
