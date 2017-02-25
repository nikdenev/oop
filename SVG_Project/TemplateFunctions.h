#include <iostream>

template <class T>
void resize(T*& array, int oldSize, int newSize)
{
	if (newSize < 0 || oldSize < 0)
		throw "Error! Invalid size!";

	if (newSize == 0)
	{
		array = NULL;
		return;
	}

	T* newArray = new T[newSize];

	for (int i = 0; i < oldSize && i < newSize; i++)
		newArray[i] = array[i];

	delete[] array;

	array = newArray;
}

template <class T>
void swap(T& obj1, T& obj2)
{
	T temp;

	temp = obj1;

	obj1 = obj2;

	obj2 = temp;
}