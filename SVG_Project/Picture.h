#pragma once

#include "Figure.h"

class Picture
{
public:
	static Picture& getInstance();

public:
	void addFigure(Figure* pObj);
	void removeFigure(int index);

	void within(const Rectangle& obj);
	void within(const Circle& obj);
	void translate(const Point& obj, int index, bool allFigures);

	const Figure* getFigure(int index) const;
	int getFiguresCount() const;

	void clean();

private:
	Picture();
	Picture(const Picture& obj);
	void operator= (const Picture& obj);
	~Picture();

private:
	Figure** figures;
	int figuresCount;
	int sizeOfFigures;
};

