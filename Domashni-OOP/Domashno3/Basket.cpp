#include "Basket.h"

#include <iostream>
#include <cstring>
#include <fstream>
#include <cassert>

//helpers
static void concatenateArray(byte*& arrayOfBaskets, byte*& basketsArray, int sizeOfarray, int getSizeOfBasket);

static bool getNameAndSizeOfBasket(char*& nameOfBasket, int& getSizeOfBasket, std::ifstream& myFile);

Basket::Basket(const char* name) :
name(NULL),
eggs(NULL),
eggsCount(0),
sizeOfEggs(0)
{
	this->name = new char[strlen(name) + 1];

	strcpy(this->name, name);
}

Basket::~Basket()
{
	clearBasket();
}

Basket::Basket(const Basket& basket) :
name(NULL),
eggs(NULL),
eggsCount(basket.eggsCount),
sizeOfEggs(basket.sizeOfEggs)
{
	this->name = new char[strlen(basket.name) + 1];
	
	strcpy(this->name, basket.name);

	try
	{
		this->eggs = new Egg[basket.sizeOfEggs];
	}
	catch (std::bad_alloc& ba)
	{
		delete[] this->name;
		throw;
	}

	for (int i = 0; i < this->eggsCount; i++)
	{
		try
		{
			this->eggs[i] = basket.eggs[i];
		}
		catch (std::bad_alloc& ba)
		{
			clearBasket();
			throw;
		}
	}
}

Basket& Basket::operator= (const Basket& basket)
{
	if (this == &basket)
		return *this;

	char* name = new char[strlen(basket.name) + 1];

	strcpy(name, basket.name);

	try
	{
		Egg* eggs = new Egg[basket.sizeOfEggs];
	}
	catch (std::bad_alloc& ba)
	{
		delete[] name;
		throw;
	}

	for (int i = 0; i < basket.eggsCount; i++)
	{
		try
		{
			eggs[i] = basket.eggs[i];
		}
		catch (std::bad_alloc& ba)
		{
			delete[] name;
			delete[] eggs;
			throw;
		}
	}
		
	clearBasket();

	this->name = name;
	this->eggs = eggs;
	this->eggsCount = basket.eggsCount;
	this->sizeOfEggs = basket.sizeOfEggs;

	return *this;
}

const char* Basket::getName() const
{
	return this->name;
}

int Basket::getEggsCount() const
{
	return this->eggsCount;
}

void Basket::addEgg(Egg egg) 
{
	if (searchIndexOfEgg(egg) == UNVALID_INDEX)
	{
		if (this->eggsCount == this->sizeOfEggs)
			resize(2 * this->eggsCount + 2);

		this->eggs[this->eggsCount] = egg;
		this->eggsCount++;
	}
	else
		std::cout << "Can't add " << egg.getName() << " again!\n";
}

void Basket::removeEgg(Egg egg)  
{
	if (this->eggsCount == 0)
	{
		std::cout << "The basket is empty! You can't remove egg \""
				  << egg.getName() << "\" from the basket!\n";
	}
	else
	{
		int indexOfTheEgg = searchIndexOfEgg(egg);

		if (indexOfTheEgg == UNVALID_INDEX)
			std::cout << "Can't find " << egg.getName() << '\n';
		else
		{
			swopEggs(indexOfTheEgg, this->eggsCount - 1);

			this->eggs[this->eggsCount - 1].clearEgg();

			this->eggsCount--;

			if (2 * this->eggsCount + 2 == this->sizeOfEggs)
				resize(this->eggsCount);
		}
	}
}

void Basket::report() const
{
	char* nameOfFile = new char[strlen("report_.txt") + strlen(this->name) + 1];

	strcpy(nameOfFile, "report_");
	strcat(nameOfFile, this->name);
	strcat(nameOfFile, ".txt");

	std::ofstream myFile(nameOfFile, std::ios::out | std::ios::trunc);

	delete[] nameOfFile;

	if (!myFile)
		throw "Error! Problem with opening report file!";

	if (this->eggsCount == 0)
	{
		myFile << "The basket is empty!\n";
		if (!myFile)
			throw "Error! Problem with writing in report file!";
	}
	else
	{
		for (int i = 0; i < this->eggsCount; i++)
		{
			myFile << this->eggs[i].getName() << ' ' << this->eggs[i].getSize() << '\n';
			if (!myFile)
				throw "Error! Problem with writing in report file!";
		}
	}

	myFile.close();
}

