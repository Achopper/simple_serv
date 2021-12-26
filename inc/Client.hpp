

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "serv_includes.hpp"
#include "Server.hpp"
#include "Request.hpp"

#include "Env.hpp"
#include "Response.hpp"


class Client
{

private:

	Server								_server;
	pollfd								*_setFd;
	std::string 						_req;
	std::time_t 						_connectTime;
	bool 								_finishReadReq;
	Response							_response;
	Request								_request;

public:

	Client								( Server const & server, pollfd *set );
	Client								( Client const& obj );
	~Client								( void );



	Client&								operator=( Client const& obj );

public:

	void 	setReq						( std::string const & req );
	void 	setSetFd					( pollfd  *setFd );
	void 	setConnTime					( void );
	void 	setFinishReadReq 			( bool isFinish );
	void 	setResponse					( Response & response );

	std::string getReq					( void ) const;
	pollfd* getSetFd					( void ) const;
	std::time_t getConTime				( void ) const;
	bool	getFinishReadReq 			( void ) const;
	const Server & getServer			( void ) const;
	Response * getResponse				( void );

public:

	void deleteClient					( void );
	void makeResponse					( void );
public:
	
	void					setRequest	( std::string const & req );
	Request&				getRequest	( void );
};

std::vector<std::string> split2			(const std::string& str, const std::string& delim) ;

#endif //CLIENT_HPP
