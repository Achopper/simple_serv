#ifndef ENV_HPP
#define ENV_HPP

#include "serv_includes.hpp"

class Env
{
	public:
		Env												( void );
		~Env();
		Env(Env const &other);
		Env &operator=(Env const &other);
public:

		std::map<std::string, std::string>	&getEnvMap	( void );
		char	**getEnvArr								( void );

		void	setEnvArr								( void );
		void	setServer								( Server const & server );
		//void	addServEnvToMap							( Server &server );
		void	addHttpEnvToMap							( Request &request );

	private:
		std::map<std::string, std::string>				_envMap;
		Server											_server;
		char											**_envArr;
};

#endif //ENV_HPP