void Basket::serialization() const
{
	std::ifstream readingFile("binaryFile.dat", std::ios::binary | std::ios::in);

	if (readingFile&&basketIsFaund(readingFile))
	{
		byte* arrayOfBaskets = NULL;
		int sizeOfArrayOfBaskets = 0;

		readingFile.seekg(0, std::ios::beg);
		while (readingFile)
		{
			char* nameOfBasket = NULL;

			byte* basketsArray = NULL;
			int getSizeOfBasket = 0;

			try
			{
				if (!getNameAndSizeOfBasket(nameOfBasket, getSizeOfBasket, readingFile))
					break;
			}catch(std::bad_alloc& ba)
			{
				delete[] arrayOfBaskets;
				throw;
			}

			if (strcmp(this->name, nameOfBasket))
			{
				try
				{
					Basket temp(nameOfBasket);
					temp.deserialization();     

					int UNUSED = 0;
					temp.basketToArray(basketsArray, UNUSED);			
				
					concatenateArray(arrayOfBaskets, basketsArray, sizeOfArrayOfBaskets, getSizeOfBasket);
				}
				catch(const char*& str)
				{
					delete[] basketsArray;
					delete[] nameOfBasket;
					delete[] arrayOfBaskets;
					throw;
				}
				catch(std::bad_alloc& ba)
				{
					delete[] basketsArray;
					delete[] nameOfBasket;
					delete[] arrayOfBaskets;
					throw;
				}

				sizeOfArrayOfBaskets +=getSizeOfBasket;
			}

			delete[] nameOfBasket;
			delete[] basketsArray;
		}

		if (!readingFile.eof())
		{
			delete[] arrayOfBaskets;
			throw "Error! Problem with \"binaryFile.dat\" file!";
		}
		
		readingFile.close();

		std::ofstream writingFile("binaryFile.dat", std::ios::binary | std::ios::trunc | std::ios::out);
		if (!writingFile)
		{
			delete[] arrayOfBaskets;
			throw "Error! Problem with opening the \"binaryFile.dat\" file!";
		}

		writingFile.write((char*)arrayOfBaskets, sizeOfArrayOfBaskets);

		delete[] arrayOfBaskets;

		if (!writingFile)
			throw "Error! Problem with writing in \"binaryFile.dat\" file!";

		writingFile.close();
	}

	readingFile.close();

	std::ofstream writingFile("binaryFile.dat", std::ios::binary | std::ios::out | std::ios::ate | std::ios::app);
	if (!writingFile)
		throw "Error! Problem with opening the \"binaryFile.dat\" file!";
	
	byte* tempArray = NULL;
	int tempSize = 0;

	basketToArray(tempArray, tempSize);

	writingFile.write((char*)tempArray, tempSize);

	delete[] tempArray;

	if (!writingFile)
		throw "Error! Problem with writing in \"binaryFile.dat\" file!";

	writingFile.close();
}


