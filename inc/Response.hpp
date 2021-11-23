

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "serv_includes.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Client;
class Server;

class Response
{
private:

	std::string 						_code;
	std::string 						_response;
	std::string							_body;
	std::string							_method;

public:

	Response							( std::string &method );
	Response							( Response const & obj );
	~Response							( void );

	Response &							operator=( Response const & obj );

public:

	bool 		setResp					( std::string const & response );
	bool 		setBody					( std::string const & body );
	void 		setCode					( std::string const & code );

	const std::string & getResp			(void ) const;
	std::string			getCode			( void ) const;
	const std::string &	getBody			( void ) const;
	std::string			getMethod		( void ) const;

	bool 		GET						( Client & client);
	bool 		getPage					( std::string const & path );



};


#endif //RESPONSE_HPP
