
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
		_listenFd = obj._listenFd;
		_serverFd = obj._serverFd;
		_serverName = obj._serverName;
		_port = obj._port;
		_addr = obj._addr;
	}
	return *this;
}

bool Server::setServIp(std::string const & ip)
{
	for (std::string::const_iterator it = ip.begin(); it != ip.end() - 1; )
		if (!(std::isdigit(*++it) || *it == '.'))
			return (false);
	_servIp = ip;
	if ((_addr.sin_addr.s_addr = inet_addr(_servIp.c_str())) == INADDR_NONE)
		return (false);
	return (true);
}

bool Server::setPort(const std::string &port)
{
	int prt;

	if ((prt = std::atoi(port.c_str())) > USHRT_MAX || prt < 1)
		return (false);
	_port = port;
	_addr.sin_port = htons(static_cast<uint16_t>(prt));
	return (true);
}

void Server::setServName(const std::string &servName)
{
	_serverName = servName;
}

bool Server::setAddr(const std::string &addr, unsigned long &pos)
{
	if (!setPort(addr.substr(pos + 1)))
		return (false);
	if (!setServIp(addr.substr(0, pos)))
		return (false);
	_addr.sin_family = PF_INET;
	memset(_addr.sin_zero, '\0', sizeof _addr.sin_zero);
	std::cout << "http://"  << _servIp << ":" << _port << std::endl;
	return (true);
}

void Server::setFd(int const & fd)
{
	_listenFd = fd;
}

void Server::setServerFd(pollfd *fdSet)
{
	_serverFd = fdSet;
}

std::string Server::getServIp() const
{
	return (_servIp);
}

std::string Server::getPort() const
{
	return (_port);
}

std::string Server::getServName() const
{
	return (_serverName);
}

sockaddr_in Server::getAddr() const
{
	return (_addr);
}

int Server::getFd() const
{
	return _listenFd;
}

pollfd* Server::getServerFd() const
{
	return (_serverFd);
}

