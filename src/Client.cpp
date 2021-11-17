
#include "../inc/Client.hpp"

Client::Client (Server const & server, pollfd* set)
: 	_server(server),
	_setFd(set)
{
	_isUrlSet = 0;
	_isMetodSet = 0;
	_isHttpVerSet = 0;
	_isFirstLineSet = 0;
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

	void	Client::setUrl(){}
	void	Client::setBuf(){}
	void	Client::setReqBody(){}
	void	Client::setMethod(){}
	void	Client::setHttpVersion(){}
	void	Client::setQueryString(){}

	std::string	Client::getUrl(){
		return _url;
	}
	std::string	Client::getBuf(){
		return _buf;
	}
	std::string	Client::getReqBody(){
		return _reqBody;
	}
	std::string	Client::getMethod(){
		return _method;
	}
	std::string	Client::getHttpVersion(){
		return _httpVersion;
	}
	std::string	Client::getQueryString(){
		return _queryString;
	}

	void	Client::parseReq(){

		_buf += _req;
		std::cout << "__________BUF__________"<< std::endl ;
		for ( std::string::iterator it=_buf.begin(); it!=_buf.end(); ++it)
		{
			switch (*it)
			{
				case 'G':
				case 'P':
				case 'D':
					if (!_isMetodSet)
					{
						std::string	metod;
						for ( ; *it!=' '; ++it)
							metod += *it;
 							// std::cout << *it; // << std::endl;//почему выводит 2 раза?
						if (metod == "GET" || metod == "POST" || metod == "DELITE")
						{
							_isMetodSet = 1;
							_method = metod;
							std::cout << "metod " << _method << std::endl ;
							++it;
						}
						break;
					}
				case 'H':
					if (!_isMetodSet)
					{
						std::string	httpVersion;
						for ( ; *it!='\r'; ++it)
							httpVersion += *it;
 							// std::cout << *it; // << std::endl;//почему выводит 2 раза?
						if (httpVersion == "HTTP/1.1")
						{
							_isHttpVerSet = 1;
							_httpVersion = httpVersion;
							std::cout << "httpVersion " << _httpVersion << std::endl ;
							++it;
						}
						break;
					}
				default:
					break;
			}
		}

		std::cout << "__________BUF__________"<< std::endl ;
	}


