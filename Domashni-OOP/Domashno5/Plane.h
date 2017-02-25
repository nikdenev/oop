#pragma once

#include "Point.h"

// a1*x + a2*y + a3*z + D = 0

class Plane
{
public:
	Plane();
	Plane(const Point& P1, const Point& P2, const Point& P3);
	//kompilatora generira copy ,operator=  i destruktor

	// < 0 above
	// > 0 under
	// = 0 on
	int disposel(const Point& P);

	void setPlane(const Point& P1, const Point& P2, const Point& P3);

private:
	double x;
	double y;
	double z;
	double distance;
};

