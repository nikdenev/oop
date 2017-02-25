#include "MagicBook.h"
#include "Spell.h"

#include <iostream>

static const int UnvalidIndex = -1;

//helpers
static void resize(Spell*& array, int oldSize, int newSize);
static int findElement(const Spell* array, int size, const Spell& element);


std::istream& operator >> (std::istream& stream, MagicBook& obj)
{
	MagicBook tempBook;

	int spellsCount = 0;

	stream >> spellsCount;
	if (!stream)
		stream.clear();

	stream.ignore(1024, '\n');

	Spell tempSpell;

	for (int i = 0; i < spellsCount; i++)
	{
		stream >> tempSpell;

		tempBook.addSpell(tempSpell);
	}

	obj = tempBook;

	return stream;
}

std::ostream& operator << (std::ostream& stream, const MagicBook& obj)
{
	stream << obj.spellsCount << '\n';

	for (int i = 0; i < obj.spellsCount; i++)
	{
		stream << obj.spells[i];
	}

	if (!stream)
	{
		stream.clear();
		throw "Error! Problem with writing in the file!";
	}

	return stream;
}


MagicBook::MagicBook() :
spellsCount(0)
{
	this->spells = new Spell[1];
}

MagicBook::~MagicBook()
{
	delete[] spells;
}

MagicBook::MagicBook(const MagicBook& obj)
{
	Spell* newSpells = new Spell[obj.spellsCount + 1];

	for (int i = 0; i < obj.spellsCount; i++)
		newSpells[i] = obj.spells[i];

	this->spells = newSpells;
	
	this->spellsCount = obj.spellsCount;
}

MagicBook& MagicBook::operator= (const MagicBook& obj)
{
	if (this != &obj)
	{
		Spell* newSpells = new Spell[obj.spellsCount + 1];

		for (int i = 0; i < obj.spellsCount; i++)
			newSpells[i] = obj.spells[i];

		delete[] this->spells;

		this->spells = newSpells;

		this->spellsCount = obj.spellsCount;
	}

	return *this;
}

void MagicBook::addSpell(const Spell& spell)
{
	this->spells[this->spellsCount] = spell;

	resize(this->spells, this->spellsCount + 1, this->spellsCount + 2);

	this->spellsCount++;

	this->sortSpells();
}

void MagicBook::removeSpell(const char* description)
{
	bool isFound = false;

	for (int i = 0; i < this->spellsCount; i++)
	{
		if (strcmp(this->spells[i].getDescription(), description) == 0)
		{
			this->swapSpells(i, this->spellsCount - 1);

			resize(this->spells, this->spellsCount + 1, this->spellsCount);

			this->spellsCount--;

			isFound = true;

			break;
		}
	}

	if (!isFound)
		throw "Can't find that spell => you can't remove it!";

}

void MagicBook::sortSpells()
{
	for (int i = 1; i < this->spellsCount; i++)
	{
		for (int j = i; j > 0; j--)
		{
			if (this->spells[j] < this->spells[j - 1])
				this->swapSpells(j, j - 1);
			else
				break;
		}
	}
}

int MagicBook::getSpellsCount() const
{
	return this->spellsCount;
}

const Spell& MagicBook::operator[] (int index) const
{
	if (index < 0 || index >= this->spellsCount)
		throw "Error! Wrong index!";

	return this->spells[index];
}

void MagicBook::swapSpells(int i, int j)
{
	Spell temp(this->spells[i]);

	this->spells[i] = this->spells[j];

	this->spells[j] = temp;
}

//////////////////////////////////////////////////////////////

static void resize(Spell*& array, int oldSize, int newSize)
{
	if (newSize < 0 || oldSize < 0)
		throw "Error! Invalid size!";

	if (newSize == 0)
	{
		array = NULL;
		return;
	}

	Spell* newArray = new Spell[newSize];

	for (int i = 0; i < oldSize && i < newSize; i++)
		newArray[i] = array[i];

	delete[] array;

	array = newArray;
}

static int findElement(const Spell* array, int size, const Spell& element)
{
	for (int i = 0; i < size; i++)
	{
		if (array[i] == element)
			return i;
	}

	return UnvalidIndex;
}