#include "Functions.h"

int main()
{
	try
	{
		Commander commander;
	
		std::ifstream file("Commanders_File.txt", std::ios::in);
		if (!file)
		{
			std::cerr << "Error! Can't open the file!\n";
			return 1;
		}
		
		file >> commander;			

		if (!file && !file.eof())
		{
			std::cerr << "Error! Problem with reading from file!\n";
			return 1;
		}

		file.close();

		function(&commander);
	}
	catch (std::bad_alloc& ba)
	{
		std::cout << ba.what() << '\n';
	}
	catch (const char* what)
	{
		std::cout << what << '\n';
	}

	return 0;
}