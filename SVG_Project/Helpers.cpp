#include "Helpers.h"

#include "Circle.h"
#include "Rectangle.h"
#include "Line.h"

#include "TemplateFunctions.h"

#include <iostream>
#include <cstring>

Figure* getFigureFromSVG_String(char* buffer)
{
	if (strcmp(buffer, "rect") == 0)
		return new Rectangle;
	else if (strcmp(buffer, "circle") == 0)
		return new Circle;
	else if (strcmp(buffer, "line") == 0)
		return new Line;
	else
		throw "Unknown figure!";
}

Figure* getFigureFromConsoleString(char* buffer)
{
	if (strcmp(buffer, "rectangle") == 0)
		return new Rectangle;
	else if (strcmp(buffer, "circle") == 0)
		return new Circle;
	else if (strcmp(buffer, "line") == 0)
		return new Line;
	else
		throw "There is no such figure!";
}

const char* ignoreCharacter(const char* buffer, char ch)
{
	while (*(buffer) == ch)
		buffer++;

	return buffer;
}

const char* ignoreWhiteCharacters(const char* buffer)
{
	while (*buffer == '\n' || *buffer == ' ' || *buffer == '\t' || *buffer == '\r')
		buffer++;

	return buffer;
}

bool findAndZeroingCharacter(char* buffer, char*& afterCharacter, char ch)
{
	int size = strlen(buffer);

	int i = 0;

	for (; i < size; i++)
	{
		if (buffer[i] == ch)
		{
			buffer[i] = '\0';

			break;
		}
	}


	if (i == size)
	{
		afterCharacter = buffer + size;

		return false;
	}

	afterCharacter = buffer + i + 1;

	return true;
}

