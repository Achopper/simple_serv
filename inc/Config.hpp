

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "serv_includes.hpp"
#include "Server.hpp"
#include "Location.hpp"

class Server;


class Config
{
private:

	std::string 						_conf;
	std::string 						_err;
	std::vector<Server> 				_servers;
	uint32_t 							_servCount;

public:

	Config								( std::string const &conf );
	Config								( Config const &obj );
	~Config								( void );
	Config &							operator=( Config const &obj );

public:

	std::string 		getErrors		( void ) const;
	std::vector<Server>	getServers		( void ) const;
	std::string 		getConf			( void ) const;
	uint32_t 			getServCount	( void ) const;

	void				setServer		( Server &server );

public:

	std::string 	readConfig			( std::string const &conf);
	void 			parseConfig			( void );

protected:

	std::vector<std::string> preParse	( std::string const &toParse );
	std::vector<std::string> splitConf	( void );
	bool 		parseServerBlock		( std::vector<std::string> &conf, std::vector<std::string>::iterator &word );
	bool 		parseLocationBlock		( std::vector<std::string> &conf, std::vector<std::string>::iterator &word,
											 Server & server);
	bool 		checkSemicolon			( std::vector<std::string>::iterator &word );
	bool 		checkUnicLocation		( Server const & server ) const;

};


#endif //CONFIG_HPP
