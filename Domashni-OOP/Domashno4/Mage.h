#pragma once

#include "Soldier.h"
#include "Sergeant.h"
#include "MagicBook.h"

class Mage : public Soldier
{
	friend std::istream& operator >> (std::istream& stream, Mage& obj);
	friend std::ostream& operator << (std::ostream& stream, const Mage& obj);

public:
	Mage();
	Mage(const char* name, int age, int fightSkills, int salary,
		 const char* description, int mana, const MagicBook& magicBook);
	~Mage();
	Mage(const Mage& obj);
	Mage& operator= (const Mage& obj);

	void setMage(const char* name, int age, int fightSkills, int salary,
				 const char* description, int mana, const MagicBook& magicBook);

	void addSergeant(const Sergeant& obj);
	void removeSergeant(const Sergeant& obj);

	int getSergeantsCount() const;
	int getSalary() const;
	int getFightSkills() const;

	int getMana() const;

	const MagicBook& getMagicBook() const;

	const Sergeant& operator[] (int index) const;

private:
	void cleanMage();
	void copyMage(const Mage& obj);

private:
	char* description;
	int mana;
	MagicBook magicBook;
	int sergeantsCount;
	Sergeant* sergeants;
	int sizeOfSergeants;
};

bool operator== (const Mage& obj1, const Mage& obj2);
bool operator!= (const Mage& obj1, const Mage& obj2);
