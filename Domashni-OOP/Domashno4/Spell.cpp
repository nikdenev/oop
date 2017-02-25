#include "Spell.h"

std::istream& operator >> (std::istream& stream, Spell& obj)
{
	char bufferForDesc[1024] = "There is no description";
	char bufferForType[1024] = "UNKNOWN";

	stream.getline(bufferForDesc, 1024);
	if (!stream)
	{
		stream.clear();
		stream.ignore(1024, '\n');
	}

	stream.getline(bufferForType, 1024);
	if (!stream)
	{
		stream.clear();
		stream.ignore(1024, '\n');
	}

	int newCastingCost = 0;

	stream >> newCastingCost;
	if (!stream)
		stream.clear();

	stream.ignore(1024, '\n');

	obj.setSpell(bufferForDesc, bufferForType, newCastingCost);

	return stream;
}

std::ostream& operator << (std::ostream& stream, const Spell& obj)
{
	stream << obj.description << '\n' << obj.type << '\n' << obj.castingCost << '\n';

	if (!stream)
	{
		stream.clear();
		throw "Error! Problem with writing in the file!";
	}

	return stream;
}

Spell::Spell() :
description(NULL),
type(NULL),
castingCost(0)
{

}

Spell::Spell(const char* description, const char* type, int castingCost) :
description(NULL),
type(NULL),
castingCost(0)
{
	(*this).setSpell(description, type, castingCost);
}

Spell::~Spell()
{
	delete[] description;
	delete[] type;
}

Spell::Spell(const Spell& obj) :
description(NULL),
type(NULL),
castingCost(0)
{
	(*this).setSpell(obj.description, obj.type, obj.castingCost);
}

Spell& Spell::operator= (const Spell& obj)
{
	if (this != &obj)
	{
		(*this).setSpell(obj.description, obj.type, obj.castingCost);
	}

	return *this;
}

void Spell::setSpell(const char* description, const char* type, int castingCost)
{
	if (castingCost < 0)
		throw "Error! Unvalid casting cost!";

	char* newDescription = NULL;
	char* newType = NULL;

	if (description != NULL)
	{
		newDescription = new char[strlen(description) + 1];

		strcpy(newDescription, description);
	}

	if (type != NULL)
	{
		newType = new char[strlen(type) + 1];

		strcpy(newType, type);
	}

	delete[] this->description;
	delete[] this->type;

	this->description = newDescription;
	this->type = newType;
	this->castingCost = castingCost;
}

const char* Spell::getDescription() const
{
	return this->description;
}

const char* Spell::getType() const
{
	return this->type;
}

int Spell::getCastingCost() const
{
	return this->castingCost;
}

//////////////////////////////////////////////////////////////

//Dve zaklinaniq sa ravni <=> vsichkite im danni sa ednakvi
bool operator== (const Spell& obj1, const Spell& obj2)
{
	return strcmp(obj1.getDescription(), obj2.getDescription()) == 0 &&
		   strcmp(obj1.getType(), obj2.getType()) == 0 &&
		   obj1.getCastingCost() == obj2.getCastingCost();

}

bool operator!= (const Spell& obj1, const Spell& obj2)
{
	return !(obj1 == obj2);
}

//Dve zaklinaniq se sravnqvat po nujnata za izpulnenieto na zaklinanieto mana
bool operator> (const Spell& obj1, const Spell& obj2)
{
	return obj1.getCastingCost() > obj2.getCastingCost();
}

bool operator>= (const Spell& obj1, const Spell& obj2)
{
	return obj1.getCastingCost() >= obj2.getCastingCost();
}

bool operator< (const Spell& obj1, const Spell& obj2)
{
	return obj1.getCastingCost() < obj2.getCastingCost();
}

bool operator<= (const Spell& obj1, const Spell& obj2)
{
	return obj1.getCastingCost() <= obj2.getCastingCost();
}