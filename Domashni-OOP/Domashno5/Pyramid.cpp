#include "Pyramid.h"

#include "Plane.h"

Pyramid::Pyramid() :
P2(1, 0, 0),
P3(0, 1, 0),
P4(0, 0 ,1)
{

}

Pyramid::Pyramid(const Pyramid& obj) :
P1(obj.P1),
P2(obj.P2),
P3(obj.P3),
P4(obj.P4)
{

}

Pyramid& Pyramid::operator= (const Pyramid& obj)
{
	if (this == &obj)
		return *this;

	this->P1 = obj.P1;
	this->P2 = obj.P2;
	this->P3 = obj.P3;
	this->P4 = obj.P4;

	return *this;
}

Pyramid::~Pyramid()
{

}

bool Pyramid::pointBelongs(const Point& point) const
{
	Plane plane123(this->P1, this->P2, this->P3);
	Plane plane124(this->P1, this->P2, this->P4);
	Plane plane134(this->P1, this->P3, this->P4);
	Plane plane234(this->P2, this->P3, this->P4);

	return
		plane123.disposel(this->P4)*plane123.disposel(point) >= 0 &&
		plane124.disposel(this->P3)*plane124.disposel(point) >= 0 &&
		plane134.disposel(this->P2)*plane134.disposel(point) >= 0 &&
		plane234.disposel(this->P1)*plane234.disposel(point) >= 0;
}

void Pyramid::print(std::ostream& stream) const
{
	stream << "pyramid ";
	this->P1.print(stream);
	this->P2.print(stream);
	this->P3.print(stream);
	this->P4.print(stream);
	stream << '\n';

	if (!stream)
		throw "Error! Problem with writing in file!";
}

void Pyramid::read(std::istream& stream)
{
	this->P1.read(stream);
	this->P2.read(stream);
	this->P3.read(stream);
	this->P4.read(stream);

	stream.ignore(1000, '\n');

	if (!this->isPyramid())
		throw "Can't make pyramid with this points!";
}

void Pyramid::translate(const Point& vector)
{
	this->P1.translate(vector);
	this->P2.translate(vector);
	this->P3.translate(vector);
	this->P4.translate(vector);
}

void Pyramid::scalarMulti(double scalar)
{
	this->P1.scalarMulti(scalar);
	this->P2.scalarMulti(scalar);
	this->P3.scalarMulti(scalar);
	this->P4.scalarMulti(scalar);
}

//////////////////////////////////////////////////////////////

bool Pyramid::isPyramid() const
{
	Plane temp;

	try
	{
		temp.setPlane(this->P1, this->P2, this->P3);
		temp.setPlane(this->P1, this->P2, this->P4);
		temp.setPlane(this->P1, this->P3, this->P4);
		temp.setPlane(this->P2, this->P3, this->P4);
	}
	catch (const char* x)
	{
		return false;
	}

	return true;
}