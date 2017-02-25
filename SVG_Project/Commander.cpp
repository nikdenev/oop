#include "Commander.h"

#include "Picture.h"

#include "Rectangle.h"
#include "Circle.h"
#include "Line.h"

#include "Point.h"

#include "Helpers.h"
#include "TemplateFunctions.h"

#include <iostream>
#include <fstream>
#include <cstring>

Commander& Commander::getInstance()
{
	static Commander theInstance;

	return theInstance;
}

//////////////////////////////////////////////////////////////

void Commander::executeCommands()
{
	typedef void (Commander::*Command)();

	const int commandsCount = 9;

	const Command commands[commandsCount] = { &Commander::open, &Commander::close, &Commander::save, &Commander::saveas, &Commander::print,
		&Commander::create, &Commander::erase, &Commander::translate, &Commander::within };

	const char* commandsNames[commandsCount] = { "open", "close", "save", "saveas", "print", "create", "erase", "translate", "within" };

	while (true)
	{
		Commander::getInstance().getCommandAndData();

		if (strcmp(this->commandStringFromConsole, "exit") == 0)
		{
			std::cout << "Exit\n";

			break;
		}

		int i = 0;

		for (; i < commandsCount; i++)
		{
			if (strcmp(this->commandStringFromConsole, commandsNames[i]) == 0)
			{
				(Commander::getInstance().*commands[i])();
				
				break;
			}
		}

		if (i == commandsCount)
			std::cout<< "There is no such command! Try again:\n";
	}
}

//////////////////////////////////////////////////////////////

void Commander::open()
{
	if (this->hasOpenFile)														
	{
		std::cout << "Error! Can't open two files!\n";

		return;
	}

	if (!this->hasData)															
	{
		std::cout << "Error! You must enter the file you want to open!\n";

		return;
	}

	try
	{
		stringIsValidSVG_File(this->dataStringFromConsole);						
	}
	catch (const char* what)
	{
		std::cout << what << '\n';

		return;
	}
	
	strcpy(this->fileName, this->dataStringFromConsole);

	this->hasOpenFile = false;

	char* fileData;

	if (!this->getDataFromFile(fileData))
		return;

	char* afterSVG_Tag;

	if (!this->getBeginningOfSVG_File(fileData, afterSVG_Tag))
		return;

	int linesCount = 1;

	linesCount += getNumberOfCharachters(this->beginningOfSVG_File, '\n');

	char* beforeTag = afterSVG_Tag;
	char* tag = afterSVG_Tag;
	char* afterTag = NULL;

	while (findAndZeroingCharacter(beforeTag, tag, '<'))
	{
		try
		{
			onlyWhiteCharacters(beforeTag, linesCount);
		}
		catch (char* what)
		{
			std::cout << what << '\n';

			delete[] what;

			this->cleanPictureAndCommander(fileData);

			return;
		}
		catch (std::bad_alloc& ba)
		{
			std::cout << "Memory proble!\n";

			this->cleanPictureAndCommander(fileData);

			return;
		}

		linesCount += getNumberOfCharachters(beforeTag, '\n');

		int oldLinesCount = linesCount;

		if (!findAndZeroingCharacter(tag, afterTag, '>'))
		{
			std::cout << "Error! The tag on " << linesCount << " in not closed\n";

			this->cleanPictureAndCommander(fileData);

			return;
		}

		int tagsLength = strlen(tag);

		if (tagsLength == 0)
		{
			std::cout << "Error! Problem on line " << oldLinesCount << " . Unvalid tag!\n";

			this->cleanPictureAndCommander(fileData);

			return;
		}

		linesCount += getNumberOfCharachters(tag, '\n');

		if (*tag == ' ' || *tag == '\t' || *tag == '\n')
		{
			std::cout << "Error! On line " << oldLinesCount << " . Tag must not start with white character!\n";

			this->cleanPictureAndCommander(fileData);

			return;
		}

		try
		{
			if (isSVG_CloseTag(tag, afterTag))
			{
				delete[] fileData;

				std::cout << "Successsfully opened " << this->fileName << '\n';

				this->hasOpenFile = true;

				return;
			}
		}
		catch (const char* what)
		{
			std::cout << what << '\n';

			this->cleanPictureAndCommander(fileData);

			return;
		}

		try
		{
			if (isCommentTag(tag))
			{
				beforeTag = afterTag;

				continue;
			}
		}
		catch (const char* what)
		{
			std::cout <<"Error! Problem on line "<< oldLinesCount<< ". " << what << '\n';

			this->cleanPictureAndCommander(fileData);

			return;
		}

		try
		{
			if (isUnknownTag(beforeTag, tag, afterTag, linesCount))
			{
				std::cout << "Worning the tag " << tag << " on line " << oldLinesCount << " is ignored!\n";

				continue;
			}
		}
		catch (char* what)
		{
			std::cout << what << '\n';

			delete[] what;

			this->cleanPictureAndCommander(fileData);

			return;
		}
		catch (std::bad_alloc& ba)
		{
			std::cout << "Error! Memory Problem!\n";

			this->cleanPictureAndCommander(fileData);

			return;
		}

		tag[tagsLength - 1] = '\0';

		beforeTag = afterTag;

		char* dataString = tag;

		findAndZeroingWhiteCharacter(dataString, dataString);

		ignoreCharacter(dataString, ' ');

		Figure* figure;

		try
		{
			figure = getFigureFromSVG_String(tag);
		}
		catch (const char* what)
		{
			std::cout << "The figure "<< tag << " on line " << oldLinesCount <<
				" is unknown and she will be ignored!\n";

			continue;
		}
		catch (std::bad_alloc& ba)
		{
			std::cout << "Error! Memory problem!\n";

			this->cleanPictureAndCommander(fileData);

			return;
		}

		try
		{
			figure->readStringFromSVG(dataString);
		}
		catch (char* what)
		{
			std::cout << "Error! Problem with the figure " << tag << "on line "
				<< oldLinesCount << ". Problem with attribute "<< what << '\n';

			delete[] what;

			this->cleanPictureAndCommander(fileData);

			return;
		}
		catch (std::bad_alloc& ba)
		{
			std::cout << "Error! Memory problem!\n";

			this->cleanPictureAndCommander(fileData);

			return;
		}

		try
		{
			Picture::getInstance().addFigure(figure);
		}
		catch (std::bad_alloc& ba)
		{
			std::cout << "Error! Memory problem!\n";

			this->cleanPictureAndCommander(fileData);

			return;
		}
	}

	delete[] fileData;
	
	std::cout << "Error! svg tag is not closed!\n";

	Picture::getInstance().clean();

	this->clean();
}

