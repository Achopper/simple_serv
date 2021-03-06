

#include "../inc/Core.hpp"

Client::Client (Server const & server, pollfd* set)
: 	_server(server),
	_setFd(set),
	_req(""),
	_connectTime (std::time(nullptr)),
	_finishReadReq(false)
{
	std::cout << GREENCOL"Client " << set->fd << " connected" << RESCOL << std::endl;
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
		_req = obj._req;
		_connectTime = obj._connectTime;
		_finishReadReq = obj._finishReadReq;
		_response = obj._response;
		_request = obj._request;
	}
	return (*this);
}

void Client::setReq(std::string const & req)
{
	_req = req;
}

void Client::setSetFd( pollfd  *setFd )
{
	_setFd = setFd;
}

void Client::setConnTime()
{
	_connectTime = std::time(nullptr);
}

void Client::setResponse(Response &response)
{
	_response = response;
}

std::string Client::getReq(void) const
{
	return (_req);
}


pollfd* Client::getSetFd(void) const
{
	return (_setFd);
}

time_t Client::getConTime() const
{
	return (_connectTime);
}

void Client::setFinishReadReq(bool isFinish)
{
	_finishReadReq = isFinish;
}

bool Client::getFinishReadReq() const
{
	return (_finishReadReq);
}

const Server &Client::getServer() const
{
	return (_server);
}

Response * Client::getResponse()
{
	return (&_response);
}

Request&	Client::getRequest()
{
	return (_request) ;
}


void Client::deleteClient()
{
	close(_setFd->fd);
	_setFd->fd = -1;
	_setFd->events = 0;
	_setFd->revents = 0;
}

void Client::makeResponse()
{
	if (_response.getCode() == "408" || _response.getCode() == "405")
	{
		_response.fillResponse();
		std::cout << _response.getResp() << std::endl;
		return;
	}
	if (_request.getHttpVersion() != PROT && _response.getCode() != "408")
	{
		_response.setCode("505");
		_response.fillResponse();
		std::cout << _response.getResp() << std::endl;;
		return;
	}
	if (_request.getErrCode().empty())
	{
		if (_request.getMethod() == "GET")
			_response.GET(_setFd->fd);
		else if (_request.getMethod() == "DELETE")
			_response.DELETE();
		else if (_request.getMethod() == "POST")
			_response.POST(_setFd->fd);
		else if (_request.getMethod() == "HEAD")
		{
			_response.HEAD(_setFd->fd);
			_response.setBody("");
		}
		else
			_response.setCode("405");
	}
		_response.fillResponse();
}


void	Client::setRequest(std::string const &req)
{
	try
	{
		_request.parseReq(req);
	}
	catch(const std::exception& e)
	{
		std::cout << "???????????????????? ??????????????!!!!!!!!!!!!" << std::endl;
		std::cout << "??????!!!!!!!!!!!!  "  << _request.getErrCode() << std::endl;
		_request.setReqEnd(true);
		_response.setCode(_request.getErrCode());
		 std::cerr << e.what() << '\n';
	}
	
}

