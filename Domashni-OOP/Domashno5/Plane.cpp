#include "Plane.h"

#include "Figure.h"

Plane::Plane() :
x(1), y(1), z(1), distance(0)
{

}

Plane::Plane(const Point& P1, const Point& P2, const Point& P3)
{
	this->setPlane(P1, P2, P3);
}

// < 0 above
// > 0 under
// = 0 on
int Plane::disposel(const Point& P)
{
	double temp =
		this->x*P.getX() +
		this->y*P.getY() +
		this->z*P.getZ() +
		distance;

	if (abs(temp) < EPS)
		return 0;

	return (temp > 0) ? 10 : -10;
}

void Plane::setPlane(const Point& P1, const Point& P2, const Point& P3)
{
	Point vec1Temp = vector(P1, P2);
	Point vec2Temp = vector(P1, P3);

	Point vector = vecMulti(vec1Temp, vec2Temp);

	if (
		abs(vector.getX()) < EPS ||
		abs(vector.getY()) < EPS ||
		abs(vector.getZ()) < EPS
		)
		throw "Plane can't be made by concurrent points!";

	this->x = vector.getX();
	this->y = vector.getY();
	this->z = vector.getZ();

	this->distance = -scalarMulti(vector, P1);
}