void Commander::close()
{
	if (this->hasData)
	{
		std::cout << "Error! Wrong command!!" << '\n';

		return;
	}

	if (!this->hasOpenFile)
	{
		std::cout << "Error! There is no open file!" << '\n';
		
		return;
	}

	this->hasOpenFile = false;

	Picture::getInstance().clean();

	Commander::getInstance().clean();

	std::cout << "Successfully closed " << this->fileName << '\n';
}

void Commander::save()
{
	if (this->hasData)
	{
		std::cout << "Error! Wrong command!!\n";

		return;
	}

	if (!this->hasOpenFile)
	{
		std::cout << "Error! There is no open file!\n";

		return;
	}

	save(this->fileName);
}

void Commander::saveas()
{
	if (!this->hasData)
	{
		std::cout << "Error! Wrong command!!\n";

		return;
	}

	if (!this->hasOpenFile)
	{
		std::cout << "Error! There is no open file!\n";

		return;
	}

	save(this->dataStringFromConsole);
}

void Commander::print()
{
	if (this->hasData)
	{
		std::cout << "Error! Wrong command!!\n";

		return;
	}

	if (!this->hasOpenFile)
	{
		std::cout << "Error! There is no open file!\n";

		return;
	}

	int figuresCount = Picture::getInstance().getFiguresCount();

	if (figuresCount == 0)
	{
		std::cout << "There isn't any figures!\n";

		return;
	}

	char** figuresData = new char*[figuresCount];

	for (int i = 0; i < figuresCount; i++)
	{
		try
		{
			figuresData[i] = Picture::getInstance().getFigure(i)->getDataInConsoleFormat();
		}
		catch (std::bad_alloc& ba)
		{
			std::cout << ba.what() << '\n';

			for (int k = 0; k < i; k++)
				delete[] figuresData[i];

			delete[] figuresData;

			return;
		}
	}

	for (int i = 0; i < figuresCount; i++)
		std::cout << i + 1 << ". " << figuresData[i] << '\n';

	for (int i = 0; i < figuresCount; i++)
		delete[] figuresData[i];

	delete[] figuresData;
}

