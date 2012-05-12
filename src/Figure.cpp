#include "Figure.h"
#include <cmath>

SquareVisitor::SquareVisitor(const Square& square) : square_(square)
{}

CircleVisitor::CircleVisitor(const Circle& circle) : circle_(circle)
{}

bool SquareVisitor::visit(const Square& square) const
{
    double distanceX = square_.getDistanceX(square);
    double distanceY = square_.getDistanceY(square);
    double allowedDistance = square.getEdgeLength()/2 + square_.getEdgeLength()/2;
    if((distanceX > allowedDistance) || (distanceY > allowedDistance))
    {
        return false;
    }
    return true;
}

bool SquareVisitor::visit(const Circle&) const
{
    throw IntersectionNotSupported();
}

bool CircleVisitor::visit(const Circle& circle) const
{
    double distance = circle_.getDistance(circle);
    double allowedDistance = circle_.getRadius()/2 + circle.getRadius();
    if(distance > allowedDistance)
    {
        return false;
    }
    return true;
}

bool CircleVisitor::visit(const Square&) const
{
    throw IntersectionNotSupported();
}

Figure::Figure(const Point& point) : centerPoint(point)
{}

double Figure::getDistance(const Figure& figure) const
{
    Point subPoint = figure.centerPoint - this->centerPoint;
    return sqrt(pow(subPoint.x,2) + pow(subPoint.y,2));
}

Square::Square(const Point& center, unsigned int length) : Figure(center),edgeLength(length)
{}

Circle::Circle(const Point& center, unsigned int radius) : Figure(center),radius_(radius)
{}

double Square::getDistanceX(const Square& square) const
{
    return std::abs(this->centerPoint.x - square.centerPoint.x);
}

double Square::getDistanceY(const Square& square) const
{
    return std::abs(this->centerPoint.y - square.centerPoint.y);
}


bool Square::accept(IntersectVisitor& v) const
{
    return v.visit(*this);
}

bool Square::intersects(const Figure& figure) const
{
    SquareVisitor squareVisitor(*this);
    return figure.accept(squareVisitor);
}

double Square::getEdgeLength() const
{
    return edgeLength;
}

double Circle::getRadius() const
{
    return radius_;
}

bool Circle::accept(IntersectVisitor& v) const
{
    return v.visit(*this);
}

bool Circle::intersects(const Figure& figure) const
{
    CircleVisitor circleVisitor(*this);
    return figure.accept(circleVisitor);
}

Point Point::operator-(const Point& point) const
{
    Point result;
    result.x = this->x - point.x;
    result.y = this->y - point.y;
    return result;
}

bool Point::operator<(const Point& point) const
{
    return this->x < point.x;
}

bool Point::operator==(const Point& point) const
{
    return this->x == point.x;
}
