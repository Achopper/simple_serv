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

		void								setEnvArr();
		// void								addServEnvToMap(Server &server);
		void								addHttpEnvToMap(Request &request);

	private:
		std::map<std::string, std::string>	_envMap;
		// std::string	;
		char								**_envArr;
};

#endif //ENV_HPP