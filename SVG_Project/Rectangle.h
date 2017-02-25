#pragma once

#include "Figure.h"

#include "Point.h"

class Rectangle :
	public Figure
{
public:

	Rectangle();

	virtual void readStringFromConsole(char* buffer);
	virtual void readStringFromSVG(char* buffer);
	virtual char* getDataInSVG_Format() const;
	virtual char* getDataInConsoleFormat() const;
	virtual bool within(const Rectangle& obj) const;
	virtual bool within(const Circle& obj) const;
	virtual void translate(const Point& obj);
	virtual const char* getFiguresName() const;

	const Point& getPoint() const;
	int getWidth() const;
	int getHeight() const;

private:

	void attributeTakesValue(const char* attribute, const char* value);

private:
	Point point;

	int width;
	int height;

	Color fill;

	int rx;
	int ry;

	Color stroke;

	int strokeWidth;
};

