#include <iostream>
#include <array>

#include "../inc/Core.hpp"

int main()
{
	Core core;

	try
	{
		core.startServ();
	}catch (std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	return 0;
}