void Commander::create()
{
	if (!this->hasData)
	{
		std::cout << "Error! Wrong command!!\n";

		return;
	}

	if (!this->hasOpenFile)
	{
		std::cout << "Error! There is no open file!\n";

		return;
	}

	char* figuresData = this->dataStringFromConsole;

	if (!findAndZeroingWhiteCharacter(figuresData, figuresData))
	{
		std::cout << "Error! The figure you wont to create must have some kind of data!\n";

		return;
	}

	figuresData = (char*)ignoreWhiteCharacters(figuresData);

	Figure* figure;

	try
	{
		figure = getFigureFromConsoleString(this->dataStringFromConsole);
	}
	catch (const char* what)
	{
		std::cout << what << '\n';

		return;
	}
	catch (std::bad_alloc& ba)
	{
		std::cout << "Error! Memory Problem!\n";

		return;
	}

	figure->readStringFromConsole(figuresData);

	Picture::getInstance().addFigure(figure);

	std::cout << "Successfully created " << this->dataStringFromConsole << "!\n";
}

void Commander::erase()
{
	if (!this->hasData)
	{
		std::cout << "Error! Wrong command!!\n";

		return;
	}

	if (!this->hasOpenFile)
	{
		std::cout << "Error! There is no open file!\n";

		return;
	}

	int index = stringToNumber(this->dataStringFromConsole);

	int figuresCount = Picture::getInstance().getFiguresCount();

	if (index > figuresCount || index < 0)
	{
		std::cout << "Error! There is no figure with index " << index << "!\n";

		return;
	}

	try
	{
		Picture::getInstance().removeFigure(index - 1);
	}
	catch (std::bad_alloc& ba)
	{
		std::cout << ba.what() << '\n';

		return;
	}

	std::cout << "Erased figure number " << index << "!\n";
}

void Commander::translate()
{
	if (!this->hasData)
	{
		std::cout << "Error! Wrong command!!\n";

		return;
	}

	if (!this->hasOpenFile)
	{
		std::cout << "Error! There is no open file!\n";

		return;
	}

	char* UNUSED;

	char* vertical = this->dataStringFromConsole;

	char* horizontal;

	bool translateAll = true;

	int index = 0;

	if (this->dataStringFromConsole[0] >= '0' && this->dataStringFromConsole[0] <= '9')
	{
		translateAll = false;

		findAndZeroingWhiteCharacter(vertical, vertical);

		index = stringToNumber(this->dataStringFromConsole);

		index--;
	}

	char* verticalTranslation = vertical;

	vertical = (char*)ignoreWhiteCharacters(vertical);

	findAndZeroingCharacter(verticalTranslation, verticalTranslation, '=');

	findAndZeroingWhiteCharacter(vertical, UNUSED);

	verticalTranslation = (char*)ignoreCharacter(verticalTranslation, ' ');

	findAndZeroingWhiteCharacter(verticalTranslation, horizontal);


	horizontal = (char*)ignoreWhiteCharacters(horizontal);

	char* horizontalTranslation = horizontal;

	findAndZeroingCharacter(horizontalTranslation, horizontalTranslation, '=');

	findAndZeroingWhiteCharacter(horizontal, UNUSED);

	horizontalTranslation = (char*)ignoreWhiteCharacters(horizontalTranslation);

	if (findAndZeroingWhiteCharacter(horizontalTranslation, UNUSED))
	{
		if (*ignoreWhiteCharacters(UNUSED) != '\0')
		{
			std::cout << "Error! Wrong command!\n";

			return;
		}
	}

	if (strcmp(vertical, "vertical") != 0)
	{
		std::cout << "Error! Wrong data!\n";

		return;
	}

	if (strcmp(horizontal, "horizontal") != 0)
	{
		std::cout << "Error! Wrong data!\n";

		return;
	}

	int y = stringToNumber(verticalTranslation);

	int x = stringToNumber(horizontalTranslation);

	Point vector(x, y);

	Picture::getInstance().translate(vector, index, translateAll);

	if (translateAll)
	{
		std::cout << "Translated all figures!\n";

		return;
	}
	
	std::cout << "Translated figure number " << index + 1<< "!\n";
}

