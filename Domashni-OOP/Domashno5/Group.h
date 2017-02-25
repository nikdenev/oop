#pragma once

#include "Figure.h"
#include "Cuboid.h"
#include "Pyramid.h"
#include "Sphere.h"


class Group :
	public Figure
{
private:
	Group(const Group& obj);
	void operator= (const Group& obj);

public:
	Group();

	virtual ~Group();

	virtual bool pointBelongs(const Point& point) const;
	virtual void print(std::ostream& stream) const;
	virtual void read(std::istream& stream);
	virtual void translate(const Point& vector);
	virtual void scalarMulti(double scalar);

private:
	Point vector;
	double scalar;
	Figure** figures;
	int figuresCount;
};

//////////////////////////////////////////////////////////////

void resize(Figure**& figures, int oldSize, int newSize);

//////////////////////////////////////////////////////////////
