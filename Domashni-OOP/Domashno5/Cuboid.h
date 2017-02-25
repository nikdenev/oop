#pragma once

#include "Figure.h"

class Cuboid :
	public Figure
{
public:
	Cuboid();
	Cuboid(const Cuboid& obj);
	Cuboid& operator= (const Cuboid& obj);

	virtual ~Cuboid();

	virtual bool pointBelongs(const Point& point) const;
	virtual void print(std::ostream& stream) const;
	virtual void read(std::istream& stream);
	virtual void translate(const Point& vector);
	virtual void scalarMulti(double scalar);

private:
	bool isCuboid() const;

private:
	Point P1;
	Point P2;
};

