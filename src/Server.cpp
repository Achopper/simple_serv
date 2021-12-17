
#include "../inc/Server.hpp"
#include <climits>
#include <cstring>

Server::Server() :
_servIp(""),
_serverName(""),
_port("")
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
		_servIp			= obj._servIp;
		_serverFd		= obj._serverFd;
		_serverName		= obj._serverName;
		_port			= obj._port;
		_addr			= obj._addr;
		_root			= obj._root;
		_locList		= obj._locList;
		_errorPages		= obj._errorPages;
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

bool Server::setServName(const std::string &servName)
{
	_serverName = servName;
	return (true);
}

bool Server::setAddr(const std::string &addr, unsigned long &pos)
{
	if (!setPort(addr.substr(pos + 1)))
		return (false);
	if (!setServIp(addr.substr(0, pos)))
		return (false);
	_addr.sin_family = PF_INET;
	memset(_addr.sin_zero, '\0', sizeof _addr.sin_zero);
	return (true);
}


void Server::setServerFd(pollfd *fdSet)
{
	_serverFd = fdSet;
}

bool Server::setRoot(const std::string &root)
{
	if (root[0] != '.')
		return (false);
	_root = root;
	return (true);
}

void Server::setLocList(const std::vector<Location> &locList)
{
	_locList = locList;
}

void Server::setLocList(const Location &location)
{
	_locList.push_back(location);
}

bool Server::setErrorPage(const std::string &address, const std::string &code, std::string & error)
{

	for (std::string::const_iterator it = code.begin(); it != code.end(); ++it)
		if (!std::isdigit(*it) || code.length() != 3)
		{
			error.append(REDCOL"Wrong error code address \n" RESCOL);
			return (false);
		}
	_errorPages.insert(std::pair<std::string, std::string>(code, address));
	return (true);
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

pollfd* Server::getServerFd() const
{
	return (_serverFd);
}

std::string Server::getRoot() const
{
	return (_root);
}

const std::vector<Location> & Server::getLocList() const
{
	return (_locList);
}

const std::map<std::string, std::string>& Server::getErrPage() const
{
	return (_errorPages);
}

bool Server::downloadFile(std::string const &path, std::string const & dst)
{
	std::string _pt(path);
	std::string toTrim("%2F");
	for (std::size_t found = _pt.find(toTrim); found != std::string::npos; found = _pt.find(toTrim) )
		_pt.replace(found, 3, "/");
	std::ifstream src;
	std::ofstream dest;
	src.open(_pt,std::ios_base::binary);
	dest.open(dst + "/" + std::string(_pt.substr(_pt.rfind('/'))), std::ios_base::binary);
	if (!src.is_open())
		return (false);
	if (!dest.is_open())
	{
		src.close();
		return (false);
	}
	else
		dest << src.rdbuf();

	return true;
}



