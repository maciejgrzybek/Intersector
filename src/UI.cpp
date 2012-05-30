#include "UI.h"

#include <GL/glut.h>
#include <iostream>
#include <cmath>

#include <boost/function.hpp>

template<class InputIterator>
  InputIterator my_find ( InputIterator first, InputIterator last, const Point& value )
  {
    for ( ;first!=last; first++) if ( first->x == value.x && first->y == value.y ) break;
    return first;
  }

UI::UI()
{
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (0.0, 200.0, 0.0, 150.0);
}

UI::~UI()
{
    // FIXME implement this
}

PointVector UI::getInputData()
{
    Point point;
    PointVector vec;
    int i = 0;
    while(!std::cin.eof())
    {
        ++i;
        std::cin >> point;
        vec.push_back(point);
    }
    if(i>0)
        vec.erase(--vec.end());
    return vec;
}

void UI::joinDrawerThread()
{
    drawerThread.join();
}

void UI::showPoints(PointVector& points,PointVector& vec,unsigned int size,FigureType figure)
{
    Drawer d(points,vec,size,figure);
    //drawerThread = boost::thread(d);
    d();
}

UI::Drawer::Drawer(PointVector& points, PointVector& vec, unsigned int size, FigureType figure) : points_(points),vec_(vec),size_(size),figure_(figure)
{}

const float DEG2RAD = 3.14159/180;

void drawCircle(unsigned int x, unsigned int y, unsigned int radius)
{

   glPushMatrix();

   glTranslatef((float)(x),(float)(y),0.0);
    glBegin(GL_LINE_LOOP);
    for (int i=0; i < 360; ++i)
    {
        float degInRad = i*DEG2RAD;
        glVertex2f(cos(degInRad)*radius,sin(degInRad)*radius);
    }
    glEnd();

   glPopMatrix();
}

void drawSquare(unsigned int x, unsigned int y, unsigned int length)
{
    glPushMatrix();

    glTranslatef((float)(x-length/2.0),(float)(y-length/2.0),0.0);
    glBegin(GL_LINE_LOOP);
        glVertex2i(0,0);
        glVertex2i(length,0);
        glVertex2i(length,length);
        glVertex2i(0,length);
    glEnd();

    glPopMatrix();
}

void UI::Drawer::drawFigure(unsigned int x, unsigned int y, unsigned int a)
{
    switch(figure_)
    {
        case SQUARE:
            return drawSquare(x,y,a);
        break;
        case CIRCLE:
            return drawCircle(x,y,a);
        break;
        default:
            exit(1);
    }
}

void UI::Drawer::display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    //draw two points
    PointVector::const_iterator iter = vec_.begin();
    PointVector::const_iterator endIter = vec_.end();
    glColor3f(0.0, 1.0, 0.0);
    for(;iter != endIter;++iter)
    {
        // FIXME
        // scale points to screen resolution here
        //std::cout << "vec::rysuje " << *iter << " ";
        int x = iter->x; // FIXME should be scaled and translated to right position
        int y = iter->y; // FIXME
        drawFigure(x,y,size_);

        PointVector::iterator findIter = my_find(points_.begin(),points_.end(),*iter);
        if(findIter != points_.end())
            points_.erase(findIter);
    }
    iter = points_.begin();
    endIter = points_.end();
    glColor3f(0.0, 0.0, 1.0);
    for(;iter != endIter;++iter)
    {
        //std::cout << "Points::rysuje " << *iter << " ";
        int x = iter->x; // FIXME should be scaled and translated to right position
        int y = iter->y; // FIXME
        //glVertex2i(x,y);
        drawFigure(x,y,size_);
        //std::cout << "n " << *iter << std::endl; // FIXME remove this
        //glFlush(); // FIXME remove this
        //sleep(1);
    }

    glFlush();
}

boost::function<void ()> displayF;

extern "C"
void displayFunc(void)
{
    displayF();
}

void UI::Drawer::operator()()
{
    // FIXME do some drawings 
//    boost::function<void ()> displayFunc = boost::bind(displayFunc,vec_);
    displayF = boost::bind(&UI::Drawer::display,*this);

    glutDisplayFunc(displayFunc);
    glutMainLoop();
}
