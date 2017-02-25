#pragma once

#include <iostream>

class Spell
{
	friend std::istream& operator >> (std::istream& stream, Spell& obj);
	friend std::ostream& operator << (std::ostream& stream, const Spell& obj);

public:
	Spell();
	Spell(const char* description, const char* type, int castingCost);
	~Spell();
	Spell(const Spell& obj);
	Spell& operator= (const Spell& obj);

	void setSpell(const char* description, const char* type, int castingCost);

	const char* getDescription() const;
	const char* getType() const;
	int getCastingCost() const;

private:
	char* description;
	char* type;
	int castingCost;
};


//Dve zaklinaniq sa ravni <=> vsichkite im danni sa ednakvi
bool operator== (const Spell& obj1, const Spell& obj2);
bool operator!= (const Spell& obj1, const Spell& obj2);

//Dve zaklinaniq se sravnqvat po nujnata za izpulnenieto na zaklinanieto mana
bool operator> (const Spell& obj1, const Spell& obj2);		
bool operator>= (const Spell& obj1, const Spell& obj2);		

bool operator< (const Spell& obj1, const Spell& obj2);
bool operator<= (const Spell& obj1, const Spell& obj2);