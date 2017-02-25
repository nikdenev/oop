#include "Cuboid.h"

Cuboid::Cuboid() :
P2(1, 1, 1)
{

}

Cuboid::Cuboid(const Cuboid& obj) :
P1(obj.P1), P2(obj.P2)
{

}

Cuboid& Cuboid::operator= (const Cuboid& obj)
{
	if (this == &obj)
		return *this;

	this->P1 = obj.P1;
	this->P2 = obj.P2;

	return *this;
}

Cuboid::~Cuboid()
{

}

bool Cuboid::pointBelongs(const Point& point) const
{
	return
		point.getX() >= ((P1.getX() < P2.getX()) ? P1.getX() : P2.getX()) &&
		point.getX() <= ((P1.getX() > P2.getX()) ? P1.getX() : P2.getX()) &&

		point.getY() >= ((P1.getY() < P2.getY()) ? P1.getY() : P2.getY()) &&
		point.getY() <= ((P1.getY() > P2.getY()) ? P1.getY() : P2.getY()) &&

		point.getZ() >= ((P1.getZ() < P2.getZ()) ? P1.getZ() : P2.getZ()) &&
		point.getZ() <= ((P1.getZ() > P2.getZ()) ? P1.getZ() : P2.getZ());
}

void Cuboid::print(std::ostream& stream) const
{
	stream << "cuboid ";
	this->P1.print(stream);
	this->P2.print(stream);
	stream << '\n';

	if (!stream)
		throw "Error! Problem with writing in file!";
}

void Cuboid::read(std::istream& stream)
{
	this->P1.read(stream);
	this->P2.read(stream);

	stream.ignore(1000, '\n');

	if (!this->isCuboid())
		throw "Error! The two point must form a cuboid!";
}

void Cuboid::translate(const Point& vector)
{
	this->P1.translate(vector);
	this->P2.translate(vector);
}

void Cuboid::scalarMulti(double scalar)
{
	this->P1.scalarMulti(scalar);
	this->P2.scalarMulti(scalar);
}

//////////////////////////////////////////////////////////////

bool Cuboid::isCuboid() const
{
	return !(
		abs(P1.getX() - P2.getX()) < EPS ||
		abs(P1.getY() - P2.getY()) < EPS ||
		abs(P1.getZ() - P2.getZ()) < EPS
		);
}