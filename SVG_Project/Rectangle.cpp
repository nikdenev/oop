#include "Rectangle.h"

#include "Helpers.h"

#include <cstring>

Rectangle::Rectangle() :
point(0, 0),
width(0),
height(0),
fill(BLACK),
rx(0),
ry(0),
stroke(NONE),
strokeWidth(1)
{

}

void Rectangle::readStringFromConsole(char* buffer)
{
	char** data;
	int count = 0;

	takeWordsFromString(buffer, data, count);

	if (count <4)
		throw "Can't make rectangle without coordinates or width or height!";

	if (count > 9)
		throw "Error! Too much attributes!";

	try
	{
		int x = stringToNumber(data[0]);

		this->point.setX(x);

		int y = stringToNumber(data[1]);

		this->point.setY(y);

		int width = stringToNumber(data[2]);

		if (width < 0)
			throw "Error! Width can't be < 0!";

		this->width = width;

		int height = stringToNumber(data[3]);

		if (height < 0)
			throw "Error! Height can't be < 0!";

		this->height = height;

		if (count == 4)
			return;

		this->fill = stringToColor(data[4]);

		if (count == 5)
			return;

		int rx = stringToNumber(data[5]);

		if (rx < 0)
			throw "Error! rx must be > 0!";

		this->rx = rx;

		if (count == 6)
			return;

		int ry = stringToNumber(data[6]);

		if (ry < 0)
			throw "Error! ry must be > 0!";

		this->ry = ry;

		if (count == 7)
			return;

		this->stroke = stringToColor(data[7]);

		if (count == 8)
			return;

		int strokeWidth = stringToNumber(data[6]);

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

void Rectangle::readStringFromSVG(char* buffer)
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

char* Rectangle::getDataInSVG_Format() const
{
	char data[256];

	data[0] = '\0';

	strcat(data, "rect ");

	const char* colorString;

	char numberString[16];


	numberToString(numberString, this->point.getX());

	strcat(data, "x=\"");

	strcat(data, numberString);

	strcat(data, "\" ");
	


	numberToString(numberString, this->point.getY());

	strcat(data, "y=\"");

	strcat(data, numberString);

	strcat(data, "\" ");
	

	numberToString(numberString, this->width);

	strcat(data, "width=\"");

	strcat(data, numberString);

	strcat(data, "\" ");
	


	numberToString(numberString, this->height);

	strcat(data, "height=\"");

	strcat(data, numberString);

	strcat(data, "\" ");
	


	colorString = colorToString(this->fill);

	strcat(data, "fill=\"");

	strcat(data, colorString);

	strcat(data, "\" ");
	


	if (this->rx != 0)
	{
		char numberString[16];

		numberToString(numberString, this->rx);

		strcat(data, "rx=\"");

		strcat(data, numberString);

		strcat(data, "\" ");
	}

	if (this->ry != 0)
	{
		char numberString[16];

		numberToString(numberString, this->ry);

		strcat(data, "ry=\"");

		strcat(data, numberString);

		strcat(data, "\" ");
	}

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

char* Rectangle::getDataInConsoleFormat() const
{
	char data[128];

	data[0] = '\0';

	strcat(data, "rectangle ");

	char numberString[16];

	numberToString(numberString, this->point.getX());

	strcat(data, numberString);

	strcat(data, " ");

	numberToString(numberString, this->point.getY());

	strcat(data, numberString);

	strcat(data, " ");

	numberToString(numberString, this->width);

	strcat(data, numberString);

	strcat(data, " ");

	numberToString(numberString, this->height);

	strcat(data, numberString);

	strcat(data, " ");

	strcat(data, colorToString(this->fill));

	strcat(data, " ");

	if (this->rx != 0)
	{
		numberToString(numberString, this->rx);

		strcat(data, numberString);

		strcat(data, " ");
	}

	if (this->ry != 0)
	{
		numberToString(numberString, this->ry);

		strcat(data, numberString);

		strcat(data, " ");
	}

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

bool Rectangle::within(const Rectangle& obj) const
{
	return
		obj.point.getX() <= this->point.getX() &&
		obj.point.getY() <= this->point.getY() &&

		obj.point.getX() + obj.getWidth() >= this->point.getX() + this->width &&
		obj.point.getY() + obj.getHeight() >= this->point.getY() + this->height;
}

bool Rectangle::within(const Circle& obj) const
{
	Point P2(this->point.getX() + this->width, this->point.getY());
	Point P3(this->point.getX(), this->point.getY() + this->height);
	Point P4(this->point.getX() + this->width, this->point.getY() + this->height);

	return
		this->point.within(obj) &&
		P2.within(obj) &&
		P3.within(obj) &&
		P4.within(obj);
}

void Rectangle::translate(const Point& obj)
{
	this->point.translate(obj);
}

const char* Rectangle::getFiguresName() const
{
	return "rectangle";
}

const Point& Rectangle::getPoint() const
{
	return this->point;
}

int Rectangle::getWidth() const
{
	return this->width;
}

int Rectangle::getHeight() const
{
	return this->height;
}

//////////////////////////////////////////////////////////////

void Rectangle::attributeTakesValue(const char* attribute, const char* value)
{
	if (strcmp(attribute, "x") == 0)
	{
		int x = stringToNumber(value);

		this->point.setX(x);
	}
	else if (strcmp(attribute, "y") == 0)
	{
		int y = stringToNumber(value);

		this->point.setY(y);
	}
	else if (strcmp(attribute, "width") == 0)
	{
		int width = stringToNumber(value);

		if (width < 0)
			throw "Width must bu positive number!";		

		this->width = width;
	}
	else if (strcmp(attribute, "height") == 0)
	{
		int height = stringToNumber(value);

		if (height < 0)
			throw "height must bu positive number!";

		this->height = height;
	}
	else if (strcmp(attribute, "rx") == 0)
	{
		int rx = stringToNumber(value);

		if (rx < 0)
			throw "rx must bu positive numbers!";		

		this->rx = rx;
	}
	else if (strcmp(attribute, "ry") == 0)
	{
		int ry = stringToNumber(value);

		if (ry < 0)
			throw "ry must bu positive numbers!";

		this->ry = ry;
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