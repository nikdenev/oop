#pragma once

#include "Egg.h"
#include <fstream>

typedef unsigned char byte;

const int UNVALID_INDEX = -1;

class Basket
{
public:
	Basket(const char* name);
	~Basket();
	Basket(const Basket& basket);
	Basket& operator = (const Basket& basket);

	const char* getName() const;
	int getEggsCount() const;

	void addEgg(Egg egg);
	void removeEgg(Egg egg);

	void report() const;

	void serialization() const;   
	void deserialization();

	Egg& operator[](int index);
	Egg operator[](int index) const;

	Egg& operator[](const char* str);
	Egg operator[](const char* str) const;

	Basket& operator+=(const Basket& basket);

	Basket& operator+=(const char* str);

	Basket& operator*=(int x);

	Basket& operator/=(int x);

	Basket& operator%=(const Basket& basket);

private:

	void swopEggs(int index1, int index2);
	int searchIndexOfEgg(Egg egg) const;
	void resize(int newSize);
	void clearBasket();

	int getSizeOfBasket() const;   

	bool basketIsFaund(std::ifstream& myFile) const; 

	void basketToArray(byte*& array, int& bytesCount) const;

	bool readBasketFromBinary(const char* name, std::ifstream& myFile);

	char* name;
	Egg* eggs;
	int eggsCount;
	int sizeOfEggs;
};

bool operator==(const Basket& basket1, const Basket& basket2);
bool operator!=(const Basket& basket1, const Basket& basket2);
bool operator>(const Basket& basket1, const Basket& basket2);
bool operator>=(const Basket& basket1, const Basket& basket2);
bool operator<(const Basket& basket1, const Basket& basket2);
bool operator<=(const Basket& basket1, const Basket& basket2);

Basket operator+(const Basket& basket1, const Basket& basket2);

Basket operator+(const Basket& basket, const char* str);
Basket operator+(const char* str, const Basket& basket);

Basket operator*(const Basket& basket, int x);
Basket operator*(int x, const Basket& basket);

Basket operator/(const Basket& basket ,int x);

Basket operator%(const Basket& basket1, const Basket& basket2);