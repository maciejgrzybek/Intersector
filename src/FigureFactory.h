#ifndef FIGURE_FACTORY_H
#define FIGURE_FACTORY_H

#include "Figure.h"

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<Figure> FigurePtr;

class FigureFactory : boost::noncopyable
{
public:
    virtual FigurePtr getFigure(const Point&,int) = 0;
protected:
    FigureFactory()
    {}
};

class CircleFactory : public FigureFactory
{
public:
    static CircleFactory& getInstance()
    {
        static CircleFactory instance;
        return instance;
    }

    FigurePtr getFigure(const Point& point, int size)
    {
        return FigurePtr(new Circle(point,size));
    }
private:
    CircleFactory()
    {}
};

class SquareFactory : public FigureFactory
{
public:
    static SquareFactory& getInstance()
    {
        static SquareFactory instance;
        return instance;
    }

    FigurePtr getFigure(const Point& point, int size)
    {
        return FigurePtr(new Square(point,size));
    }
private:
    SquareFactory()
    {}
};

#endif
