#include "Commander.h"

#include <iostream>

static const int UnvalidIndex = -1;

//helpers
void resize(Mage*& array, int oldSize, int newSize);
int findElement(const Mage* array, int size, const Mage& element);


std::istream& operator >> (std::istream& stream, Commander& obj)
{
	Commander tempCommander;

	stream >> (Soldier&)tempCommander;

	char buffer[1024] = "Description unknown.";

	stream.getline(buffer, 1024);
	if (!stream)
	{
		stream.clear();
		stream.ignore(1024, '\n');
	}

	tempCommander.description = new char[strlen(buffer) + 1];
	strcpy(tempCommander.description, buffer);

	int magesCount = 0;

	stream >> magesCount;
	if (!stream)
		stream.clear();

	stream.ignore(1024, '\n');

	Mage tempMage;

	for (int i = 0; i < magesCount; i++)
	{
		stream >> tempMage;

		tempCommander.addMage(tempMage);
	}

	obj = tempCommander;

	return stream;
}

std::ostream& operator << (std::ostream& stream, const Commander& obj)
{
	stream << (Soldier)obj;
	stream << obj.description << '\n';
	stream << obj.magesCount << '\n';

	for (int i = 0; i < obj.magesCount; i++)
		stream << obj.mages[i];

	if (!stream)
	{
		stream.clear();
		throw "Error with writing in the file!";
	}

	return stream;
}

Commander::Commander() :
description(NULL),
magesCount(0),
mages(NULL),
sizeOfMages(0)
{

}

Commander::Commander(const char* name, int age, int fightSkills, int salary, const char* description) :
Soldier(name, age, fightSkills, salary),
description(NULL),
magesCount(0),
mages(NULL),
sizeOfMages(0)
{
	if (description != NULL)
	{
		this->description = new char[strlen(description) + 1];

		strcpy(this->description, description);
	}
}

Commander::~Commander()
{
	this->cleanCommander();
}

Commander::Commander(const Commander& obj) :
Soldier(obj),
description(NULL),
magesCount(0),
mages(NULL),
sizeOfMages(0)
{
	this->copyCommander(obj);
}

Commander& Commander::operator= (const Commander& obj)
{
	if (this != &obj)
	{
		this->Soldier::operator= (obj);

		this->copyCommander(obj);
	}

	return *this;
}


void Commander::setCommander(const char* name, int age, int fightSkills, int salary, const char* description)
{
	char* newDescription;

	if (description != NULL)
	{
		newDescription = new char[strlen(description) + 1];

		strcpy(newDescription, description);
	}

	try
	{
		this->Soldier::setSoldier(name, age, fightSkills, salary);
	}
	catch (std::bad_alloc& ba)
	{
		delete[] newDescription;
		throw;
	}
	catch (const char* what)
	{
		delete[] newDescription;
		throw;
	}

	delete[] this->description;

	this->description = newDescription;
}

void Commander::addMage(const Mage& obj)
{
	if (findElement(this->mages, this->magesCount, obj) == UnvalidIndex)
	{
		if (this->fightSkills <= obj.Soldier::getFightSkills())
			throw "The commander's fight power must be more than the mage's fight power!";

		if (this->magesCount == this->sizeOfMages)
		{
			resize(this->mages, this->sizeOfMages, (3 * this->sizeOfMages) / 2 + 2);

			this->sizeOfMages = (3 * this->sizeOfMages) / 2 + 2;
		}

		this->mages[this->magesCount++] = obj;
	}
	else
	{
		throw "You can't add one mage twice ):";
	}
}
void Commander::removeMage(const Mage& obj)
{
	if (this->magesCount <= 0)
		throw "There are no mages to remove!";

	int index = findElement(this->mages, this->magesCount, obj);

	if (index != UnvalidIndex)
	{
		if (2 * this->magesCount == this->sizeOfMages)
			resize(this->mages, this->magesCount, (5 * this->magesCount) / 4);

		this->sizeOfMages = (5 * this->magesCount) / 4;

		Mage temp(this->mages[index]);
		this->mages[index] = this->mages[this->magesCount - 1];
		this->mages[this->magesCount-- - 1] = temp;
	}
	else
	{
		throw "Can't find that mage!";
	}
}

int Commander::getMagesCount() const
{
	return this->magesCount;
}

int Commander::getSalary() const
{
	return this->salary;
}

int Commander::getFightSkills() const
{
	return this->fightSkills;
}

const Mage& Commander::operator[] (int index) const
{
	if (index < 0 || index >= this->magesCount)
		throw "Error! Unvalid index!";

	return mages[index];
}

void Commander::cleanCommander()
{
	delete[] this->description;
	this->description = NULL;

	delete[] this->mages;
	this->mages = NULL;

	this->magesCount = 0;
	this->sizeOfMages = 0;
}

void Commander::copyCommander(const Commander& obj)
{
	char* description = NULL;

	if (obj.description != NULL)
	{
		description = new char[strlen(obj.description) + 1];

		strcpy(description, obj.description);
	}

	Mage* mages;

	try
	{
		mages = new Mage[obj.magesCount];
	}
	catch (std::bad_alloc& ba)
	{
		delete[] description;
		throw;
	}

	for (int i = 0; i < obj.magesCount; i++)
	{
		try
		{
			mages[i] = obj.mages[i];
		}
		catch (std::bad_alloc& ba)
		{
			delete[] description;

			delete[] mages;

			throw;
		}
	}

	this->cleanCommander();

	this->description = description;
	this->mages = mages;
	this->magesCount = obj.magesCount;
	this->sizeOfMages = obj.magesCount;
}

//////////////////////////////////////////////////////////////

void resize(Mage*& array, int oldSize, int newSize)
{
	if (newSize < 0 || oldSize < 0)
		throw "Error! Invalid size!";

	if (newSize == 0)
	{
		array = NULL;
		return;
	}

	Mage* newArray = new Mage[newSize];

	for (int i = 0; i < oldSize && i < newSize; i++)
		newArray[i] = array[i];

	delete[] array;

	array = newArray;
}

int findElement(const Mage* array, int size, const Mage& element)
{
	for (int i = 0; i < size; i++)
	{
		if (array[i] == element)
			return i;
	}

	return UnvalidIndex;
}
