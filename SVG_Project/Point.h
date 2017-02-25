#pragma once

#include "Figure.h"

class Point
{
public:
	Point(int x=0, int y=0);

	void translate(const Point& obj);

	void setX(int x);
	void setY(int y);

	void setPoint(int x, int y);

	bool within(const Rectangle& obj) const;
	bool within(const Circle& obj) const;

	int getX() const;
	int getY() const;

private:
	int x;
	int y;
};

