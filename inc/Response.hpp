

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "serv_includes.hpp"
#include "Server.hpp"
#include "DefaultPage.hpp"
#include "Request.hpp"
#include "Autoindex.hpp"
#include "Env.hpp"

class Server;

class Response
{
private:

	std::string 										_code;
	std::string 										_response;
	std::string											_body;
	std::string											_method;
	Server												_server;
	Request												_request;
	size_t 												_maxLen;
	Env													_env;
	static std::map<std::string, std::string> 			_statusCodes;
	static std::map<std::string, std::string> 			_contentType;

private:

	static std::map<std::string, std::string>setStatusCode	( void );
	static std::map<std::string, std::string>setContentType	( void );

public:

	Response											( void );
	Response											( Server &server, Request & request);
	Response											( Response const & obj );
	~Response											( void );

	Response &											operator=( Response const & obj );

public:

	bool 		setResp									( std::string const & response );
	bool 		setBody									( std::string const & body );
	void 		setCode									( std::string const & code );
	void 		setServer								( Server const & server );
	void 		setRequest								( Request const & request );

	const std::string & getResp							( void ) const;
	std::string			getCode							( void ) const;
	const std::string &	getBody							( void ) const;
	std::string			getMethod						( void ) const;
	Env&				getEnv							( void );

	void 		addCodetoResp							( std::string const &code );
	void 		addContentLen							( std::string::size_type const &len );
	void 		addContentType							( std::string const & filePath );
	void 		addServerName							( std::string const & serverName );
	bool		makeCgi									( int &socket, Location const &location);
	bool 		cgiCall									( int socket, const char *body, Location const &location);

	bool 		GET										( int & sock );
	bool 		HEAD									( int & sock );
	bool		DELETE									( void );
	bool 		POST									( int & sock );
	bool 		getPage									( std::string const & path );
	void 		fillResponse							( void );
	bool		checkLocation							(  std::vector<Location>::const_iterator &lit,
															std::string const &reqPath, std::string & url, bool & isF);
	void  makeRedirect									( std::vector<Location> const & locList,
														  std::vector<Location>::const_iterator & it, std::string &url);
};

#include "Core.hpp"

#endif //RESPONSE_HPP
