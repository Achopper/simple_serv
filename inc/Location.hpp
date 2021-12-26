
#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "serv_includes.hpp"

class Location {
private:


	std::string 								_name;
	std::string 								_alias;
	std::string 								_path;
	std::map<std::string, bool> 				_methods;
	size_t 										_clientMaxBodySize;
	std::string 								_index;
	std::string									_root;
	bool 										_autoindex;
	bool 										_redirect;
	std::string 								_cgi;
	std::string 								_pathToRedirect;


public:
	Location							( void );
	Location							( Location const & obj );
	~Location							( void );

	Location & 							operator=( Location const & obj );

public:


	bool 		setName							( std::string const & name );
	bool 		setAlias						( std::string const & alias );

	bool 		setAutoindex					( std::string const & autoindex );
	bool		setMethods						( std::string const & methods );
	bool 		setClientBodySize				( std::string const & num );

	bool		setIndex						( std::string const & index );
	bool 		setRoot							( std::string const & root );
	bool 		setRedirect						( std::string const & path );
	bool		setPath							( std::string const & serverRoot, std::string & err );
	bool 		setCgi							( std::string const & cgiExtension );


	const std::string & getName					( void ) const;
	bool 		getAutoindex					( void ) const;
	std::string getPath							( void ) const;
	std::string getIndex						( void ) const;
	std::string getRoot							( void ) const;
	std::map<std::string, bool>	getMethods		( void ) const;
	size_t 		getClientSize					( void ) const;
	const std::string & getPathToRedir			( void ) const;
	const std::string & getCgi					( void ) const;
	bool isRedirect								( void ) const;

};

#endif //LOCATION_HPP
