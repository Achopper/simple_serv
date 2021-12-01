
#include "../inc/Client.hpp"
#include "../inc/Response.hpp"

Client::Client (Server const & server, pollfd* set)
: 	_server(server),
	_setFd(set),
	_body(""),
	_req(""),
	_connectTime (std::time(nullptr)),
	_finishReadReq(false)
{
	_request = Request();
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
		_body = obj._body;
		_req = obj._req;
		_connectTime = obj._connectTime;
		_finishReadReq = obj._finishReadReq;
		_response = obj._response;
		_request = obj._request;
//		path = obj.path; //TODO del
//		method = obj.method; //TODO del
//		prot = obj.prot; //TODO del
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

void Client::setConnTime()
{
	_connectTime = std::time(nullptr);
}

void Client::setResponse(Response &response)
{
	_response = &response;
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

time_t Client::getConTime() const
{
	return (_connectTime);
}

void Client::setFinishReadReq(bool isFinish)
{
	_finishReadReq = isFinish;
}

bool Client::getFinishReadReq(void) const
{
	return (_finishReadReq);
}

const Server &Client::getServer() const
{
	return (_server);
}

const Response *Client::getResponse(void) const
{
	return (_response);
}


void Client::deleteClient()
{
	close(_setFd->fd);
	_setFd->fd = -1;
	_setFd->events = 0;
	_setFd->revents = 0;
}



void Client::makeResponse(Response &response)
{
	if (_response->getCode() == "408")
	{
		response.fillResponse();
		std::cout << _response->getResp() << std::endl;
		return;
	}
	if (response.getMethod() == "GET")
		response.GET(*this);
	else
		_response->setCode("405");
	//esle if ("POST")
	response.fillResponse();
#if DEBUG_MODE > 0
	std::cout <<GREENCOL "Response of client " << _setFd->fd << ": " << RESCOL << std::endl << response.getResp()  <<
		std::endl;
#endif

}


Request&	Client::getRequest( void )
{
	return (_request) ;
}

void	Client::setRequest(std::string  req)
{
	_request.parseReq(req);
}