bool findAndZeroingWhiteCharacter(char* buffer, char*& afterCharacter)
{
	int size = strlen(buffer);

	int i = 0;

	for (; i < size; i++)
	{
		if (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n' || buffer[i] == '\r')
		{
			buffer[i] = '\0';

			break;
		}
	}


	if (i == size)
	{
		afterCharacter = buffer + size;

		return false;
	}

	afterCharacter = buffer + i + 1;

	return true;
}

char* strStr(const char* Where, const char* what, int& LengthToSVG_Tag)
{
	LengthToSVG_Tag = 0;

	int whatLen = strlen(what);
	int pos;
	const char* whereEnd = Where + strlen(Where);

	while (Where + whatLen <= whereEnd)
	{
		for (pos = 0; pos < whatLen; ++pos)
		{
			if (Where[pos] != what[pos])
				break;
		}

		if (pos == whatLen) 
			return (char*)Where;

		++Where;

		++LengthToSVG_Tag;
	}

	return NULL;
}

int getNumberOfCharachters(const char* str, char ch)
{
	int count = 0;

	while (*str != '\0')
	{
		if (*(str++) == ch)
			++count;
	}

	return count;
}

void takeWordsFromString(char* buffer, char**& data, int& count)
{
	data = NULL;

	count = 0;

	char* tempPointer;

	for (;; count++)
	{
		tempPointer = (char*)ignoreWhiteCharacters(buffer);

		if (*(tempPointer) == '\0')
			break;

		if (!findAndZeroingWhiteCharacter(tempPointer, buffer))
		{
			try
			{
				resize(data, count, count + 1);

				data[count] = new char[strlen(tempPointer) + 1];
			}
			catch (std::bad_alloc& ba)
			{
				for (int i = 0; i < count; i++)
				{
					delete[] data[i];
				}

				delete[] data;

				throw;
			}

			strcpy(data[count], tempPointer);

			count++;

			break;
		}

		try
		{
			resize(data, count, count + 1);

			data[count] = new char[strlen(tempPointer) + 1];
		}
		catch (std::bad_alloc& ba)
		{
			for (int i = 0; i < count; i++)
			{
				delete[] data[i];
			}

			delete[] data;

			throw;
		}

		strcpy(data[count], tempPointer);
	}
}

void takeAttributeFromString(char* buffer, char*& value)
{
	if (!findAndZeroingCharacter(buffer, value, '='))
		throw "Error! Problem with the attribute!!";

	if (*value != '\"')
		throw "Error! Problem with the attribute!";

	value++;

	if (!findAndZeroingCharacter(value, buffer, '\"'))
		throw "Error! Problem with the attribute!!!";
}

const char* colorToString(Color clr)
{
	switch (clr)
	{
	case BLACK:
		return "black";
	case NONE:
		return "none";
	case RED:
		return "red";
	case WHITE:
		return "white";
	case BLUE:
		return "blue";
	case GREEN:
		return "green";
	case YELLOW: 
		return "yellow";
	case ORANGE:
		return "orange";
	}

	throw "Error! There is no such color!";
}

Color stringToColor(const char* str)
{
	if (strcmp(str, "black") == 0)
		return BLACK;

	if (strcmp(str, "none") == 0)
		return NONE;

	if (strcmp(str, "red") == 0)
		return RED;

	if (strcmp(str, "white") == 0)
		return WHITE;

	if (strcmp(str, "blue") == 0)
		return BLUE;

	if (strcmp(str, "green") == 0)
		return GREEN;

	if (strcmp(str, "yellow") == 0)
		return YELLOW;

	if (strcmp(str, "orange") == 0)
		return ORANGE;

	throw "Error! There is no such color!";
}

int stringToNumber(const char* str)
{
	bool isNegative = false;

	if (*str == '-')
	{
		isNegative = true;
		
		str++;
	}

	str = ignoreCharacter(str, ' ');

	int length = strlen(str);

	for (int i = 0; i < length; i++)
	{
		if (str[i] > '9' || str[i] < '0')
			throw "Error! This is not a number!";		//pobutni go malko
	}

	if (str[0] == '0' && length > 1)
		throw "Error! A number can't begin with 0!";

	int p;

	int x = 0;

	for (int i = 0; i < length; i++)
	{
		p = 1;

		for (int k = 0; k < i; k++)
			p *= 10;

		x += p*(str[length - 1 - i] - '0');
	}

	if (isNegative)
		return -x;

	return x;
}

void numberToString(char* const& str ,int number)
{
	int i = 0;

	bool isNegative = false;

	if (number < 0)
	{
		isNegative = true;

		number *= -1;
	}

	if (number == 0)
		str[i++] = '0';

	for (; number != 0; i++)
	{
		str[i] = '0' + number % 10;

		number /= 10;
	}

	if (isNegative)
		str[i++] = '-';

	str[i] = '\0';

	char temp;

	for (int k = 0; k < i / 2; k++)
	{
		temp = str[k];

		str[k] = str[i - 1 - k];

		str[i - 1 - k] = temp;
	}
}

void stringIsValidSVG_File(const char* fileString)
{
	int size = strlen(fileString);

	for (int i = 0; i < size; i++)
	{
		if (*(fileString + size - 1 - i) == '.')
		{
			fileString += size - i;
			break;
		}
	}

	if (strcmp(fileString, "svg") != 0)
		throw "Error! The file must have .svg extension!";
}

void onlyWhiteCharacters(const char* str, int oldLinesCount)
{
	const char* start = str;

	str = ignoreWhiteCharacters(str);

	if (*str != '\0')
	{
		int length = str - start;

		int newLinesCount = 0;

		for (int i = 0; i < length; i++)
		{
			if (start[i] == '\n')
				++newLinesCount;
		}

		char numberString[16];

		numberToString(numberString, oldLinesCount + newLinesCount);

		char* throwString = new char[strlen("Error! Problen on line ") + strlen(numberString)
			+ strlen(" . Must not have not white characters between the tags!") + 1];

		strcpy(throwString, "Error! Problen on line ");
		strcat(throwString, numberString);
		strcat(throwString, " . Must not have not white characters between the tags!");

		throw throwString;
	}
}

bool isSVG_CloseTag(const char* tag, const char* afterTag)
{
	int length = strlen(tag);

	if (length > 3 && tag[0] == '/' && tag[1] == 's' && tag[2] == 'v' && tag[3] == 'g')
	{
		if (*ignoreWhiteCharacters(tag + 4) != '\0')
			throw "Error! Problem with svg close tag!";

		if (*ignoreWhiteCharacters(afterTag) != '\0')
			throw "Error! There must be nothing after </svg> tag!\n";

		return true;
	}

	return false;
}

bool isCommentTag(const char* tag)
{
	int length = strlen(tag);

	if (length > 2 && tag[0] == '!' && tag[1] == '-' && tag[2] == '-')
	{
		if (length > 4 && tag[length - 1] == '-' && tag[length - 1] == '-')
			return true;

		throw "Comment is not terminated!";
	}

	return false;
}

bool isUnknownTag(char*&beforeTag, char* tag, char* afterTag, int& linesCount)
{
	int tagsLength = strlen(tag);

	char linesCountString[16];

	numberToString(linesCountString, linesCount);

	if (tag[tagsLength - 1] == '/')
		return false;

	char* content;

	findAndZeroingWhiteCharacter(tag, content);

	if (*content == '\0')
		content++;

	int closeTagStringLength = strlen(tag) + 2;
	
	char* closeTagString = new char[closeTagStringLength + 1];
	
	strcpy(closeTagString, "</");
	strcat(closeTagString, tag);

	int unusedInt = 0;
	beforeTag = strStr(afterTag, closeTagString, unusedInt);

	char* theCloseTag = beforeTag;

	delete[] closeTagString;

	if (beforeTag == NULL)
	{
		char* throwString = new char[strlen("Error! The tag ") + strlen(tag) +
			strlen(" on line ") + strlen(linesCountString) + strlen(" is not closed!") + 1];

		strcpy(throwString, "Error! The tag ");
		strcat(throwString, tag);
		strcat(throwString, " on line ");
		strcat(throwString, linesCountString);
		strcat(throwString, " is not closed!");

		throw throwString;
	}

	if (!findAndZeroingCharacter(beforeTag, beforeTag, '>') || 
		*ignoreWhiteCharacters(theCloseTag + closeTagStringLength) != '\0')
	{
		char* throwString = new char[strlen("Error! The tag ") + strlen(tag) +
			strlen(" on line ") + strlen(linesCountString) + strlen(" is not properly closed!") + 1];

		strcpy(throwString, "Error! The tag ");
		strcat(throwString, tag);
		strcat(throwString, " on line ");
		strcat(throwString, linesCountString);
		strcat(throwString, " is not properly closed!");

		throw throwString;
	}

	linesCount += getNumberOfCharachters(content, '\n');
	
	return true;
}
