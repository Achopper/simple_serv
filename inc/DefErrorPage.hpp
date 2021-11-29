
#ifndef DEFERRORPAGE_HPP
#define DEFERRORPAGE_HPP
#include "serv_includes.hpp"

class DefErrorPage
{
public:

	DefErrorPage();

	~DefErrorPage();

	std::string makePage(std::string const &errorCode, std::string const & errmsg, std::string const &servVer);

};


#endif //DEFERRORPAGE_HPP
