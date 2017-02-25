#pragma once

#include "Figure.h"

class Pyramid :
	public Figure
{
public:
	Pyramid();
	Pyramid(const Pyramid& obj);
	Pyramid& operator= (const Pyramid& obj);

	virtual ~Pyramid();
	
	virtual bool pointBelongs(const Point& point) const;
	virtual void print(std::ostream& stream) const;
	virtual void read(std::istream& stream);
	virtual void translate(const Point& vector);
	virtual void scalarMulti(double scalar);

private:
	bool isPyramid() const;

private:
	Point P1;
	Point P2;
	Point P3;
	Point P4;
};

