#include "Mage.h"

#include <iostream>

static const int UnvalidIndex = -1;

//helpers
static void resize(Sergeant*& array, int oldSize, int newSize);
static int findElement(const Sergeant* array, int size, const Sergeant& element);


std::istream& operator >> (std::istream& stream, Mage& obj)
{
	Mage tempMage;

	stream >> (Soldier&)tempMage;

	char buffer[1024] = "Description unknown.";

	stream.getline(buffer, 1024);
	if (!stream)
	{
		stream.clear();
		stream.ignore(1024, '\n');
	}

	int mana = 0;

	stream >> mana;
	if (!stream)
		stream.clear();

	stream.ignore(1024, '\n');

	tempMage.description = new char[strlen(buffer) + 1];
	strcpy(tempMage.description, buffer);
	tempMage.mana = mana;

	stream >> tempMage.magicBook;

	int sergeantsCount = 0;

	stream >> sergeantsCount;
	if (!stream)
		stream.clear();

	stream.ignore(1024, '\n');

	Sergeant tempSergeant;

	for (int i = 0; i < sergeantsCount; i++)
	{
		stream >> tempSergeant;

		tempMage.addSergeant(tempSergeant);
	}

	obj = tempMage;

	return stream;

}

std::ostream& operator << (std::ostream& stream, const Mage& obj)
{
	stream << (Soldier)obj;
	stream << obj.description << '\n';
	stream << obj.mana << '\n';
	stream << obj.magicBook;
	stream << obj.sergeantsCount << '\n';

	for (int i = 0; i < obj.sergeantsCount; i++)
		stream << obj.sergeants[i];

	if (!stream)
	{
		stream.clear();
		throw "Error with writing in the file!";
	}

	return stream;
}

Mage::Mage() :
description(NULL), 
mana(0),
sergeants(NULL),
sergeantsCount(0),
sizeOfSergeants(0)
{

}

Mage::Mage(const char* name, int age, int fightSkills, int salary,
	const char* description, int mana, const MagicBook& magicBook) :

Soldier(name, age, fightSkills, salary),
description(NULL),
mana(mana),
sergeants(NULL),
sergeantsCount(0),
sizeOfSergeants(0)
{
	if (mana < 0)
		throw "Error! Unvalid mana!";

	if (description != NULL)
	{
		this->description = new char[strlen(description) + 1];

		strcpy(this->description, description);
	}

	this->magicBook = magicBook;
}

Mage::~Mage()
{
	this->cleanMage();
}

Mage::Mage(const Mage& obj) :
Soldier(obj),
description(NULL),
mana(0),
sergeants(NULL),
sergeantsCount(0),
sizeOfSergeants(0)
{
	this->copyMage(obj);
}

Mage& Mage::operator= (const Mage& obj)
{
	if (this != &obj)
	{
		this->Soldier::operator= (obj);

		this->copyMage(obj);
	}

	return *this;
}

void Mage::setMage(const char* name, int age, int fightSkills, int salary,
	const char* description, int mana, const MagicBook& magicBook)
{
	if (mana < 0)
		throw "Error! Unvalid mana!";

	this->Soldier::setSoldier(name, age, fightSkills, salary);

	this->magicBook = magicBook;

	char* newDescription;

	if (description != NULL)
	{
		newDescription = new char[strlen(description) + 1];

		strcpy(newDescription, description);
	}

	delete[] this->description;

	this->description = newDescription;
	this->mana = mana;
}

void Mage::addSergeant(const Sergeant& obj)
{
	if (findElement(this->sergeants, this->sergeantsCount, obj) == UnvalidIndex)
	{
		if (this->fightSkills <= obj.Soldier::getFightSkills())
			throw "Mage's fight power must be more than sergeant's fight power!";

		if (this->sergeantsCount == this->sizeOfSergeants)
		{
			resize(this->sergeants, this->sizeOfSergeants, (3 * this->sizeOfSergeants) / 2 + 2);

			this->sizeOfSergeants = (3 * this->sizeOfSergeants) / 2 + 2;
		}

		this->sergeants[this->sergeantsCount++] = obj;
	}
	else
	{
		throw "You can't add one sergeant twice ):";
	}
}

