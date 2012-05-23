#include "UI.h"

#include <iostream>

UI::UI()
{
    // FIXME implement this
}

UI::~UI()
{
    // FIXME implement this
}

PointVector UI::getInputData()
{
    Point point;
    PointVector vec;
    while(std::cin >> point)
    {
        vec.push_back(point);
    }
    return vec;
}

void UI::joinDrawerThread()
{
    drawerThread.join();
}

void UI::showPoints(const PointVector& vec)
{
    Drawer d(vec);
    drawerThread = boost::thread(d);
}

UI::Drawer::Drawer(const PointVector& vec) : vec_(vec)
{}

void UI::Drawer::operator()()
{
   // FIXME do some drawings 
}