void Basket::deserialization()
{
	std::ifstream myFile("binaryFile.dat", std::ios::binary | std::ios::in);
	if (myFile)
	{
		bool isFind = false;

		while (myFile.good())
		{
			char* nameOfBasket = NULL;
			int getSizeOfBasket = 0;

			if (!getNameAndSizeOfBasket(nameOfBasket, getSizeOfBasket, myFile))
				break;

			if (strcmp(this->name, nameOfBasket) == 0)
			{
				myFile.seekg(0 - getSizeOfBasket, std::ios::cur);
				myFile.seekg(sizeof(int) + strlen(nameOfBasket), std::ios::cur);
				
				bool tempReturnedValue;
				try
				{
					tempReturnedValue = readBasketFromBinary(nameOfBasket, myFile);
				}catch (std::bad_alloc& ba)
				{
					delete[] nameOfBasket;
					throw;
				}

				if (tempReturnedValue)
				{
					delete[] nameOfBasket;
					isFind = true;
					break;
				}
				else
				{
					delete[] nameOfBasket;
					throw "Error! Problem with reading from \"binaryFile.dat\" file!";
				}
			}
			else
				delete[] nameOfBasket;
		}

		if (!myFile && !myFile.eof() && !isFind)
			throw "Error! Problem with \"binaryFile.dat\" file!";

		myFile.close();

		if (!isFind)
			std::cout << "Can't find " << name << " basket!\n";
	}
	else
	{
		std::cout << "There is no \"binaryFile.dat\" file!\n";
	}
}

void Basket::swopEggs(int index1, int index2)
{
	assert(index1 >= 0 &&
		index2 >= 0 &&
		this->eggsCount > index1 &&
		this->eggsCount > index2);

	if (index1 != index2)
	{
		Egg temp = this->eggs[index1];

		this->eggs[index1] = this->eggs[index2];

		this->eggs[index2] = temp;
	}
}

int Basket::searchIndexOfEgg(Egg egg) const
{
	for (int i = 0; i < this->eggsCount; i++)
	{
		if (this->eggs[i] == egg)
		{
			return i;
		}
	}

	return UNVALID_INDEX;
}

void Basket::resize(int newSize)
{
	assert(newSize >= 0);

	if (newSize == 0)
	{
		delete[] this->eggs;
		this->eggs = NULL;
	}
	else
	{
		Egg* newEggs = new Egg[newSize];
		
		for (int i = 0; i < this->eggsCount && i < newSize; i++)
		{
			try
			{
				newEggs[i] = this->eggs[i];
			}
			catch (std::bad_alloc& ba)
			{
				delete[] newEggs;
				throw;
			}
		}

		delete[] this->eggs;

		this->eggs = newEggs;

		this->sizeOfEggs = newSize;
	}
}

void Basket::clearBasket()
{
	delete[] this->name;
	this->name = NULL;
	delete[] this->eggs;
	this->eggs = NULL;
}

bool Basket::readBasketFromBinary(const char* name, std::ifstream& myFile)
{
	if (!myFile)
		return false;

	char* newName = new char[strlen(name) + 1];

	strcpy(newName, name);

	int eggsCount;

	myFile.read((char*)&eggsCount, sizeof(int));
	if (!myFile)
	{
		delete[] newName;
		return false;
	}

	Egg* eggs;
	try
	{		
		eggs = new Egg[eggsCount];
	}catch(std::bad_alloc& ba)
	{
		delete[] newName;
		throw;
	}

	for (int i = 0; i < eggsCount; i++)
	{
		int eggNameLength;

		myFile.read((char*)&eggNameLength, sizeof(int));
		if (!myFile)
		{
			delete[] newName;
			delete[] eggs;
			return false;
		}

		char* tempName;
		try
		{
			tempName = new char[eggNameLength + 1];
		}catch(std::bad_alloc& ba)
		{
			delete[] newName;
			delete[] eggs;
			throw;
		}

		myFile.read(tempName, eggNameLength);
		if (!myFile)
		{
			delete[] newName;
			delete[] eggs;
			delete[] tempName;
			return false;
		}

		tempName[eggNameLength] = '\0';

		int tempSize;

		myFile.read((char*)&tempSize, sizeof(int));
		if (!myFile)
		{
			delete[] newName;
			delete[] eggs;
			delete[] tempName;
			return false;
		}

		eggs[i].setEgg(tempName, tempSize);

		delete[] tempName;
	}

	clearBasket();  

	this->name = newName;
	this->eggs = eggs;
	this->eggsCount = eggsCount;
	this->sizeOfEggs = eggsCount;

	return true;
}

