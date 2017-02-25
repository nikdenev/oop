#pragma once

#include "Figure.h"

#include "Point.h"

class Line :
	public Figure
{
public:
	Line();

	virtual void readStringFromConsole(char* buffer);
	virtual void readStringFromSVG(char* buffer);
	virtual char* getDataInSVG_Format() const;
	virtual char* getDataInConsoleFormat() const;
	virtual bool within(const Rectangle& obj) const;
	virtual bool within(const Circle& obj) const;
	virtual void translate(const Point& obj);
	virtual const char* getFiguresName() const;

	const Point& getPoint1() const;
	const Point& getPoint2() const;

private:

	void attributeTakesValue(const char* attribute, const char* value);
	
private:
	Point point1;
	Point point2;

	Color stroke;

	int strokeWidth;
};

