#include "Sergeant.h"
#include <iostream>

static const int UnvalidIndex = -1;

//helpers
static void resize(Soldier*& array, int oldSize, int newSize);
static int findElement(const Soldier* array, int size, const Soldier& element);


std::istream& operator >> (std::istream& stream, Sergeant& obj)
{
	Sergeant tempSergeant;

	stream >> (Soldier&)tempSergeant;

	char buffer[1024] = "Description unknown.";

	stream.getline(buffer, 1024);
	if (!stream)
	{
		stream.clear();
		stream.ignore(1024, '\n');
	}

	tempSergeant.description = new char[strlen(buffer) + 1];
	strcpy(tempSergeant.description, buffer);

	int soldiersCount = 0;

	stream >> soldiersCount;
	if (!stream)
		stream.clear();

	stream.ignore(1024, '\n');

	Soldier tempSoldier;

	for (int i = 0; i < soldiersCount; i++)
	{
		stream >> tempSoldier;

		tempSergeant.addSoldier(tempSoldier);
	}

	obj = tempSergeant;

	return stream;
}

std::ostream& operator << (std::ostream& stream, const Sergeant& obj)
{
	stream << (Soldier)obj;	
	stream << obj.description << '\n';
	stream << obj.soldiersCount << '\n';

	for (int i = 0; i < obj.soldiersCount;i++)
		stream << obj.soldiers[i];

	if (!stream)
	{
		stream.clear();
		throw "Error with writing in the file!";
	}

	return stream;
}

Sergeant::Sergeant() :
Soldier(NULL, 0, 0, 0),
description(NULL),
soldiersCount(0),
soldiers(NULL),
sizeOfSoldiers(0)
{

}

Sergeant::Sergeant(const char* name, int age, int fightSkills, int salary, const char* description) :
Soldier(name, age, fightSkills, salary),
description(NULL),
soldiersCount(0),
soldiers(NULL),
sizeOfSoldiers(0)
{
	if (description != NULL)
	{
		this->description = new char[strlen(description) + 1];

		strcpy(this->description, description);
	}

}

Sergeant::~Sergeant()
{
	this->cleanSergeant();
}

Sergeant::Sergeant(const Sergeant& obj) :
Soldier(obj),
description(NULL),
soldiersCount(0),
soldiers(NULL),
sizeOfSoldiers(0)
{
	this->copySergeant(obj);
}

Sergeant& Sergeant::operator= (const Sergeant& obj)
{
	if (this != &obj)
	{
		this->Soldier::operator= (obj);

		this->copySergeant(obj);
	}

	return *this;
}

void Sergeant::setSergeant(const char* name, int age, int fightSkills, int salary, const char* description)
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

void Sergeant::addSoldier(const Soldier& obj)
{
	if (findElement(this->soldiers, this->soldiersCount, obj) == UnvalidIndex)
	{
		if (this->fightSkills <= obj.Soldier::getFightSkills())
			throw "Sergeant's fight power must be more than soldier's fight power!";

		if (this->soldiersCount == this->sizeOfSoldiers)
		{
			resize(this->soldiers, this->sizeOfSoldiers, (3 * this->sizeOfSoldiers) / 2 + 2);

			this->sizeOfSoldiers = (3 * this->sizeOfSoldiers) / 2 + 2;
		}

		this->soldiers[this->soldiersCount++] = obj;
	}
	else
	{
		throw "You can't add one soldier twice ):";
	}
}

void Sergeant::removeSoldier(const Soldier& obj)
{
	if (this->soldiersCount <= 0)
		throw "There are no soldier to remove!";

	int index = findElement(this->soldiers, this->soldiersCount, obj);

	if (index != UnvalidIndex)
	{
		if (2 * this->soldiersCount == this->sizeOfSoldiers)
			resize(this->soldiers, this->soldiersCount, (5 * this->soldiersCount) / 4);

		this->sizeOfSoldiers = (5 * this->soldiersCount) / 4;

		Soldier temp(this->soldiers[index]);
		this->soldiers[index] = this->soldiers[this->soldiersCount - 1];
		this->soldiers[this->soldiersCount-- - 1] = temp;
	}
	else
	{
		throw "Can't find that soldier!";
	}
}

int Sergeant::getSoldiersCount() const
{
	return this->soldiersCount;
}

int Sergeant::getSalary() const
{
	return this->salary;
}
int Sergeant::getFightSkills() const
{
	return this->fightSkills;
}

const Soldier& Sergeant::operator[] (int index) const
{
	if (index < 0 || index >= this->soldiersCount)
		throw "Error! Wrong index!";

	return this->soldiers[index];
}

void Sergeant::cleanSergeant()
{
	delete[] this->description;
	this->description = NULL;

	delete[] this->soldiers;
	this->soldiers = NULL;

	this->soldiersCount = 0;
	this->sizeOfSoldiers = 0;
}

void Sergeant::copySergeant(const Sergeant& obj)
{
	char* description = NULL;

	if (obj.description != NULL)
	{
		description = new char[strlen(obj.description) + 1];

		strcpy(description, obj.description);	
	}

	Soldier* soldiers;

	try
	{
		soldiers = new Soldier[obj.soldiersCount];
	}
	catch (std::bad_alloc& ba)
	{
		delete[] description;
		throw;
	}

	for (int i = 0; i < obj.soldiersCount; i++)
	{
		try
		{
			soldiers[i] = obj.soldiers[i];
		}
		catch (std::bad_alloc& ba)
		{
			delete[] description;

			delete[] soldiers;

			throw;
		}
	}

	this->cleanSergeant();

	this->description = description;
	this->soldiers = soldiers;
	this->soldiersCount = obj.soldiersCount;
	this->sizeOfSoldiers = obj.soldiersCount;
}

//////////////////////////////////////////////////////////////


bool operator== (const Sergeant& obj1, const Sergeant& obj2)
{
	return operator==((Soldier)obj1, (Soldier)obj2);
}

bool operator!= (const Sergeant& obj1, const Sergeant& obj2)
{
	return !(obj1 == obj2);
}


//helpers
static void resize(Soldier*& array, int oldSize, int newSize)
{
	if (newSize < 0 || oldSize < 0)
		throw "Error! Invalid size!";

	if (newSize == 0)
	{
		array = NULL;
		return;
	}

	Soldier* newArray = new Soldier[newSize];

	for (int i = 0; i < oldSize && i < newSize; i++)
		newArray[i] = array[i];

	delete[] array;

	array = newArray;
}

static int findElement(const Soldier* array, int size, const Soldier& element)
{
	for (int i = 0; i < size; i++)
	{
		if (array[i] == element)
			return i;
	}

	return UnvalidIndex;
}