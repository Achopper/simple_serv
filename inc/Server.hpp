
#ifndef SERVER_HPP
#define SERVER_HPP

#include "serv_includes.hpp"
#include "Location.hpp"

class Server
{
private:

	sockaddr_in 										_addr;
	std::string											_servIp;
	pollfd												*_serverFd;
	std::string											_serverName;
	std::string											_port;
	std::string 										_root;
	std::vector<Location>								_locList;
	std::map<std::string, std::string> 					_errorPages;

public:

	Server												( void );
	Server												( Server const& obj );
	~Server												( void );
	Server& 											operator=( Server const& obj );

public:

	bool	setServIp									( std::string const & ip );
	bool 	setPort										( std::string const & port);
	bool 	setServName									( std::string const & servName);
	bool 	setAddr										( std::string const &addr, std::string::size_type &pos );
	void 	setServerFd									( pollfd *fdSet );
	bool 	setRoot										( std::string const &root );
	void	setLocList									( std::vector<Location> const &locList) ;
	void 	setLocList									( Location const & location );
	bool 	setErrorPage								( std::string const & address,
														  std::string const & code, std::string  & error);

	std::string		getServIp							( void ) const;
	std::string 	getPort								( void ) const;
	std::string 	getServName							( void ) const;
	sockaddr_in		getAddr								( void ) const;
	pollfd *		getServerFd							( void ) const;
	std::string 	getRoot								( void ) const;
	const std::vector<Location> & getLocList			( void ) const;
	const std::map<std::string, std::string>&getErrPage	( void ) const;

};


#endif //SERVER_HPP
