#pragma once

class Commander
{
public:
	static Commander& getInstance();

public:
	void executeCommands();

private:
	void open();

	void close();

	void save();

	void saveas();

	void print();

	void create();

	void erase();
	
	void translate();

	void within();

private:

	bool getDataFromFile(char*& fileData);

	bool getBeginningOfSVG_File(char* fileData, char*& afterSVG_Tag);

	void getCommandAndData();

	void clean();

	void cleanPictureAndCommander(char* somethingToErase);

	void save(char* theFileName);

	

private:
	Commander();
	~Commander();
	Commander(const Commander& obj);
	void operator=(const Commander& obj);

private:
	char fileName[1024];

	char commandStringFromConsole[1024];

	char dataStringFromConsole[1024];

	char* beginningOfSVG_File;

	bool hasOpenFile;

	bool hasData;
};

