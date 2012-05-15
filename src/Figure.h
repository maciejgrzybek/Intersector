#ifndef _FIGURE_H_
#define _FIGURE_H_

#include "FigureException.h"

#include <ostream>

class Figure;
class Square;
class Circle;

class IntersectVisitor
{
public:
    virtual bool visit(const Square&) const = 0;
    virtual bool visit(const Circle&) const = 0;
};

class SquareVisitor : public IntersectVisitor
{
public:
    SquareVisitor(const Square&);
    bool visit(const Square&) const; // for intersect(Square,Square)
    bool visit(const Circle&) const; // for intersect(Circle,Square)
private:
    const Square& square_;
};

class CircleVisitor : public IntersectVisitor
{
public:
    CircleVisitor(const Circle&);
    bool visit(const Square&) const; // for intersect(Square,Circle)
    bool visit(const Circle&) const; // for intersect(Circle,Circle)
private:
    const Circle& circle_;
};

struct Point
{
    Point();
    Point(int x, int y);
    int x;
    int y;
    double distanceTo(const Point&) const;
    Point operator-(const Point&) const;
    bool operator<(const Point&) const;
    bool operator==(const Point&) const;

    friend std::ostream& operator<<(std::ostream&,const Point&);
};

class Figure
{
public:
    Figure(const Point&);
    virtual bool accept(IntersectVisitor&) const = 0;
    /**
     * Method checks whether current figure intersect given one.
     * @param const Figure& Reference to figure to be intersected with.
     * @return bool true if intersects, false otherwise.
     */
    virtual bool intersects(const Figure&) const = 0;
    virtual double getDistance(const Figure&) const;
protected:
    Point centerPoint;
};

class Square : public Figure
{
public:
    Square(const Point&, unsigned int);
    bool accept(IntersectVisitor&) const;
    double getEdgeLength() const;

    /**
     * Method checks whether current figure intersect given one.
     * @param const Figure& Reference to figure to be intersected with.
     * @return bool true if intersects, false otherwise.
     */
    bool intersects(const Figure&) const;

    /**
     * Method returns distance between two squares basis on only X axis.
     * @param const Square& Reference to square to be compared with current.
     * @return double distance (subtraction of X-axis of center points).
     */
    virtual double getDistanceX(const Square&) const;

    /**
     * Method returns distance between two squares basis on only Y axis.
     * @param const Square& Reference to square to be compared with current.
     * @return double distance (subtraction of Y-axis of center points).
     */
    virtual double getDistanceY(const Square&) const;
private:
    double edgeLength;
};

class Circle : public Figure
{
public:
    Circle(const Point&, unsigned int);
    bool accept(IntersectVisitor&) const;
    double getRadius() const;
    /**
     * Method checks whether current figure intersect given one.
     * @param const Figure& Reference to figure to be intersected with.
     * @return bool true if intersects, false otherwise.
     */
    virtual bool intersects(const Figure&) const;
private:
    double radius_;
};

#endif
