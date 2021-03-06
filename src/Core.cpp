
#include "../inc/Core.hpp"
#include <cstring>

Core::Core(Config & config)
{
	memset (&_fdset, -1, sizeof(_fdset));
	_servSize = config.getServCount();
	_servers = config.getServers();
}

Core::~Core()
{
}


void Core::startServ()
{
	initSocets();
	mainLoop();
}

bool Core::acceptClientConnect(std::vector<Server>::iterator& iterator, nfds_t& num)
{
	int newSock;
	struct sockaddr_storage their_addr;
	int addrLen = sizeof their_addr;
	if ((newSock = accept(iterator->getServerFd()->fd, (struct sockaddr *) &their_addr, (socklen_t *) &addrLen)) < 0)
	{
		std::cout << REDCOL"Error in accept: " << strerror(errno) << RESCOL << std::endl;
		throw CoreException();
	}
	iterator->getServerFd()->revents &= ~POLLRDNORM;
	fcntl(newSock, F_SETFL , O_NONBLOCK);
	for (int j = static_cast<int>(num); j < FOPEN_MAX + 1; ++j) {
		if (j == FOPEN_MAX) {
			std::cerr << "Can't serve more clients. Try again later.." << std::endl;
			break;
		}
		if (_fdset[j].fd < 0) {
			_fdset[j].fd = newSock;
			_fdset[j].events = (POLLRDNORM | POLLOUT);
			Client client(*iterator, &_fdset[j]);
			client.getResponse()->setServer(*iterator);
			_clientList.push_back(client);
			num++;
			break;
		}
	}
	return (true);
}

void Core::readRequest(std::list<Client>::iterator &it, nfds_t& num)
{
	ssize_t valread;
	char buf[BUFSIZ] = {0};

	valread = recv(it->getSetFd()->fd, buf, DEF_CLI_MAX_BDY_SZ, 0);
	it->setConnTime();
	if (valread < 0)
	{
		std::cout << REDCOL << strerror(errno) << " no request" << RESCOL << std::endl;
		it->deleteClient();
		it = _clientList.erase(it);
		num--;
	}
	else if (valread == 0) {
		std::cout << "Connection close by client " << it->getSetFd()->fd << std::endl;
		it->deleteClient();
		it = _clientList.erase(it);
		num--;
	}
	else if (valread > 0)
	{
		it->setReq(buf);
#if DEBUG_MODE > 0
		std::cout <<GREENCOL "client " << it->getSetFd()->fd << " reсive " << valread << " bytes and revent: " <<
		it->getSetFd()->revents << RESCOL << std::endl;
#endif
	}
	else
		return ;
}

bool Core::sendResponce(std::list<Client>::iterator &it, nfds_t& num)
{
	ssize_t s;
	try
	{
		it->makeResponse();
		s = send(it->getSetFd()->fd, it->getResponse()->getResp().c_str(), it->getResponse()->getResp().length(), 0);
	}
	catch (std::string &cgi)
	{
		std::cout << GREENCOL << "That made " << cgi << " code!" << RESCOL << std::endl;
	}
	if (s < 0)
	{
		std::cout << REDCOL"Error in send: " << strerror(errno) << RESCOL << std::endl;
		throw CoreException();
	}
#if DEBUG_MODE > 0
	std::cout << GREENCOL << "client " << it->getSetFd()->fd << " send  " << s << RESCOL << std::endl
	<< "Responce: \n" << it->getResponse()->getResp() << std::endl;
#endif
	it->deleteClient();
	it = _clientList.erase(it);
	num--;
	if (it == _clientList.end())
		it = _clientList.begin();
	return (true);
}


bool Core::initSocets()
{
	int 		yes = 1;

	for (uint32_t i = 0; i < _servSize; ++i)
	{
		int sock;
		sockaddr_in addr = _servers.at(i).getAddr();
		if ((sock = socket(addr.sin_family, SOCK_STREAM, 0)) < 0)
		{
			std::cout << REDCOL"cant create socket:" << strerror(errno) << RESCOL << std::endl;
			throw CoreException();
		}
		_sockfd.push_back(sock);
		setsockopt(_sockfd.at(i), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
		fcntl(_sockfd.at(i), F_SETFL , O_NONBLOCK);
		if (bind(_sockfd.at(i), (struct sockaddr *) &addr, sizeof(addr)) < 0)
		{
			std::cout << REDCOL"Can't bind socket: " << strerror(errno) << RESCOL << std::endl;
			throw CoreException();
		}
		if (listen(_sockfd.at(i), SOMAXCONN))
		{
			std::cout << REDCOL"Can't listen socket: " << strerror(errno) << RESCOL << std::endl;
			throw CoreException();
		}
		_servers.at(i).setServerFd(&_fdset[i]);
		_fdset[i].fd = _sockfd.at(i);
		_fdset[i].events = POLLRDNORM;
	}
	return (true);
}

void Core::mainLoop() {
    nfds_t numfds = _servSize;
	int pollRet;
	std::string fullReq;

	while (true) {
		if ((pollRet = poll(_fdset, numfds + 1, TIMEOUT)) < 0)
		{
			std::cout << REDCOL"Can't poll" << strerror(errno) << RESCOL << std::endl;
			throw CoreException();
		}
		for(std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); ++it)
		{
			if (it->getServerFd()->revents & POLLRDNORM)
				acceptClientConnect(it, numfds);
		}
		for (std::list<Client>::iterator cli_it = _clientList.begin(); cli_it != _clientList.end(); ++cli_it)
		{
			if (cli_it->getSetFd()->revents & (POLLRDNORM | POLLERR))
			{
				if (!cli_it->getFinishReadReq() )
				{
					readRequest(cli_it, numfds);
					fullReq.append(cli_it->getReq());
					cli_it->setRequest(cli_it->getReq());
				}
				if (!cli_it->getRequest().getIsRequestEnd())
					continue;
				else
				{
					cli_it->getSetFd()->revents &= ~(POLLRDNORM | POLLERR);
					cli_it->setFinishReadReq(true);
					cli_it->getResponse()->setRequest(cli_it->getRequest());
				}
			}
			if (cli_it->getFinishReadReq())
			{
#if DEBUG_MODE > 0
				std::cout << REDCOL << fullReq << RESCOL << std::endl;
				fullReq.erase();
#endif
				sendResponce(cli_it, numfds);
			}
			if (!_clientList.empty() && difftime(time(nullptr), cli_it->getConTime()) > CLI_TIMEOUT_SEC)
			{
#if DEBUG_MODE > 0
				std::cout << REDCOL"Client " << cli_it->getSetFd()->fd  << " disconnected by timeout" << RESCOL <<
						  std::endl;
#endif
				cli_it->getResponse()->setCode("408");
				sendResponce(cli_it, numfds);
			}
		}
	}
}


