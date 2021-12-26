
#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include "serv_includes.hpp"

class Autoindex
{
private:
	std::vector<std::string> 			_names;
	std::string 						_page;
public:
	Autoindex							( void );
	~Autoindex							( void );

	bool makePage						( std::string const & path , std::string const & locPath);
	void getNames						( DIR *dir );

	const std::string & getPage			( void );
};


#endif //AUTOINDEX_HPP