void Mage::removeSergeant(const Sergeant& obj)
{
	if (this->sergeantsCount <= 0)
		throw "There are no sergeants to remove!";

	int index = findElement(this->sergeants, this->sergeantsCount, obj);

	if (index != UnvalidIndex)
	{
		if (2 * this->sergeantsCount == this->sizeOfSergeants)
			resize(this->sergeants, this->sergeantsCount, (5 * this->sergeantsCount) / 4);

		this->sizeOfSergeants = (5 * this->sergeantsCount) / 4;

		Sergeant temp(this->sergeants[index]);
		this->sergeants[index] = this->sergeants[this->sergeantsCount - 1];
		this->sergeants[this->sergeantsCount-- - 1] = temp;
	}
	else
	{
		throw "Can't find that sergeant!";
	}
}

int Mage::getSergeantsCount() const
{
	return this->sergeantsCount;
}

int Mage::getSalary() const
{
	return this->salary;
}

int Mage::getFightSkills() const
{
	return this->fightSkills;
}

int Mage::getMana() const
{
	return this->mana;
}

const Sergeant& Mage::operator[] (int index) const
{
	if (index < 0 || index >= this->sergeantsCount)
		throw "Error! Wrong index!";

	return this->sergeants[index];
}

const MagicBook& Mage::getMagicBook() const
{
	return this->magicBook;
}

void Mage::cleanMage()
{
	delete[] this->description;
	this->description = NULL;

	this->mana = 0;

	delete[] this->sergeants;
	this->sergeants = NULL;

	this->sergeantsCount = 0;
	this->sizeOfSergeants = 0;
}

void Mage::copyMage(const Mage& obj)
{
	this->magicBook = obj.magicBook;

	char* description = NULL;

	if (obj.description != NULL)
	{
		description = new char[strlen(obj.description) + 1];

		strcpy(description, obj.description);
	}

	Sergeant* sergeants;

	try
	{
		sergeants = new Sergeant[obj.sergeantsCount];
	}
	catch (std::bad_alloc& ba)
	{
		delete[] description;
		throw;
	}

	for (int i = 0; i < obj.sergeantsCount; i++)
	{
		try
		{
			sergeants[i] = obj.sergeants[i];
		}
		catch (std::bad_alloc& ba)
		{
			delete[] description;

			delete[] sergeants;

			throw;
		}
	}

	this->cleanMage();

	this->description = description;
	this->mana = obj.mana;
	//Magicheskata kniga prisvoih nai otgore
	this->sergeants = sergeants;
	this->sergeantsCount = obj.sergeantsCount;
	this->sizeOfSergeants = obj.sizeOfSergeants;
}

//////////////////////////////////////////////////////////////


bool operator== (const Mage& obj1, const Mage& obj2)
{
	return operator== ((Soldier)obj1, (Soldier)obj2);
}

bool operator!= (const Mage& obj1, const Mage& obj2)
{
	return !(obj1 == obj2);
}

//////////////////////////////////////////////////////////////

static void resize(Sergeant*& array, int oldSize, int newSize)
{
	if (newSize < 0 || oldSize < 0)
		throw "Error! Invalid size!";

	if (newSize == 0)
	{
		array = NULL;
		return;
	}

	Sergeant* newArray = new Sergeant[newSize];

	for (int i = 0; i < oldSize && i < newSize; i++)
		newArray[i] = array[i];

	delete[] array;

	array = newArray;
}

static int findElement(const Sergeant* array, int size, const Sergeant& element)
{
	for (int i = 0; i < size; i++)
	{
		if (array[i] == element)
			return i;
	}

	return UnvalidIndex;
}