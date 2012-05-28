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
    void showPoints(PointVector&,PointVector&,unsigned int);
    void joinDrawerThread();
private:
    struct Drawer
    {
        Drawer(PointVector&,PointVector&,unsigned int);
        void display();
        void operator()();

        PointVector& vec_;
        PointVector& points_;
        unsigned int size_;
    };
    boost::thread drawerThread;
};

#endif
