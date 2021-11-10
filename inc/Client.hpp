

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "serv_includes.hpp"
#include "Server.hpp"

class Client
{

private:

	Server					_server;
	pollfd					*_setFd;
	std::string				_body;
	std::string 			_req;

public:

	Client					( /*const int& srvFd,*/ Server const & server, pollfd *set );
	Client					( Client const& obj );
	~Client					( void );

	Client&					operator=( Client const& obj );

public:

	void 	setBody			( std::string const & body );
	void 	setReq			( std::string const & req );
	void 	setSetFd		( pollfd  *setFd );
	std::string getBody		( void ) const;
	std::string getReq		( void ) const;
	pollfd* getSetFd		( void ) const;

public:
	void deleteClient		( void );







};


#endif //CLIENT_HPP
