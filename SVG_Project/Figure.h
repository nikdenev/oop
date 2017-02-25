#pragma once

enum Color
{
	BLACK,
	NONE,
	RED,
	WHITE,
	BLUE,
	GREEN,
	YELLOW,
	ORANGE,
	COLORS_COUNT
};


class Rectangle;
class Circle;
class Point;

class Figure
{
public:
	virtual ~Figure() {}

	virtual void readStringFromConsole(char* str) = 0;
	virtual void readStringFromSVG(char* str) = 0;
	virtual char* getDataInSVG_Format() const = 0;
	virtual char* getDataInConsoleFormat() const = 0;
	virtual bool within(const Rectangle& obj) const = 0;
	virtual bool within(const Circle& obj) const = 0;
	virtual void translate(const Point& obj) = 0;
	virtual const char* getFiguresName() const = 0;
};