void Commander::within()
{
	if (!this->hasData)
	{
		std::cout << "Error! Wrong command!!\n";

		return;
	}

	if (!this->hasOpenFile)
	{
		std::cout << "Error! There is no open file!\n";

		return;
	}

	char* figuresData = this->dataStringFromConsole;

	findAndZeroingWhiteCharacter(figuresData, figuresData);

	figuresData = (char*)ignoreWhiteCharacters(figuresData);

	try
	{
		if (strcmp(this->dataStringFromConsole, "rectangle") == 0)
		{
			Rectangle rectangle;

			rectangle.readStringFromConsole(figuresData);

			Picture::getInstance().within(rectangle);
		}
		else if (strcmp(this->dataStringFromConsole, "circle") == 0)
		{
			Circle circle;

			circle.readStringFromConsole(figuresData);

			Picture::getInstance().within(circle);
		}
		else
		{
			std::cout << "Error! Wrong command!\n";
		}
	}
	catch (std::bad_alloc& ba)
	{
		std::cout << "Error! Memory Problem!\n";
	}
}

//////////////////////////////////////////////////////////////

bool Commander::getDataFromFile(char*& fileData)
{
	const char*  baseBeginningOfSVG_File = "<?xml version=\"1.0\" standalone=\"no\"?>\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n<svg width=\"12cm\" height=\"4cm\" viewBox=\"0 0 1200 400\"\nxmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";

	std::ifstream file(this->dataStringFromConsole, std::ios::in | std::ios::binary);
	if (!file)
	{
		try
		{
			this->beginningOfSVG_File = new char[strlen(baseBeginningOfSVG_File) + 1];
		}
		catch (std::bad_alloc& ba)
		{
			std::cout << "Error! Memory Problem!\n";

			return false;
		}

		strcpy(this->beginningOfSVG_File, baseBeginningOfSVG_File);

		this->hasOpenFile = true;

		std::cout << "Successsfully opened " << this->fileName << '\n';

		return false;
	}

	file.seekg(0, std::ios::end);

	int fileSize = file.tellg();
	if (fileSize == 0)
	{
		std::cout << "Error! The " << this->fileName << " file is empty!\n";

		return false;
	}

	file.seekg(0, std::ios::beg);

	try
	{
		fileData = new char[fileSize + 1];
	}
	catch (std::bad_alloc& ba)
	{
		std::cout << "Error! Memory problem!\n";

		return false;
	}

	file.read(fileData, fileSize);
	if (!file)
	{
		delete[] fileData;

		std::cout << "Problem with reading the " << this->fileName << " file\n";

		return false;
	}

	fileData[fileSize] = '\0';

	file.close();

	return true;
}

bool Commander::getBeginningOfSVG_File(char* fileData, char*& afterSVG_Tag)
{
	int lengthToSVG_Tag = 0;

	afterSVG_Tag = strStr(fileData, "<svg", lengthToSVG_Tag);

	if (afterSVG_Tag == NULL || !(afterSVG_Tag[4] == ' ' || afterSVG_Tag[4] == '>'))
	{
		delete[] fileData;

		std::cout << "Error! Can't find <svg ...> tag!\n";

		return false;
	}

	if (!findAndZeroingCharacter(afterSVG_Tag, afterSVG_Tag, '>'))
	{
		delete[] fileData;

		std::cout << "Error! SVG tag must be closed with '>' charachter\n";

		return false;
	}

	if (lengthToSVG_Tag == 0)
	{
		delete[] fileData;

		std::cout << "Error! The file " << this->fileName << " must have header tags!\n";

		return false;
	}

	try
	{
		this->beginningOfSVG_File = new char[strlen(fileData) + 2];
	}
	catch (std::bad_alloc& ba)
	{
		delete[] fileData;

		std::cout << "Error! Memory problem!\n";

		return false;
	}

	strcpy(this->beginningOfSVG_File, fileData);

	strcat(this->beginningOfSVG_File, ">");

	return true;
}

