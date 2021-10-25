
#ifndef SERVER_HPP
#define SERVER_HPP

#include "serv_includes.hpp"
//#include "Config.hpp"
//class Config;

typedef struct sockaddr_in sockaddr_in;


class Server
{
private:

	sockaddr_in 					_addr;
	std::string						_servIp;
	std::string						_serverName;
	std::string						_port;

public:

	Server							( void );
	Server							( Server const& obj );
	~Server							( void );

	Server& 						operator=( Server const& obj );

	void	setServIp				( std::string const & ip );
	void 	setPort					( std::string const & port);
	void 	setServName				( std::string const & servName);
	void 	setAddr					( void );

	std::string		getServIp		( void ) const;
	std::string 	getPort			( void ) const;
	std::string 	getServName		( void ) const;
	sockaddr_in		getAddr			( void ) const;

};


#endif //SERVER_HPP
