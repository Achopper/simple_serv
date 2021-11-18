
#include "../inc/Client.hpp"

Client::Client (Server const & server, pollfd* set)
: 	_server(server),
	_setFd(set),
	_body(""),
	_req(""),
	_connectTime (std::time(nullptr)),
	_finishReadReq(false)
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
		_setFd = obj._setFd;
		_body = obj._body;
		_req = obj._req;
		_connectTime = obj._connectTime;
		_finishReadReq = obj._finishReadReq;
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

void Client::deleteClient()
{
	close(_setFd->fd);
	_setFd->fd = -1;
}

void Client::setFinishReadReq(bool isFinish)
{
	_finishReadReq = isFinish;
}

bool Client::getFinishReadReq(void) const
{
	return (_finishReadReq);
}




