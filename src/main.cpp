#include <iostream>
#include <array>

#include "../inc/Core.hpp"


int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << REDCOL"Too many arguments, type path to config file" << RESCOL << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string pathToConf = (argc == 1) ? "./config/def.conf" : argv[1];
	Config config(pathToConf);
	if (config.getErrors().size() > 1)
	{
		std::cerr << config.getErrors() << std::endl;
		return (EXIT_FAILURE);
	}
	Core core(config);

	try
	{
		core.startServ();
	}catch (std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	return 0;
}


