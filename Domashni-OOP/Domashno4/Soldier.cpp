#include "Soldier.h"

std::istream& operator >> (std::istream& stream, Soldier& obj)
{
	char buffer[1024] = "UNKNOWN";

	stream.getline(buffer, 1024);
	if (!stream)
	{
		stream.clear();
		stream.ignore(1024, '\n');
	}

	int newAge = 0;

	int newFightSkills = 0;

	int newSalary = 0;

	stream >> newAge;
	if (!stream)
	{
		stream.clear();
		stream.ignore(1024, '\n');
	}

	stream >> newFightSkills;
	if (!stream)
	{
		stream.clear();
		stream.ignore(1024, '\n');
	}

	stream >> newSalary;
	if (!stream)
	{
		stream.clear();
	}

	stream.ignore(1024, '\n');

	obj.setSoldier(buffer, newAge, newFightSkills, newSalary);

	return stream;
}

std::ostream& operator << (std::ostream& stream, const Soldier& obj)
{
	stream << obj.name << '\n' << obj.age << '\n' 
		   << obj.fightSkills << '\n' << obj.salary << '\n';

	if (!stream)
	{
		stream.clear();
		throw "Error with writing in the file!";
	}

	return stream;
}

Soldier::Soldier() :
name(NULL),
age(0),
fightSkills(0),
salary(0)
{

}

Soldier::Soldier(const char* name, int age, int fightSkills, int salary) :
name(NULL)
{
	this->setSoldier(name, age, fightSkills, salary);
}

Soldier::~Soldier()
{
	delete[] name;
}          

Soldier::Soldier(const Soldier& obj) :
name(NULL)
{
	(*this).setSoldier(obj.name, obj.age, obj.fightSkills, obj.salary);
}

Soldier& Soldier::operator= (const Soldier& obj)
{
	if (this != &obj)
	{
		(*this).setSoldier(obj.name, obj.age, obj.fightSkills, obj.salary);  
	}

	return *this;
}

void Soldier::setSoldier(const char* name, int age, int fightSkills, int salary)
{
	if (age < 0 || fightSkills < 0 || salary < 0)
		throw "Error! Wrong data!";

	char* newName = NULL;

	if (name != NULL)
	{
		newName = new char[strlen(name) + 1];

		strcpy(newName, name);
	}

	delete[] this->name;

	this->name = newName;

	this->age = age;
	this->fightSkills = fightSkills;
	this->salary = salary;
}

const char* Soldier::getName() const
{
	return this->name;
}

int Soldier::getAge() const
{
	return this->age;
}

int Soldier::getFightSkills() const
{
	return this->fightSkills;
}

int Soldier::getSalary() const
{
	return this->salary;
}

//////////////////////////////////////////////////////////////

bool operator== (const Soldier& obj1, const Soldier& obj2)
{
	return strcmp(obj1.getName(), obj2.getName()) == 0 &&
		   obj1.getAge() == obj2.getAge() &&
		   obj1.getFightSkills() == obj2.getFightSkills() &&
		   obj1.getSalary() == obj2.getSalary();
}

bool operator!= (const Soldier& obj1, const Soldier& obj2)
{
	return !(obj1 == obj2);
}
