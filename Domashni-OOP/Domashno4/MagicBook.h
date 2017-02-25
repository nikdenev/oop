#pragma once

#include "Spell.h"

class MagicBook
{
	friend std::istream& operator >> (std::istream& stream, MagicBook& obj);
	friend std::ostream& operator << (std::ostream& stream, const MagicBook& obj);

public:
	MagicBook();
	~MagicBook();
	MagicBook(const MagicBook& obj);
	MagicBook& operator= (const MagicBook& obj);

	void addSpell(const Spell& spell);
	void removeSpell(const char* description);

	void sortSpells();

	int getSpellsCount() const;

	const Spell& operator[] (int index) const;

private:
	void swapSpells(int i, int j);

private:
	Spell* spells;
	int spellsCount;
};

