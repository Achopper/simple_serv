

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "serv_includes.hpp"
#include "Server.hpp"

class Client
{

private:

	bool									_isUrlSet;
	bool									_isMetodSet;
	bool									_isHttpVerSet;
	bool									_isFirstLineSet;

	std::string								_method;
	std::string								_url;
	std::string								_httpVersion;
	//std::map<std::string, std::string>	headers; // header -> content
	std::string								_reqBody;
	std::string 							_queryString;
	std::string								_buf;
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

	void 	setBody			( std::string const & body );
	void 	setReq			( std::string const & req );
	void 	setSetFd		( pollfd  *setFd );
	std::string getBody		( void ) const;
	std::string getReq		( void ) const;
	pollfd* getSetFd		( void ) const;
	

public:
	void 	deleteClient	( void );

public:

	void					setUrl();
	void					setBuf();
	void					setReqBody();
	void					setMethod();
	void					setHttpVersion();
	void					setQueryString();

	std::string				getUrl();
	std::string				getBuf();
	std::string				getReqBody();
	std::string				getMethod();
	std::string				getHttpVersion();
	std::string				getQueryString();

	void					parseReq();


};


#endif //CLIENT_HPP
