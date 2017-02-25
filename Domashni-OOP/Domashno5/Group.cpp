#include "Group.h"

Group::Group() :
scalar(1), figures(NULL), figuresCount(0)
{

}

Group::~Group()
{
	for (int i = 0; i < this->figuresCount; i++)
		delete this->figures[i];

	delete[] this->figures;
}

bool Group::pointBelongs(const Point& point) const
{
	bool isFound = false;

	int i = 0;

	for (; i < this->figuresCount && !isFound; ++i)
	{
		this->figures[i]->scalarMulti(this->scalar);

		this->figures[i]->translate(this->vector);

		if (figures[i]->pointBelongs(point))
			isFound = true;
	}

	Point oppositeVector(this->vector);

	oppositeVector.scalarMulti(-1.0);

	for (int j = 0; j < i; j++)
	{
		this->figures[j]->translate(oppositeVector);

		this->figures[j]->scalarMulti(1.0/this->scalar);
	}

	return isFound;
}

void Group::print(std::ostream& stream) const
{
	for (int i = 0; i < this->figuresCount; ++i)
	{
		this->figures[i]->scalarMulti(this->scalar);

		this->figures[i]->translate(this->vector);
	}

	stream << "group in ";
	this->vector.print(stream);
	stream << this->scalar << '\n';

	for (int i = 0; i < this->figuresCount; i++)
		this->figures[i]->print(stream);

	stream << "group out\n";

	Point oppositeVector(this->vector);

	oppositeVector.scalarMulti(-1.0);

	for (int j = 0; j < this->figuresCount; j++)
	{
		this->figures[j]->translate(oppositeVector);

		this->figures[j]->scalarMulti(1.0 / this->scalar);
	}

	if (!stream)
		throw "Error! Problem with writing in file!";
}

void Group::read(std::istream& stream)
{
	this->vector.read(stream);

	stream >> this->scalar;
	if (!stream)
		stream.clear();

	stream.ignore(1000, '\n');

	if (abs(scalar) < EPS)
		throw "Error! Scalar mustn't be 0!";

	char buffer[8];

	Figure* tempFigure;

	for (int i = 0; true; ++i)
	{
		stream.getline(buffer, 8, ' ');

		if (stream.eof())
			throw "Error! Problem with the file!";

		try
		{
			if (strcmp(buffer, "sphere") == 0)
				tempFigure = new Sphere;
			else if (strcmp(buffer, "cuboid") == 0)
				tempFigure = new Cuboid;
			else if (strcmp(buffer, "pyramid") == 0)
				tempFigure = new Pyramid;
			else
			{
				if (strcmp(buffer, "group") == 0)
				{
					char tempBuffer[5];

					stream.getline(tempBuffer, 5);

					if (strcmp(tempBuffer, "out") == 0)
						break;
					else
						throw "Error! You can't put group in group!";
				}
				else
					throw "Error! There is no such figure!";
			}
		}
		catch (std::bad_alloc& ba)
		{
			for (int j = 0; j < i; j++)
				delete this->figures[j];

			throw;
		}

		try
		{
			resize(figures, this->figuresCount, this->figuresCount + 1);
		}
		catch (...)
		{
			for (int j = 0; j <= i; j++)
				delete this->figures[j];

			throw;
		}

		this->figures[i] = tempFigure;

		++this->figuresCount;

		try
		{
			this->figures[i]->read(stream);
		}
		catch (const char* what)
		{
			for (int j = 0; j <= i; j++)
				delete this->figures[j];

			throw;
		}
	}
}

void Group::translate(const Point& vector)
{
	for (int i = 0; i < this->figuresCount; ++i)
		this->figures[i]->translate(vector);
}

void Group::scalarMulti(double scalar)
{
	for (int i = 0; i < this->figuresCount; ++i)
		this->figures[i]->scalarMulti(scalar);
}

//////////////////////////////////////////////////////////////


void resize(Figure**& figures, int oldSize, int newSize)
{
	if (newSize < 0 || oldSize < 0)
		throw "Error! Unvalid size!";

	Figure** newFigures = NULL;

	if (newSize != 0)
	{
		newFigures = new Figure*[newSize];

		for (int i = 0; i < oldSize && i < newSize; i++)
			newFigures[i] = figures[i];
	}

	delete[] figures;

	figures = newFigures;
}