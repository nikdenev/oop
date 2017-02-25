#include "Circle.h"

#include "Helpers.h"
#include "TemplateFunctions.h"

#include <cstring>
#include <cmath>

Circle::Circle() :
center(0, 0),
radius(0),
fill(BLACK),				
stroke(NONE),
strokeWidth(1)
{

}

void Circle::readStringFromConsole(char* buffer)
{
	char** data;
	int count = 0;

	takeWordsFromString(buffer, data, count);

	if (count < 3)
		throw "Can't make circle without coordinates or radius!";

	if (count > 6)
		throw "Error! Too much attributes!";

	try
	{
		int x = stringToNumber(data[0]);

		this->center.setX(x);

		int y = stringToNumber(data[1]);

		this->center.setY(y);

		int radius = stringToNumber(data[2]);

		if (radius < 0)
			throw "Error! Radius must be a positive number!";

		this->radius = radius;

		if (count == 3)
			return;

		this->fill = stringToColor(data[3]);

		if (count == 4)
			return;

		this->stroke = stringToColor(data[4]);

		if (count == 5)
			return;

		int strokeWidth = stringToNumber(data[5]);

		if (strokeWidth < 0)
			throw "Error! Stroke-width must be positive number!";

		this->strokeWidth = strokeWidth;
	}
	catch (const char* what)
	{
		for (int i = 0; i < count; i++)
			delete[] data[i];

		delete[] data;

		throw;
	}

	for (int i = 0; i < count; i++)
		delete[] data[i];

	delete[] data;
}

void Circle::readStringFromSVG(char* buffer)
{
	char** data = NULL;
	int count = 0;

	takeWordsFromString(buffer, data, count);

	char* value;

	for (int i = 0; i < count; i++)
	{
		try
		{
			takeAttributeFromString(data[i], value);

			this->attributeTakesValue(data[i], value);
		}
		catch (const char* what)
		{
			char* somethingToThrow = new char[strlen(data[i]) + 1];

			strcpy(somethingToThrow, data[i]);

			for (int k = 0; k < i; k++)
				delete[] data[k];

			delete[] data;

			throw somethingToThrow;
		}

	}

	for (int k = 0; k < count; k++)
		delete[] data[k];

	delete[] data;
}

char* Circle::getDataInSVG_Format() const
{
	char data[256];

	data[0] = '\0';

	strcat(data, "circle ");

	const char* colorString;

	char numberString[16];

	numberToString(numberString, this->center.getX());

	strcat(data, "cx=\"");

	strcat(data, numberString);

	strcat(data, "\" ");
	

	numberToString(numberString, this->center.getY());

	strcat(data, "cy=\"");

	strcat(data, numberString);

	strcat(data, "\" ");
	


	numberToString(numberString, this->radius);

	strcat(data, "r=\"");

	strcat(data, numberString);

	strcat(data, "\" ");
	

	
	colorString = colorToString(this->fill);

	strcat(data, "fill=\"");

	strcat(data, colorString);

	strcat(data, "\" ");

	
	if (this->stroke != NONE)
	{
		colorString = colorToString(this->stroke);

		strcat(data, "stroke=\"");

		strcat(data, colorString);

		strcat(data, "\" ");
	}

	if (this->strokeWidth != 1)
	{
		char numberString[16];

		numberToString(numberString, this->strokeWidth);

		strcat(data, "stroke-width=\"");

		strcat(data, numberString);

		strcat(data, "\" ");
	}

	char* temp = new char[strlen(data) + 1];

	strcpy(temp, data);

	return temp;
}

char* Circle::getDataInConsoleFormat() const
{
	char data[128];

	data[0] = '\0';

	strcat(data, "circle ");

	char numberString[16];

	numberToString(numberString, this->center.getX());

	strcat(data, numberString);

	strcat(data, " ");

	numberToString(numberString, this->center.getY());

	strcat(data, numberString);

	strcat(data, " ");

	numberToString(numberString, this->radius);

	strcat(data, numberString);

	strcat(data, " ");

	strcat(data, colorToString(this->fill));

	strcat(data, " ");

	if (this->stroke != NONE)
	{
		strcat(data, colorToString(this->stroke));

		strcat(data, " ");
	}

	if (this->strokeWidth != 1)
	{
		numberToString(numberString, this->strokeWidth);

		strcat(data, numberString);

		strcat(data, " ");
	}

	char* temp = new char[strlen(data) + 1];

	strcpy(temp, data);

	return temp;
}

bool Circle::within(const Rectangle& obj) const
{
	Point P1(this->center.getX() + this->radius, this->center.getY());
	Point P2(this->center.getX(), this->center.getY() + this->radius);
	Point P3(this->center.getX() - this->radius, this->center.getY());
	Point P4(this->center.getX(), this->center.getY() - this->radius);

	return
		P1.within(obj) &&
		P2.within(obj) &&
		P3.within(obj) &&
		P4.within(obj);
}

bool Circle::within(const Circle& obj) const
{
	return sqrt((this->center.getX() - obj.center.getX())*(this->center.getX() - obj.center.getX()) +
		(this->center.getY() - obj.center.getY())*(this->center.getY() - obj.center.getY())) +
		this->radius <= obj.radius;
}

void Circle::translate(const Point& obj)
{
	this->center.translate(obj);
}

const char* Circle::getFiguresName() const
{
	return "circle";
}

const Point& Circle::getCenter() const
{
	return this->center;
}

int Circle::getRadius() const
{
	return radius;
}

//////////////////////////////////////////////////////////////

void Circle::attributeTakesValue(const char* attribute, const char* value)
{
	if (strcmp(attribute, "cx") == 0)
	{
		int x = stringToNumber(value);

		this->center.setX(x);
	}
	else if (strcmp(attribute, "cy") == 0)
	{
		int y = stringToNumber(value);

		this->center.setY(y);
	}
	else if (strcmp(attribute, "r") == 0)
	{
		int r = stringToNumber(value);

		if (r < 0)
			throw "Radius must bu positive number!";		

		this->radius = r;
	}
	else if (strcmp(attribute, "fill") == 0)
	{
		this->fill = stringToColor(value);
	}
	else if (strcmp(attribute, "stroke") == 0)
	{
		this->stroke = stringToColor(value);
	}
	else if (strcmp(attribute, "stroke-width") == 0)
	{
		int strokeWidth = stringToNumber(value);

		if (strokeWidth < 0)
			throw "stroke-width must bu positive numbers!";

		this->strokeWidth = strokeWidth;
	}
	else
	{
		throw "There is no such attribute!";
	}
}