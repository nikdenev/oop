#pragma once

#include <iostream>
#include <cstring>

class Egg
{
public:
	Egg();
	Egg(const char* newName, int newSize);
	~Egg();
	Egg(const Egg& egg);
	Egg& operator=(const Egg& egg);

	void setName(const char* newName);
	void setSize(int newSize);
	void setEgg(const char* newName, int newSize);
	
	const char* getName() const;
	double getSize() const;

	bool strInEggName(const char* str) const;

	void clearEgg();

	void print() const;

	Egg& operator+=(const char* str);
	Egg& operator+=(int size);

	Egg& operator*=(int x);
	Egg& operator/=(int x);

private:
	char* name;
	int size;
};

bool operator==(const Egg& egg1, const Egg& egg2);
bool operator!=(const Egg& egg1, const Egg& egg2);
bool operator>(const Egg& egg1, const Egg& egg2);
bool operator>=(const Egg& egg1, const Egg& egg2);
bool operator<(const Egg& egg1, const Egg& egg2);
bool operator<=(const Egg& egg1, const Egg& egg2);

Egg operator+(const Egg& egg, const char* str);
Egg operator+(const char* str, const Egg& egg);

Egg operator+(const Egg& egg, int size);
Egg operator+(int size, const Egg& egg);

Egg operator*(const Egg& egg, int x);
Egg operator*(int x, const Egg& egg);
Egg operator/(const Egg& egg, int x);

