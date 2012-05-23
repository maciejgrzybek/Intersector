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
    void showPoints(const PointVector&);
    void joinDrawerThread();
private:
    struct Drawer
    {
        Drawer(const PointVector&);
        void operator()();

        const PointVector& vec_;
    };
    boost::thread drawerThread;
};

#endif
