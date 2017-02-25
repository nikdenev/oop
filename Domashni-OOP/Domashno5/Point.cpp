#include "Point.h"
#include "Figure.h"

#include <cmath>

Point::Point() :
x(0), y(0), z(0)
{

}

Point::Point(double x, double y, double z) :
x(x), y(y), z(z)
{

}

Point::~Point()
{

}

Point::Point(const Point& obj) :
x(obj.x), y(obj.y), z(obj.z)
{

}

Point& Point::operator= (const Point& obj)
{
	this->setPoint(obj.x, obj.y, obj.z);

	return *this;
}

void Point::setPoint(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Point::translate(const Point& vector)
{
	this->x += vector.x;
	this->y += vector.y;
	this->z += vector.z;
}

void Point::scalarMulti(double scalar)
{
	if (abs(scalar) < EPS)
		throw "Error! Unvalid scalar!";

	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
}

double Point::getX() const
{
	return this->x;
}

double Point::getY() const
{
	return this->y;
}

double Point::getZ() const
{
	return this->z;
}

void Point::read(std::istream& stream)
{
	double x = 0, y = 0, z = 0;

	stream >> x;

	if (!stream)
		stream.clear();

	stream >> y;

	if (!stream)
		stream.clear();

	stream >> z;

	if (!stream)
		stream.clear();

	this->x = x;
	this->y = y;
	this->z = z;
}

void Point::print(std::ostream& stream) const
{
	stream  << this->x << ' '
			<< this->y << ' '
			<< this->z << ' ';

	if (!stream)
		throw "Error! Problem with writing in file!";
}

//////////////////////////////////////////////////////////////

double distance(const Point& p1, const Point& p2)
{
	return sqrt(
		(p1.getX() - p2.getX())*(p1.getX() - p2.getX()) +
		(p1.getY() - p2.getY())*(p1.getY() - p2.getY()) +
		(p1.getZ() - p2.getZ())*(p1.getZ() - p2.getZ()));
}

double scalarMulti(const Point& vec1, const Point& vec2)
{
	return vec1.getX()*vec2.getX() + vec1.getY()*vec2.getY() + vec1.getZ()*vec2.getZ();
}

Point vecMulti(const Point& vec1, const Point& vec2)
{
	return
		Point(
		vec1.getY() * vec2.getZ() - vec1.getZ() * vec2.getY(),
		vec1.getZ() * vec2.getX() - vec1.getX() * vec2.getZ(),
		vec1.getX() * vec2.getY() - vec1.getY() * vec2.getX()
		);
}

Point vector(const Point& A, const Point& B)
{
	return Point(B.getX() - A.getX(), B.getY() - A.getY(), B.getZ() - A.getZ());
}

//////////////////////////////////////////////////////////////