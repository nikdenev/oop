#include "Point.h"

#include "Rectangle.h"
#include "Circle.h"

Point::Point(int x, int y) :
x(x), y(y)
{

}

void Point::translate(const Point& obj)
{
	this->x += obj.getX();
	this->y += obj.getY();
}

void Point::setX(int x)
{
	this->x = x;
}
void Point::setY(int y)
{
	this->y = y;
}

void Point::setPoint(int x, int y)
{
	this->x = x;
	this->y = y;
}

bool Point::within(const Rectangle& obj) const
{
	Point rectanglePoint = obj.getPoint();

	return
		rectanglePoint.getX() <= this->x &&
		rectanglePoint.getY() <= this->y &&

		rectanglePoint.getX() + obj.getWidth() >= this->x &&
		rectanglePoint.getY() + obj.getHeight() >= this->y;
}

bool Point::within(const Circle& obj) const
{
	Point p = obj.getCenter();

	return (p.getX() - this->x)*(p.getX() - this->x) + (p.getY() - this->y)*(p.getY() - this->y) <=
		obj.getRadius()*obj.getRadius();
}

int Point::getX() const
{
	return this->x;
}

int Point::getY() const
{
	return this->y;
}