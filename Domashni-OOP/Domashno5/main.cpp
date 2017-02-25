#include "Figure.h"
#include "Group.h"

#include <fstream>

void read(std::istream& stream, Figure**& figures, int &figuresCount)
{
	Figure* tempFigure;

	char buffer[8];

	for (int i = 0; true; ++i)
	{
		stream.getline(buffer, 8, ' ');

		if (stream.eof())
			break;

		try
		{
			if (strcmp(buffer, "sphere") == 0)
				tempFigure = new Sphere;
			else if (strcmp(buffer, "cuboid") == 0)
				tempFigure = new Cuboid;
			else if (strcmp(buffer, "pyramid") == 0)
				tempFigure = new Pyramid;
			else if (strcmp(buffer, "group") == 0)
			{
				char tempBuffer[5];

				stream.getline(tempBuffer, 5, ' ');

				if (strcmp(tempBuffer, "in") == 0)
					tempFigure = new Group;
				else
				{
					stream.ignore(1000, '\n');

					continue;
				}
			}
			else
			{
				stream.ignore(1000, '\n');

				continue;
			}
		}
		catch (std::bad_alloc& ba)
		{
			for (int j = 0; j < i; j++)
				delete figures[j];

			throw;
		}

		try
		{
			resize(figures, figuresCount, figuresCount + 1);
		}
		catch (...)
		{
			delete tempFigure;

			throw;
		}

		figures[i] = tempFigure;

		++figuresCount;

		figures[i]->read(stream);
		
	}
}

void printFiguresWithPoint(Figure** figures, int figuresCount, const Point& point)
{
	for (int i = 0; i < figuresCount; i++)
	{
		if (figures[i]->pointBelongs(point))
			figures[i]->print(std::cout);
	}
}

void printFigure(Figure** figures, int figuresCount, int index)
{
	if (index < 0 || index >= figuresCount)
		throw "Error! Unvalid Index!";

	figures[index]->print(std::cout);
}

int main()
{
	std::ifstream myFile("test.txt");
	if (!myFile)
	{
		std::cerr << "Can't open the file!\n";

		return 1;
	}

	Figure** figures = NULL;

	int figuresCount = 0;

	try
	{
		read(myFile, figures, figuresCount);
	}
	catch (const char* what)
	{
		std::cerr << what << '\n';
	}
	catch (std::bad_alloc& ba)
	{
		std::cerr << ba.what() << '\n';
	}

	Point point(1.0, 1.0, 1.0);

	printFiguresWithPoint(figures, figuresCount, point);

	for (int i = 0; i < figuresCount; i++)
		delete figures[i];

	delete[] figures;

	return 0;
}