
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
	std::string 					_root;
	std::string 					_response;

public:

	Server							( void );
	Server							( Server const& obj );
	~Server							( void );
	Server& 						operator=( Server const& obj );

public:

	bool	setServIp				( std::string const & ip );
	bool 	setPort					( std::string const & port);
	void 	setServName				( std::string const & servName);
	bool 	setAddr					( std::string const &addr, std::string::size_type &pos );
	void 	setServerFd				( pollfd *fdSet );
	void 	setRoot					( std::string const &root );

	std::string		getServIp		( void ) const;
	std::string 	getPort			( void ) const;
	std::string 	getServName		( void ) const;
	sockaddr_in		getAddr			( void ) const;
	pollfd *		getServerFd		( void ) const;
	std::string 	getRoot			( void ) const;

public:

};


#endif //SERVER_HPP
