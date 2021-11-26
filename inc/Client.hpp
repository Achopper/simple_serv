

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "serv_includes.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include <map>

#define PARSE_FIRST_LINE 1
#define PARSE_HEADERS 2
#define PARSE_BODY 3
#define PARSE_END 4


class Client
{

private:

	Request					_request;
	//Response				_resp

	// Parser					_parser();
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

	void 					setBody( std::string const & body );
	void 					setReq( std::string const & req );
	void					setSetFd( pollfd  *setFd );
	std::string				getBody( void ) const;
	std::string				getReq( void ) const;
	pollfd* 				getSetFd( void ) const;
	std::string				getReq( void );
	

public:
	void 	deleteClient	( void );

public:
	
	void					setRequest(std::string _req);
	Request&				getRequest( void );

};

std::vector<std::string> split2(const std::string& str, const std::string& delim) ;

#endif //CLIENT_HPP
