#ifndef UI_H
#define UI_H

#include "Space.h"

#include <boost/thread.hpp>

class UI
{
public:
    UI();
    ~UI();
    PointVector getInputData();
    void showPoints(PointVector&,PointVector&,unsigned int,FigureType);
    void joinDrawerThread();
private:
    struct Drawer
    {
        Drawer(PointVector&,PointVector&,unsigned int,FigureType figure);
        void drawFigure(unsigned int,unsigned int,unsigned int);
        void display();
        void operator()();

        PointVector& vec_;
        PointVector& points_;
        unsigned int size_;
        FigureType figure_;
    };
    boost::thread drawerThread;
};

#endif
