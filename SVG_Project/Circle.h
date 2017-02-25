#pragma once

#include "Figure.h"

#include "Point.h"

class Circle :
	public Figure
{
public:
	Circle();

	virtual void readStringFromConsole(char* buffer);
	virtual void readStringFromSVG(char* buffer);
	virtual char* getDataInSVG_Format() const;
	virtual char* getDataInConsoleFormat() const;
	virtual bool within(const Rectangle& obj) const;
	virtual bool within(const Circle& obj) const;
	virtual void translate(const Point& obj);
	virtual const char* getFiguresName() const;

	const Point& getCenter() const;
	int getRadius() const;

private:

	void attributeTakesValue(const char* attribute, const char* value);

private:
	Point center;
	int radius;

	Color fill;

	Color stroke;

	int strokeWidth;
};

