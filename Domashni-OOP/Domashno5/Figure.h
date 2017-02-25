#pragma once

#include "Point.h"

#include <iostream>
#include <cstring>

static const double EPS = 1E-6;

class Figure
{
public:
	virtual ~Figure();

	virtual bool pointBelongs(const Point& point) const = 0;
	virtual void print(std::ostream& stream) const = 0;
	virtual void read(std::istream& stream) = 0;
	virtual void translate(const Point& vector) = 0;
	virtual void scalarMulti(double scalar) = 0;
};

