
#include "../inc/Client.hpp"

Client::Client (const int &srvFd, Server const & server)
: _server(server), _servFd(srvFd)
{
}

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
		_servFd = obj._servFd;
		_setFd = obj._setFd;
		_body = obj._body;
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

void Client::setSetFd( pollfd & setFd )
{
	_setFd = setFd;
}

int Client::getServFd() const
{
	return (_servFd);
}

//int Client::getCliFd() const
//{
//	return (_clientFd);
//}

std::string Client::getBody() const
{
	return (_body);
}

std::string Client::getReq(void) const
{
	return (_req);
}

pollfd Client::getSetFd(void) const
{
	return (_setFd);
}