void Basket::basketToArray(byte*& array, int& bytesCount) const
{
	bytesCount = getSizeOfBasket();

	array = new byte[bytesCount];

	int readedBytes = 0;

	int nameLength = strlen(this->name);

	memcpy(array + readedBytes, (byte*)&nameLength, sizeof(int));
	readedBytes += sizeof(int);

	memcpy(array + readedBytes, (byte*)this->name, nameLength);
	readedBytes += nameLength;

	memcpy(array + readedBytes, (byte*)&(this->eggsCount), sizeof(int));
	readedBytes += sizeof(int);

	for (int i = 0; i < this->eggsCount; i++)
	{
		int eggNameLength = strlen(this->eggs[i].getName());

		memcpy(array + readedBytes, (byte*)&eggNameLength, sizeof(int));
		readedBytes += sizeof(int);

		memcpy(array + readedBytes, this->eggs[i].getName(), eggNameLength);
		readedBytes += eggNameLength;

		int temporarySize = this->eggs[i].getSize();

		memcpy(array + readedBytes, (byte*)&temporarySize, sizeof(int));
		readedBytes += sizeof(int);
	}

	assert(readedBytes == bytesCount);
}

int Basket::getSizeOfBasket() const
{
	int bytesCount = 0;

	bytesCount += sizeof(int)+strlen(this->name) + sizeof(int);

	for (int i = 0; i < this->eggsCount; i++)
		bytesCount += sizeof(int)+strlen(this->eggs[i].getName()) + sizeof(int);

	return bytesCount;
}

bool Basket::basketIsFaund(std::ifstream& myFile) const
{
	while(myFile)
	{
		char* tempName = NULL;

		int UNUSED = 0;
		if(!getNameAndSizeOfBasket(tempName, UNUSED, myFile))
			break;

		if (strcmp(this->name, tempName)==0)
		{
			delete[] tempName;
			return true;
		}
			
		delete[] tempName;
	}

	if (!myFile.eof())
		throw "Error! Problem with the \"binaryFile.dat\" file!";

	myFile.close();

	return false;
}

//Oprerators which are in the class
Egg& Basket::operator[](int index) 
{
	if (index < 0 || index >= this->eggsCount)
		throw "Error! Oprerator[] failed! Wrong index of egg!";                          

	return this->eggs[index];
}

Egg Basket::operator[](int index) const
{
	if (index < 0 || index >= this->eggsCount)
		throw "Error! Oprerator[] failed! Wrong index of egg!";                          

	return this->eggs[index];
}

Egg& Basket::operator[](const char* str) 
{
	for (int i = 0; i < this->eggsCount; i++)
	{
		if (this->eggs[i].strInEggName(str))
			return this->eggs[i];
	}

	throw "Can't find egg which has that string!";
}

Egg Basket::operator[](const char* str) const
{
	for (int i = 0; i < this->eggsCount; i++)
	{
		if (this->eggs[i].strInEggName(str))
			return this->eggs[i];
	}

	throw "Can't find egg which has that string!";
}

Basket& Basket::operator+=(const Basket& basket)
{
	int cnt = basket.eggsCount;

	for (int i = 0; i < cnt; i++)
		addEgg(basket.eggs[i]);

	return *this;
}

Basket& Basket::operator+=(const char* str)
{
	for (int i = 0; i < this->eggsCount; i++)
		this->eggs[i] += str;

	return *this;
}

Basket& Basket::operator*=(int x)
{
	if (x < 0)
		throw "The size of the egg must be >= 0!";

	for (int i = 0; i < this->eggsCount; i++)
		this->eggs[i] *= x;

	return *this;
}

Basket& Basket::operator/=(int x)
{
	if (x <= 0)
		throw "Can't devide by 0 and the size of the egg must be >= 0!";

	for (int i = 0; i < this->eggsCount; i++)
		this->eggs[i] /= x;

	return *this;
}

Basket& Basket::operator%=(const Basket& basket)
{
	for (int i = 0; i < this->eggsCount; i++)
	{
		int j = 0;
		for (; j < basket.eggsCount; j++)
		{
			if (eggs[i] == basket[j])
				break;
		}
		if (j == basket.eggsCount)
			removeEgg(this->eggs[i--]);
	}

	return *this;
}

