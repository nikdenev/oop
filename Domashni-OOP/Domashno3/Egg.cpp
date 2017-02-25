#include "Egg.h"

#include <iostream>
#include <cstring>

static const double EPS = 1e-6;

//G4 begin
Egg::Egg() :
name(NULL), size(0)
{

}

Egg::Egg(const char* newName, int newSize)
{
	this->name = new char[strlen(newName) + 1];

	strcpy(this->name, newName);

	this->size = newSize;
}

Egg::~Egg()
{
	delete[] this->name;
}

Egg::Egg(const Egg& egg)
{
	this->name = new char[strlen(egg.name) + 1];

	strcpy(this->name, egg.name);

	this->size = egg.size;
}

Egg& Egg::operator=(const Egg& egg)
{
	if (this == &egg)
		return *this;

	setName(egg.name);
	setSize(egg.size);
}

void Egg::setName(const char* newName)
{
	char* name = new char[strlen(newName) + 1];

	strcpy(name, newName);
	
	delete[] this->name;

	this->name = name;
}

void Egg::setSize(int newSize)
{
	this->size = newSize;
}

void Egg::setEgg(const char* newName, int newSize)
{
	setName(newName);
	setSize(newSize);
}

const char* Egg::getName() const
{
	return this->name;
}

double Egg::getSize() const
{
	return this->size;
}

bool Egg::strInEggName(const char* str) const
{
	int nameLength = strlen(this->name);
	int strLength = strlen(str);

	for (int i = 0; i < nameLength - strLength; i++)
	{
		int j = 0;
		for (; j < strLength; j++)
		{
			if (str[j] != this->name[i + j])
				break;
		}
		if (j == strLength)
			return true;
	}

	return false;
}

void Egg::clearEgg()
{
	delete[] this->name;

	this->name = NULL;

	this->size = 0;
}

void Egg::print() const
{
	std::cout << this->name << " " << this->size << '\n';
}

//Operators whitch are in the class
Egg& Egg::operator+=(const char* str)
{
	char* name = new char[strlen(this->name) + strlen(str) + 1];

	strcpy(name, this->name);
	strcat(name, str);

	delete[] this->name;

	this->name = name;

	return *this;
}

Egg& Egg::operator+=(int size)
{
	setSize(this->size + size);

	return *this;
}


Egg& Egg::operator*=(int x)
{
	if (x < 0)
		throw "The egg must have positive value!";

	this->size *= x;

	return *this;
}

Egg& Egg::operator/=(int x)
{
	if (x <= 0)
		throw "Can't devide by 0 and the egg must have size >= 0!";

	this->size /= x;

	return *this;
}
//They ends

//Out of the class
bool operator==(const Egg& egg1, const Egg& egg2)
{
	return strcmp(egg1.getName(), egg2.getName()) == 0;
}

bool operator!=(const Egg& egg1, const Egg& egg2)
{
	return !(egg1 == egg2);
}

bool operator>(const Egg& egg1, const Egg& egg2)
{
	if (strcmp(egg1.getName(), egg2.getName()) > 0)   
		return true;

	return false;
}


bool operator>=(const Egg& egg1, const Egg& egg2)
{
	return egg1 > egg2 || egg1 == egg2;
}

bool operator<(const Egg& egg1, const Egg& egg2)
{
	return !(egg1 >= egg2);
}

bool operator<=(const Egg& egg1, const Egg& egg2)
{
	return !(egg1 > egg2);
}

Egg operator+(const Egg& egg, const char* str)
{
	Egg temp(egg);

	temp += str;

	return temp;
}

Egg operator+(const char* str, const Egg& egg)
{
	return egg + str;
}

Egg operator+(const Egg& egg, int size)
{
	Egg temp(egg);

	temp += size;

	return temp;
}

Egg operator+(int size, const Egg& egg)
{
	return egg+size;
}

Egg operator*(const Egg& egg, int x)
{
	Egg temp(egg); 

	temp *= x;

	return temp;
}

Egg operator*(int x, const Egg& egg)
{
	return egg*x;
}

Egg operator/(const Egg& egg, int x)
{
	Egg temp(egg);

	temp /= x;

	return temp;
}

