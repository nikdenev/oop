#pragma once

#include <iostream>
#include <fstream>

class Soldier
{
	friend std::istream& operator >> (std::istream& stream, Soldier& obj);
	friend std::ostream& operator << (std::ostream& stream, const Soldier& obj);

public:
	Soldier();
	Soldier(const char* name, int age, int fightSkills, int salary);
	~Soldier();
	Soldier(const Soldier& obj);
	Soldier& operator= (const Soldier& obj);

	void setSoldier(const char* name, int age, int fightSkills, int salary);

	const char* getName() const;
	int getAge() const;
	int getFightSkills() const;
	int getSalary() const;

protected:
	char* name;
	int age;
	int fightSkills;
	int salary;
};


bool operator== (const Soldier& obj1, const Soldier& obj2);
bool operator!= (const Soldier& obj1, const Soldier& obj2);



