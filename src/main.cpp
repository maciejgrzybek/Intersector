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
#include "UI.h"

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
    void operator()(int m = 0)
    {
        PointVector points = graph.getPointVector();
        graph.solve((m == 1));
        *result = graph.getPointVector(); // ugly solvation of drawing problem
    }
    AdjacencyGraph graph;
    PointVector* result;
};

static bool state = false;

CalcThread calculate(Space& s, unsigned int d)
{
//    AdjacencyGraph graph = s.buildIntersectionGraph(s.pointsNeighborhood(2*d),d); // create graph of intersecting circles of radius d
//    CalcThread ct(graph);
//    boost::thread thread = boost::thread(ct);
//    return ct;
}

int median(int n)
{
    if(n%2 == 0)
    {
        return (n/2 + n/2 + 1)/2;
    }
    else
    {
        return n/2;
    }
}

double log_2(double n)
{
    return (double)(log(n)/log(2));
}

int main(int argc,char* argv[])
{
    /* glut initialization before UI instantiation */

    PointVector input;
    PointVector result;

    unsigned int mode;

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
        mode = boost::lexical_cast<unsigned int>(argv[3]);
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

        if(mode == 0)
        {
            std::cout << "You have chosen mode with reading data from input. After you put data, type return (new-line character)." << std::endl;
            std::cout << "Calculations will start as soon as you finish input." << std::endl;

	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (800, 600);
	glutInitWindowPosition (400, 100);
	glutCreateWindow ("Intersector");
            UI ui;

            input = ui.getInputData();
            if(input.size() == 0)
            {
                std::cout << "No points inputed." << std::endl;
                return 1;
            }
            Space s(input,figureType);

            PointVector toDraw = s.getPointVector();
     int d = size;
            AdjacencyGraph graph = s.buildIntersectionGraph(s.pointsNeighborhood(2*d),d); // create graph of intersecting circles of radius d
            PointVector resultPoints;
            CalcThread ct(graph,&resultPoints,size);
            //boost::thread thread = boost::thread(ct);
            ct();

            ui.showPoints(toDraw,resultPoints,size,figureType);
        }
        else if(mode == 1)
        {
            if(argc<6)
            {
                std::cout << "Not enought parameters!" << std::endl;
                std::cout << argv[0] << "figure size " << mode << " number_of_figures size_of_space 1/0" << std::endl;
                std::cout << "Where 1/0 is boolean which indicates whather set is rare or not." << std::endl;
                return 1;
            }
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
            Space s(n,d,rare,figureType);
            afterGenerationTime = clock();

            PointVector toDraw = s.getPointVector();
            startTime = clock();
            AdjacencyGraph graph = s.buildIntersectionGraph(s.pointsNeighborhood(2*size),size); // create graph of intersecting circles of radius d
            afterGenerationTime = clock();
            CalcThread ct(graph,&resultPoints,size);
            ct(mode);
            endTime = clock();
            if(mode == 1)
            {

	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (800, 600);
	glutInitWindowPosition (400, 100);
	glutCreateWindow ("Intersector");
                UI ui;
                ui.showPoints(toDraw,resultPoints,size,figureType);
            }
            printf("Generation time: %f\n",(double)((afterGenerationTime-(double)startTime)/CLOCKS_PER_SEC));
            printf("Calculation time: %f\n",(double)((endTime-(double)afterGenerationTime)/CLOCKS_PER_SEC));
            printf("Overall: %f\n",(double)((endTime-(double)startTime)/CLOCKS_PER_SEC));
        }
        else if(mode == 2)
        {
            if(argc<7)
            {
                std::cout << "Not enought parameters!" << std::endl;
                std::cout << argv[0] << "figure size " << mode << " number_of_figures_min size_of_space_min number_of_figures_max steps" << std::endl;
            }
            std::cout << "You have chosen random generation of data in measurement mode." << std::endl;
            unsigned int n_min = 1;
            unsigned int d_min = 1;
            unsigned int n_max = 1;
            unsigned int steps = 1;
            bool rare = false;
            try
            {
                n_min= boost::lexical_cast<unsigned int>(argv[4]);
            }
            catch(boost::bad_lexical_cast&)
            {
                std::cout << "Wrong format for number of points." << std::endl;
                return 1;
            }
            try
            {
                n_max= boost::lexical_cast<unsigned int>(argv[5]);
            }
            catch(boost::bad_lexical_cast&)
            {
                std::cout << "Wrong format for number of points." << std::endl;
                return 1;
            }
            if(n_max<n_min)
            {
                std::cout << "n_max has to be more than n_min" << std::endl;
                return 1;
            }
            try
            {
                d_min = boost::lexical_cast<unsigned int>(argv[6]);
            }
            catch(boost::bad_lexical_cast&)
            {
                std::cout << "Wrong format for size of space." << std::endl;
                return 1;
            }
            try
            {
                steps = boost::lexical_cast<unsigned int>(argv[7]);
            }
            catch(boost::bad_lexical_cast&)
            {
                std::cout << "Wrong format for number of steps" << std::endl;
                return 1;
            }
            std::vector<std::pair<int,int> > ns;
            std::vector<double> times;
            int amountDiff = (n_max-n_min)/steps;
            int sizeDiff = sqrt(2);//(d_max-d_min)/steps;
            for(unsigned int j = 0;j<2;++j)
            {
                rare = !rare;
                for(unsigned int i = 0;i<steps;++i)
                {

                    clock_t startTime;
                    clock_t endTime;
                    PointVector resultPoints;
                    startTime = clock();
                    std::cout << "tworze przestrzen: " << n_min + i*amountDiff << " " << d_min +i*sizeDiff << std::endl;
                    Space s(n_min + i*amountDiff,d_min+i*sizeDiff,rare,figureType);
                    std::cout << "stworzylem" << std::endl;

                startTime = clock();
                AdjacencyGraph graph = s.buildIntersectionGraph(s.pointsNeighborhood(2*size),size); // create graph of intersecting circles of radius d
                CalcThread ct(graph,&resultPoints,size);
                ct(mode);
                    endTime = clock();
                    ns.push_back( std::make_pair(n_min + i*amountDiff, d_min + i*sizeDiff) );
                    times.push_back( (double)((endTime-(double)startTime)/CLOCKS_PER_SEC) );
                    //std::cout << ( (double)((endTime-(double)startTime)/CLOCKS_PER_SEC) ) << std::endl;
                }
            }
            bool r = false;
            for(unsigned int j = 0;j<2;++j)
            {
                r = !r;
                for(unsigned int i = 0;i<steps;++i)
                {
                    long double Tn_median = ns[j*steps + median(steps)].first*ns[j*steps + median(steps)].first*log_2(ns[j*steps + median(steps)].first)*log_2(ns[j*steps + median(steps)].first);
//                    std::cout << "Tn_median = " << Tn_median << std::endl;
                    long double Tn = ns[j*steps + i].first*ns[j*steps + i].first*log_2(ns[j*steps + i].first)*log_2(ns[j*steps + i].first);
                    long double Tn_median_tm = Tn_median/times[j*steps + median(steps)];
                    long double tn_Tn = times[j*steps + i]/Tn;
//                    std::cout << "ns["<<(j*steps+i)<<"] = " << ns[j*steps + i].first << std::endl;
                    std::cout << "ns^2 = " << (ns[j*steps + i].first * ns[j*steps + i].first) << std::endl;
//                    std::cout << "Tn = " << Tn << std::endl;
//                    long double q = times[j*steps + i]*Tn_median/(Tn*times[j*steps + median(steps)]);
                    long double q = Tn_median_tm*tn_Tn;
                    std::cout << ns[j*steps + i].first << "*" << ns[j*steps + i].second << " " << times[j*steps + i] << " " << q << " " << r << std::endl;
                }
                std::cout << "------" << std::endl;
            }
        }
    }
//    ui.joinDrawerThread();

    //sleep(3);
    return 0;
}
