#include "Basket.h"
#include "Egg.h"

#include <iostream>

int main()
{
	try
	{
		Egg g1("g1", 5);
		Egg g2("g2", 65);
		Egg g3("g3", 54);
		Egg g4("g4", 25);
		Egg g5("g5", 15);
		Egg g6("g6", 50);
		Egg g7("g7", 650);
		Egg g8("g8", 540);
		Egg g9("g9", 250);
		Egg g10("g10", 150);

		Basket x1("Stavrito");

		x1.addEgg(g1);

		x1.addEgg(g2);

		x1.addEgg(g3);

		x1.addEgg(g4);

		x1.addEgg(g5);

		x1.removeEgg(g2);

		x1.serialization();

		Basket x2("Icaka");

		x2.addEgg(g1);

		x2.addEgg(g2);

		x2.addEgg(g3);

		x2.addEgg(g4);

		x2 %= x1;

		x2*=3;

		x2.report();

		std::cout<<(x1>x2)<<'\n';

		x2.serialization();
	}
	catch (const char*& what)
	{
		std::cout << what << '\n';
	}
	catch (std::bad_alloc& ba)
	{
		std::cout<<ba.what()<<'\n';
	}
			

	return 0;
}