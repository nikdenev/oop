#include "Line.h"

#include "Helpers.h"
#include "TemplateFunctions.h"

#include <cstring>

Line::Line() :
point1(0, 0),
point2(0, 0),
stroke(NONE),
strokeWidth(1)
{

}

void Line::readStringFromConsole(char* buffer)
{
	char** data;
	int count = 0;

	takeWordsFromString(buffer, data, count);

	if (count < 4)
		throw "Can't make line without coordinates!";

	if (count > 6)
		throw "Error! Too much attributes!";

	try
	{
		int x1 = stringToNumber(data[0]);

		this->point1.setX(x1);

		int y1 = stringToNumber(data[1]);

		this->point1.setY(y1);

		int x2 = stringToNumber(data[2]);

		this->point2.setX(x2);

		int y2 = stringToNumber(data[3]);
		
		this->point2.setY(y2);

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

void Line::readStringFromSVG(char* buffer)
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

char* Line::getDataInSVG_Format() const
{
	char data[256];

	data[0] = '\0';

	strcat(data, "line ");

	const char* colorString;

	char numberString[16];

	numberToString(numberString, this->point1.getX());

	strcat(data, "x1=\"");

	strcat(data, numberString);

	strcat(data, "\" ");


	numberToString(numberString, this->point1.getY());

	strcat(data, "y1=\"");

	strcat(data, numberString);

	strcat(data, "\" ");


	numberToString(numberString, this->point2.getX());

	strcat(data, "x2=\"");

	strcat(data, numberString);

	strcat(data, "\" ");


	numberToString(numberString, this->point2.getY());

	strcat(data, "y2=\"");

	strcat(data, numberString);

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

char* Line::getDataInConsoleFormat() const
{
	char data[128];

	data[0] = '\0';

	strcat(data, "line ");

	char numberString[16];

	numberToString(numberString, this->point1.getX());

	strcat(data, numberString);

	strcat(data, " ");

	numberToString(numberString, this->point1.getY());

	strcat(data, numberString);

	strcat(data, " ");



	numberToString(numberString, this->point2.getX());

	strcat(data, numberString);

	strcat(data, " ");

	numberToString(numberString, this->point2.getY());

	strcat(data, numberString);

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

bool Line::within(const Rectangle& obj) const
{
	return this->point1.within(obj) && this->point2.within(obj);
}

bool Line::within(const Circle& obj) const
{
	return this->point1.within(obj) && this->point2.within(obj);
}

void Line::translate(const Point& obj)
{
	this->point1.translate(obj);
	this->point2.translate(obj);
}

const char* Line::getFiguresName() const
{
	return "line";
}

const Point& Line::getPoint1() const
{
	return this->point1;
}

const Point& Line::getPoint2() const
{
	return this->point2;
}

//////////////////////////////////////////////////////////////

void Line::attributeTakesValue(const char* attribute, const char* value)
{
	if (strcmp(attribute, "x1") == 0)
	{
		int x = stringToNumber(value);

		this->point1.setX(x);
	}
	else if (strcmp(attribute, "y1") == 0)
	{
		int y = stringToNumber(value);

		this->point1.setY(y);
	}
	else if (strcmp(attribute, "x2") == 0)
	{
		int x = stringToNumber(value);

		this->point2.setX(x);
	}
	else if (strcmp(attribute, "y2") == 0)
	{
		int y = stringToNumber(value);

		this->point2.setY(y);
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