void Commander::getCommandAndData()
{
	char buffer[2048];

	char* command;

	char* data;

	this->hasData = false;

	std::cin.getline(buffer, 2048);

	command = buffer;

	if (*buffer != '>')
	{
		this->commandStringFromConsole[0] = '\0';

		return;
	}

	command = buffer + 1;

	command = (char*)ignoreWhiteCharacters(command);

	if (!findAndZeroingWhiteCharacter(command, data))
	{
		strcpy(this->commandStringFromConsole, command);

		return;
	}

	data = (char*)ignoreWhiteCharacters(data);

	if (*(data) != '\0')
	{
		this->hasData = true;

		int length = strlen(data);

		for (int i = 0; i < length; i++)
		{
			if (data[length - 1 - i] != ' ')
			{
				data[length - i] = '\0';

				break;
			}
		}
	}

	strcpy(this->commandStringFromConsole, command);

	strcpy(this->dataStringFromConsole, data);
}

void Commander::clean()
{
	delete[] this->beginningOfSVG_File;

	this->beginningOfSVG_File = NULL;
}

void Commander::save(char* theFileName)
{
	try
	{
		stringIsValidSVG_File(theFileName);
	}
	catch (const char* what)
	{
		std::cout << what << '\n';

		return;
	}

	int sizeOfDataForSVG_File = 0;

	char* dataForSVG_File = NULL;

	int beginningLength = strlen(this->beginningOfSVG_File);

	try
	{
		resize(dataForSVG_File, sizeOfDataForSVG_File, beginningLength + 3);

		sizeOfDataForSVG_File += beginningLength + 3;
	}
	catch (std::bad_alloc& ba)
	{
		delete[] dataForSVG_File;

		std::cout << "Error! Memory Problem!\n";

		return;
	}

	strcpy(dataForSVG_File, this->beginningOfSVG_File);
	strcat(dataForSVG_File, "\n\n");

	char* figureData;

	int figureDataLength = 0;

	int figuresCount = Picture::getInstance().getFiguresCount();

	for (int i = 0; i < figuresCount; i++)
	{
		try
		{
			figureData = Picture::getInstance().getFigure(i)->getDataInSVG_Format();
		}
		catch (std::bad_alloc& ba)
		{
			delete[] dataForSVG_File;

			std::cout << "Error! Memory Problem!\n";

			return;
		}

		figureDataLength = strlen(figureData);

		try
		{
			resize(dataForSVG_File, sizeOfDataForSVG_File, sizeOfDataForSVG_File + figureDataLength + 4);

			sizeOfDataForSVG_File += figureDataLength + 4;
		}
		catch (std::bad_alloc& ba)
		{
			delete[] figureData;

			delete[] dataForSVG_File;

			std::cout << "Error! Memory Problem!\n";

			return;
		}

		strcat(dataForSVG_File, "<");
		strcat(dataForSVG_File, figureData);
		strcat(dataForSVG_File, "/>\n");

		delete[] figureData;
	}

	try
	{
		resize(dataForSVG_File, sizeOfDataForSVG_File, sizeOfDataForSVG_File + 7);

		sizeOfDataForSVG_File += 7;
	}
	catch (std::bad_alloc& ba)
	{
		delete[] dataForSVG_File;

		std::cout << "Error! Memory Problem!\n";

		return;
	}

	strcat(dataForSVG_File, "\n</svg>");

	std::ofstream file(theFileName, std::ios::trunc);
	if (!file)
	{
		delete[] dataForSVG_File;

		std::cout << "Error! Can't write in this file or can't create file " << theFileName << "!\n";

		return;
	}

	file.write(dataForSVG_File, sizeOfDataForSVG_File - 1);

	if (!file)
	{
		delete[] dataForSVG_File;

		std::cout << "Problem with writing in file " << theFileName << "!\n";

		return;
	}

	delete[] dataForSVG_File;

	file.close();

	std::cout << "Successfully saved " << theFileName << "!\n";
}

void Commander::cleanPictureAndCommander(char* somethingToErase)
{
	delete[] somethingToErase;

	this->clean();

	Picture::getInstance().clean();
}

//////////////////////////////////////////////////////////////

Commander::Commander() :
beginningOfSVG_File(NULL),
hasOpenFile(false),
hasData(false)
{

}

Commander::~Commander()
{
	this->clean();
}