#include "Picture.h"

#include "TemplateFunctions.h"

#include <iostream>

Picture& Picture::getInstance()
{
	static Picture theInstance;
	return theInstance;
}

//////////////////////////////////////////////////////////////

void Picture::addFigure(Figure* pObj)
{
	if (this->figuresCount == this->sizeOfFigures)
	{
		resize(this->figures, this->sizeOfFigures, 2 * this->sizeOfFigures + 2);

		this->sizeOfFigures = 2 * this->sizeOfFigures + 2;
	}

	this->figures[this->figuresCount++] = pObj;
}

void Picture::removeFigure(int index)
{
	if (index < 0 || index >= this->figuresCount)
		throw "Error! Wrong index!";

	if (2 * this->figuresCount == this->sizeOfFigures)
	{
		resize(this->figures, this->sizeOfFigures, (3 * this->figuresCount) / 2);

		this->sizeOfFigures = (3 * this->figuresCount) / 2;
	}

	swap(figures[index], figures[this->figuresCount - 1]);

	delete figures[this->figuresCount-- - 1];
}

void Picture::within(const Rectangle& obj)
{
	bool hasFigureIn = false;

	for (int i = 0; i < this->figuresCount; i++)
	{
		if (this->figures[i]->within(obj))
		{
			char* temp = this->figures[i]->getDataInConsoleFormat();

			std::cout << i + 1 << ". " << temp << '\n';

			delete[] temp;
		}
	}

	if (!hasFigureIn)
		std::cout << "There are no figure in this one!\n";
}

void Picture::within(const Circle& obj)
{
	bool hasFigureIn = false;

	for (int i = 0; i < this->figuresCount; i++)
	{
		if (this->figures[i]->within(obj))
		{
			hasFigureIn = true;

			char* temp = this->figures[i]->getDataInConsoleFormat();

			std::cout << i + 1 << ". " << temp << '\n';

			delete[] temp;
		}
	}

	if (!hasFigureIn)
		std::cout << "There are no figure in this one!\n";
}

void Picture::translate(const Point& obj, int index, bool allFigures)
{
	if (allFigures)
	{
		for (int i = 0; i < this->figuresCount; i++)
			this->figures[i]->translate(obj);
	}
	else
	{
		if (index < 0 || index >= this->figuresCount)
			throw "Error! Wrong index!";

		this->figures[index]->translate(obj);
	}
}

const Figure* Picture::getFigure(int index) const
{
	if (index < 0 || index >= this->figuresCount)
		throw "Error! Wrong index!";

	return this->figures[index];
}

int Picture::getFiguresCount() const
{
	return this->figuresCount;
}

void Picture::clean()
{
	for (int i = 0; i < this->figuresCount; i++)
		delete this->figures[i];

	if (this->figuresCount > 0)
		delete[] this->figures;

	this->figures = NULL;

	this->figuresCount = 0;
	this->sizeOfFigures = 0;
}

//////////////////////////////////////////////////////////////

Picture::Picture() :
figures(NULL),
figuresCount(0),
sizeOfFigures(0)
{

}

Picture::~Picture()
{
	this->clean();
}
