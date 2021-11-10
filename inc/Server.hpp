
#ifndef SERVER_HPP
#define SERVER_HPP

#include "serv_includes.hpp"

typedef struct sockaddr_in sockaddr_in;


class Server
{
private:

	sockaddr_in 					_addr;
	std::string						_servIp;
	pollfd							*_serverFd;
	std::string						_serverName;
	std::string						_port;
	std::string 					_response;
	int 							_listenFd; //TODO mb del?

public:

	Server							( void );
	Server							( Server const& obj );
	~Server							( void );

	Server& 						operator=( Server const& obj );

	bool	setServIp				( std::string const & ip );
	bool 	setPort					( std::string const & port);
	void 	setServName				( std::string const & servName);
	bool 	setAddr					( std::string const &addr, std::string::size_type &pos );
	void 	setFd					( int const & fd);
	void 	setServerFd				( pollfd *fdSet );

	std::string		getServIp		( void ) const;
	std::string 	getPort			( void ) const;
	std::string 	getServName		( void ) const;
	sockaddr_in		getAddr			( void ) const;
	int 			getFd			( void ) const;
	pollfd *		getServerFd		( void ) const;

};


#endif //SERVER_HPP
