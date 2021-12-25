#ifndef ENV_HPP
#define ENV_HPP

#include "Core.hpp"

class Env
{
	public:
		Env();
		~Env();
		Env(Env const &other);
		Env &operator=(Env const &other);

		std::map<std::string, std::string>	&getEnvMap();
		char								**getEnvArr();
		size_t								&getArrRows();

		void								setEnvArr();
		
		// void								addServEnvToMap(Server &server);
		void								addHttpEnvToMap(Request &request);
		void								addEnvToMap();
		char								**makeEnvArr();

	private:

		std::map<std::string, std::string>	_envMap;
		char								**_envArr;
		size_t								_arrRows;
};

#endif //ENV_HPP