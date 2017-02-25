#pragma once

#include "Figure.h"

class Sphere :
	public Figure
{
public:
	Sphere();
	Sphere(const Sphere& obj);
	Sphere& operator= (const Sphere& obj);

	virtual ~Sphere();

	virtual bool pointBelongs(const Point& point) const;
	virtual void print(std::ostream& stream) const;
	virtual void read(std::istream& stream);
	virtual void translate(const Point& vector);
	virtual void scalarMulti(double scalar);

private:
	Point center;
	double radius;
};

