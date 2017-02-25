#pragma once

#include "Soldier.h"

class Sergeant : public Soldier
{
	friend std::istream& operator >> (std::istream& stream, Sergeant& obj);
	friend std::ostream& operator << (std::ostream& stream, const Sergeant& obj);

public:
	Sergeant();
	Sergeant(const char* name, int age, int fightSkills, int salary, const char* description);
	~Sergeant();
	Sergeant(const Sergeant& obj);
	Sergeant& operator= (const Sergeant& obj);

	void setSergeant(const char* name, int age, int fightSkills, int salary, const char* description);

	void addSoldier(const Soldier& obj);
	void removeSoldier(const Soldier& obj);

	int getSoldiersCount() const;
	int getSalary() const;
	int getFightSkills() const;

	const Soldier& operator[] (int index) const;

private:
	void cleanSergeant();
	void copySergeant(const Sergeant& obj);

private:
	char* description;
	int soldiersCount;
	Soldier* soldiers;
	int sizeOfSoldiers;
};

bool operator== (const Sergeant& obj1, const Sergeant& obj2);
bool operator!= (const Sergeant& obj1, const Sergeant& obj2);

