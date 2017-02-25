#pragma once

#include "Soldier.h"
#include "Mage.h"

class Commander : public Soldier
{
	friend std::istream& operator >> (std::istream& stream, Commander& obj);
	friend std::ostream& operator << (std::ostream& stream, const Commander& obj);

public:
	Commander();
	Commander(const char* name, int age, int fightSkills, int salary, const char* description);
	~Commander();
	Commander(const Commander& obj);
	Commander& operator= (const Commander& obj);


	void setCommander(const char* name, int age, int fightSkills, int salary, const char* description);

	void addMage(const Mage& obj);
	void removeMage(const Mage& obj);

	int getMagesCount() const;
	int getSalary() const;
	int getFightSkills() const;

	const Mage& operator[] (int index) const;

private:
	void cleanCommander();
	void copyCommander(const Commander& obj);

private:
	char* description;
	int magesCount;
	Mage* mages;
	int sizeOfMages;
};

