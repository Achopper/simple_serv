
#include "../inc/Client.hpp"

Client::Client (Server const & server, pollfd* set)
: 	_server(server),
	_setFd(set)
{}

Client::Client(const Client &obj)
{
	*this = obj;
}

Client::~Client(void)
{
}

Client &Client::operator=(const Client &obj)
{
	if (this != &obj)
	{
		_server = obj._server;
		_setFd = obj._setFd;
		_body = obj._body;
		_req = obj._req;
	}
	return (*this);
}

void Client::setReq(std::string const & req)
{
	_req = req;
}

void Client::setBody(const std::string &body)
{
	_body = body;
}

void Client::setSetFd( pollfd  *setFd )
{
	_setFd = setFd;
}

std::string Client::getBody() const
{
	return (_body);
}

std::string Client::getReq(void) const
{
	return (_req);
}

pollfd* Client::getSetFd(void) const
{
	return (_setFd);
}

void Client::deleteClient()
{
	std::cout << REDCOL"Client " << _setFd->fd  << " disconnected" << RESCOL << std::endl;
	close(_setFd->fd);
	_setFd->fd = -1;
}

std::string	Client::getReq( void )
{
	return (_req) ;
}

Request&	Client::getRequest( void )
{
	return (_request) ;
}

void	Client::setRequest(std::string _req)
{
	_request.parseReq(_req);
}
