
#include "../inc/Server.hpp"

Server::Server(void) : _servIp(""), _serverName(""), _port("")
{
}

Server::~Server(void)
{
}

Server::Server(const Server &obj)
{
	*this = obj;
}

Server &Server::operator=(const Server &obj)
{
	if (this != &obj)
	{
		_servIp = obj._servIp;
		_serverName = obj._serverName;
		_port = obj._port;
	}
	return *this;
}

void Server::setServIp(std::string const & ip)
{
	_servIp = ip;
}

void Server::setPort(const std::string &port)
{
	_port = port;
}

void Server::setServName(const std::string &servName)
{
	_serverName = servName;
}

std::string Server::getServIp(void) const
{
	return (_servIp);
}

std::string Server::getPort(void) const
{
	return (_port);
}

std::string Server::getServName(void) const
{
	return (_serverName);
}
