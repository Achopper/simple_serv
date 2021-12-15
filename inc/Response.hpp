

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

// #include "serv_includes.hpp"
// #include "Server.hpp"
// #include "Client.hpp"
#include "DefaultPage.hpp"

class Client;
class Server;
//class DefaultPage;

class Response
{
private:

	std::string 										_code;
	std::string 										_response;
	std::string											_body;
	std::string											_method;
	Client												_client;
	static std::map<std::string, std::string> 			_statusCodes;
	static std::map<std::string, std::string> 			_contentType;



private:

	static std::map<std::string, std::string>setStatusCode	( void );
	static std::map<std::string, std::string>setContentType	( void );

public:

	Response											( std::string method,Client & client );
	Response											( Response const & obj );
	~Response											( void );

	Response &											operator=( Response const & obj );

public:

	bool 		setResp									( std::string const & response );
	bool 		setBody									( std::string const & body );
	void 		setCode									( std::string const & code );

	const std::string & getResp							( void ) const;
	std::string			getCode							( void ) const;
	const std::string &	getBody							( void ) const;
	std::string			getMethod						( void ) const;

	void 		addCodetoResp							( std::string const &code );
	void 		addContentLen							( std::string::size_type const &len );
	void 		addContentType							( std::string const & filePath );
	void 		addServerName							( std::string const & serverName );

	bool 		GET										( Client & client);
	bool 		getPage									( std::string const & path );
	void 		fillResponse							( void );



};

#include "Core.hpp"

#endif //RESPONSE_HPP
