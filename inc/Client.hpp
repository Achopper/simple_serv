

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "serv_includes.hpp"
#include "Server.hpp"


class Response;

class Client
{

private:

	Server								_server;
	pollfd								*_setFd;
	std::string							_body; //TODO ?
	std::string 						_req;
	std::time_t 						_connectTime;
	bool 								_finishReadReq;
	Response							*_response;

public:

		std::string method;
		std::string path;
		std::string prot;
		//TODO test

	Client								( Server const & server, pollfd *set );
	Client								( Client const& obj );
	~Client								( void );

	Client&								operator=( Client const& obj );

public:

	void 	setBody						( std::string const & body );
	void 	setReq						( std::string const & req );
	void 	setSetFd					( pollfd  *setFd );
	void 	setConnTime					( void );
	void 	setFinishReadReq 			( bool isFinish );
	void 	setResponse					( Response & response );

	std::string getBody					( void ) const;
	std::string getReq					( void ) const;
	pollfd* getSetFd					( void ) const;
	std::time_t getConTime				( void ) const;
	bool	getFinishReadReq 			( void ) const;
	const Server & getServer			( void ) const;
	const Response * getResponse		( void ) const;

public:

	void deleteClient					( void );
	void makeResponse					( Response & response);

};

#endif //CLIENT_HPP
