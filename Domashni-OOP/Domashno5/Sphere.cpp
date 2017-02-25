#include "Sphere.h"

Sphere::Sphere() :
radius(1)
{

}

Sphere::Sphere(const Sphere& obj) :
center(obj.center), radius(obj.radius)
{

}

Sphere& Sphere::operator= (const Sphere& obj)
{
	if (this == &obj)
		return *this;

	this->center = obj.center;

	this->radius = obj.radius;

	return *this;
}

Sphere::~Sphere()
{

}

bool Sphere::pointBelongs(const Point& point) const
{
	double dist = distance(this->center, point);

	return dist <= radius;
}

void Sphere::print(std::ostream& stream) const
{
	stream << "sphere ";
	this->center.print(stream);
	stream << ' ' << this->radius << '\n';

	if (!stream)
		throw "Error! Problem with writing in file!";
}

void Sphere::read(std::istream& stream)
{
	Point tempCenter;
	double tempR = 0;

	tempCenter.read(stream);

	stream >> tempR;

	if (!stream)
		stream.clear();

	stream.ignore(1000, '\n');

	if (tempR < EPS)
		throw "Error! Radius must be > 0!";

	this->center = tempCenter;
	this->radius = tempR;
}

void Sphere::translate(const Point& vector)
{
	this->center.translate(vector);
}

void Sphere::scalarMulti(double scalar)
{
	this->center.scalarMulti(scalar);

	this->radius *= ((scalar > 0) ? scalar : - scalar);
}
