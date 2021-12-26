#ifndef ENV_HPP
#define ENV_HPP

#include "serv_includes.hpp"
#include "Request.hpp"
#include "Server.hpp"

class Env
{
	public:
		Env												( void );
		~Env();
		Env(Env const &other);
		Env &operator=(Env const &other);
public:


		std::map<std::string, std::string>	&getEnvMap();
		char								**getEnvArr();
		size_t								&getArrRows();

		void								setEnvArr();
		
		// void								addServEnvToMap(Server &server);
		void								addHttpEnvToMap(Request &request);
		void								addEnvToMap();
		char								**makeEnvArr();
		void 	setServer					(Server const *server);

	private:

		std::map<std::string, std::string>	_envMap;
		char								**_envArr;
		size_t								_arrRows;
		const Server*						_server;
};

#endif //ENV_HPP