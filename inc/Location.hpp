
#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "serv_includes.hpp"

class Location {
private:

	std::string 								_path;
	std::map<std::string, bool> 				_methods;
	size_t 										_clientMaxBodySize;
	std::string 								_index;
	std::string									_root;
	bool 										_autoindex;

public:
	Location									( void );
	Location									( Location const & obj );
	~Location									( void );

	Location & 									operator=( Location const & obj );

public:

	bool		setPath							( std::string const & path );
	bool 		setAutoindex					( std::string const & autoindex );
	bool		setMethods						( std::string const & methods );
	bool 		setClientBodySize				( std::string const & num );
	bool		setIndex						( std::string const & index );
	bool 		setRoot							( std::string const & root );

	bool 		getAutoindex					( void ) const;
	std::string getPath							( void ) const;
	std::string getIndex						( void ) const;
	std::string getRoot							( void ) const;
	std::map<std::string, bool>	getMethods		( void ) const;
	size_t 		getClientSize					( void ) const;

};

#endif //LOCATION_HPP