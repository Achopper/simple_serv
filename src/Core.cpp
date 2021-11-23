
#include "../inc/Core.hpp"



Core::Core(Config & config)
{
	memset (&_fdset, 0, sizeof(_fdset));
	for (int i = 0; i < OPEN_MAX; ++i)
		_fdset[i].fd = -1;
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
	std::cout << GREENCOL"Client " << newSock << " connected" << RESCOL << std::endl;
	iterator->getServerFd()->revents &= ~POLLRDNORM;
	fcntl(newSock, F_SETFL , O_NONBLOCK);
	for (int j = static_cast<int>(num); j < OPEN_MAX + 1; ++j) {
		if (j == OPEN_MAX) {
			std::cerr << "Can't serve more clients. Exit.." << std::endl;
			break; //TODO exit?
		}
		if (_fdset[j].fd < 0) {
			_fdset[j].fd = newSock;
			_fdset[j].events = POLLRDNORM;
			_clientList.push_back(Client(*iterator, &_fdset[j]));
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

	it->getSetFd()->revents &= ~(POLLRDNORM | POLLERR);
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
		std::cout << "Connection close by client" << std::endl;
		it->deleteClient();
		it = _clientList.erase(it);
		num--;
	}
	else if (valread > 0)
		it->setReq(it->getReq().append(buf, static_cast<size_t>(valread)));
	else
		return ;
}

bool Core::sendResponce(std::list<Client>::iterator &it, nfds_t& num)
{
	send(it->getSetFd()->fd, it->getResponse()->getResp().c_str(), it->getResponse()->getResp().length(), 0);
	it->deleteClient();
	it = _clientList.erase(it);
	num--;
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

void testParse(std::string const & req, std::list<Client>::iterator it) //TODO test
{
	std::vector<std::string> splitRequest;
	for (std::string::size_type start = 0, end = 0; start != req.length();)
	{
		end = req.find("\r\n", start);
		if (end == std::string::npos)
		{
			splitRequest.push_back(req.substr(start, req.length()));
			break;
		}
		std::string tok = req.substr(start,end - start);
		splitRequest.push_back(tok);
		start = end + 2;
	}
	for (std::string::size_type start = 0, end = 0; start != splitRequest.at(0).length();)
	{
		end = splitRequest.at(0).find(' ', start);
		if (end == std::string::npos)
		{
			it->prot = (splitRequest.at(0).substr(start, splitRequest.at(0).length()));
			break;
		}
		std::string tok = splitRequest.at(0).substr(start,end - start);
		tok[0] == '/' ? it->path = tok : it->method = tok;
		start = end + 1;
	}
}


void Core::mainLoop() {
    nfds_t numfds = _servSize;
    int pollRet;


	while (true) {
		if ((pollRet = poll(_fdset, numfds, TIMEOUT)) < 0)
		{
			std::cout << REDCOL"Can't poll" << RESCOL << std::endl;
			throw CoreException();
		}
		if (pollRet == 0)
		{
			std::cout << REDCOL"TIMEOUT at fd " << _sockfd[0] << RESCOL << std::endl;
			_sockfd.erase(_sockfd.begin());
			close(_fdset[0].fd);
			_fdset[0].fd = -1;
			if (_sockfd.empty())
				return;
			continue;
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
				if (!cli_it->getFinishReadReq())
					readRequest(cli_it, numfds);
				std::string::size_type pos = cli_it->getReq().find("\r\n\r\n");
				if (pos == std::string::npos)
					continue;
				else
					cli_it->setFinishReadReq(true);
			}
			if (cli_it->getFinishReadReq())
			{
				testParse(cli_it->getReq(), cli_it);//TODO Test part
				std::cout << cli_it->getReq() << std::endl;
				Response response(cli_it->method);
				cli_it->setResponse(response);
				cli_it->makeResponse(response);
				sendResponce(cli_it, numfds);
			}
			if (std::difftime(std::time(nullptr), cli_it->getConTime()) > CLI_TIMEOUT_SEC)
			{
				std::cout << REDCOL"Client " << cli_it->getSetFd()->fd  << " disconnected by timeout" << RESCOL <<
						  std::endl;
				cli_it->deleteClient();
				//TODO add time_out response
				cli_it = _clientList.erase(cli_it);
			}
		}
	}
}