//Operators which are out of the class
bool operator==(const Basket& basket1, const Basket& basket2)    
{
	int eggsCount1 = basket1.getEggsCount();

	if (eggsCount1 != basket2.getEggsCount())
		return false;

	for (int i = 0; i < eggsCount1; i++)
	{
		if (basket1[i] != basket2[i])
			return false;
	}

	return true;
}

bool operator!=(const Basket& basket1, const Basket& basket2)
{
	return !(basket1 == basket2);
}

bool operator>(const Basket& basket1, const Basket& basket2)
{
	int eggsCount1 = basket1.getEggsCount();
	int eggsCount2 = basket2.getEggsCount();

	for (int i = 0; i < eggsCount1 && i < eggsCount2; i++)
	{
		if (basket1[i] != basket2[i])
		{
			if (basket1[i]>basket2[i])
				return true;
			else
				return false;
		}
	}

	if (eggsCount1 == eggsCount2)
		return false;
	else
	{
		if (eggsCount1 > eggsCount2)
			return true;

		return false;
	}
}

bool operator>=(const Basket& basket1, const Basket& basket2)
{
	return basket1 == basket2 || basket1 > basket2;
}

bool operator<(const Basket& basket1, const Basket& basket2)
{
	return !(basket1 >= basket2);
}

bool operator<=(const Basket& basket1, const Basket& basket2)
{
	return !(basket1>basket2);
}

Basket operator+(const Basket& basket1, const Basket& basket2)
{
	Basket temp(basket1);

	temp += basket2;

	return temp;
}

Basket operator+(const Basket& basket, const char* str)
{
	Basket temp(basket);

	temp += str;

	return temp;
}

Basket operator+(const char* str, const Basket& basket)
{
	return basket + str;
}

Basket operator*(const Basket& basket, int x)
{
	Basket temp(basket);

	temp *= x;

	return temp;
}

Basket operator*(int x, const Basket& basket)
{
	return basket*x;
}

Basket operator/(const Basket& basket, int x)
{
	Basket temp(basket);

	temp /= x;

	return temp;
}

Basket operator%(const Basket& basket1, const Basket& basket2)
{
	Basket temp(basket1);

	temp %= basket2;

	return temp;
}


//helpers
static bool getNameAndSizeOfBasket(char*& nameOfBasket, int& getSizeOfBasket, std::ifstream& myFile)
{
	int nameLength;

	myFile.read((char*)&nameLength, sizeof(int));
	if (!myFile)
		return false;

	getSizeOfBasket += sizeof(int);

	char* name = new char[nameLength + 1];

	myFile.read(name, nameLength);
	if (!myFile)
	{
		delete[] name;
		return false;
	}

	getSizeOfBasket += nameLength;

	name[nameLength] = '\0';

	nameOfBasket = name;

	int eggsCount;

	myFile.read((char*)&eggsCount, sizeof(int));
	if (!myFile)
	{
		delete[] name;
		return false;
	}

	getSizeOfBasket += sizeof(int);

	for (int i = 0; i < eggsCount; i++)
	{
		int eggNameLength;

		myFile.read((char*)&eggNameLength, sizeof(int));
		if (!myFile)
		{
			delete[] name;
			return false;
		}

		getSizeOfBasket += sizeof(int) + eggNameLength + sizeof(int);

		myFile.seekg(eggNameLength + sizeof(int), std::ios::cur);
	}

	if (!myFile)
	{
		delete[] name;
		return false;
	}

	return true;
}

static void concatenateArray(byte*& arrayOfBaskets, byte*& basketsArray, int sizeOfArray, int getSizeOfBasket)
{
	byte* newArray = new byte[sizeOfArray + getSizeOfBasket];
		
	memcpy(newArray, arrayOfBaskets, sizeOfArray);

	memcpy(newArray + sizeOfArray, basketsArray, getSizeOfBasket);

	delete[] arrayOfBaskets;

	arrayOfBaskets = newArray;
}