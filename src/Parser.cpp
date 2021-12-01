#include "../inc/Parser.hpp"

Parser::Parser		( void )
{

}

Parser::Parser::~Parser		( void )
{

}

Parser::Parser		( Parser const& obj )
{
	*this = obj;
}

Parser&		Parser::operator=( Parser const& obj )
{
	// if (this != &obj)
	// {
	// 	// _server = obj._server;
	// }
	return (*this);
}


void		Parser::setUrl()
{

}

void		Parser::setBuf()
{

}

void		Parser::setBody()
{

}

void		Parser::setMethod()
{

}

void		Parser::setHttpVersion()
{

}

void		Parser::setQueryString()
{

}


std::string	Parser::getUrl()
{

}

std::string	Parser::getBuf()
{

}

std::string	Parser::getBody()
{

}

std::string	Parser::getMethod()
{

}

std::string	Parser::getHttpVersion()
{

}

std::string	Parser::getQueryString()
{

}
