
#include "../inc/Core.hpp"


Core::Core(Config & config)
{
	memset (&_fdset, 0, sizeof(_fdset));
	for (int i = 0; i < OPEN_MAX; ++i)
		_fdset[i].fd = -1;
	memset (&_sockfd, 0, sizeof(_sockfd));
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


bool Core::initSocets()
{
	int 		yes = 1;

	for (uint32_t i = 0; i < _servSize; ++i)
	{
		sockaddr_in addr = _servers.at(i).getAddr();
		if ((_sockfd[i] = socket(addr.sin_family, SOCK_STREAM, 0)) < 0)
		{
			std::cout << REDCOL"cant create socket" << RESCOL << std::endl;
			throw CoreException();
		}
		setsockopt(_sockfd[i], SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
		fcntl(_sockfd[i], F_SETFL | O_NONBLOCK);

		if (bind(_sockfd[i], (struct sockaddr *) &addr, sizeof(addr)) < 0)
		{
			std::cout << REDCOL"Can't bind socket" << RESCOL << std::endl;
			throw CoreException();
		}
		if (listen(_sockfd[i], SOMAXCONN))
		{
			std::cout << REDCOL"Can't listen socket" << RESCOL << std::endl;
			throw CoreException();
		}
		_servers.at(i).setFd(_sockfd[i]);
		_fdset[i].fd = _sockfd[i];
		_fdset[i].events = POLLRDNORM;
	}
	return (true);
}

void Core::mainLoop(void)
{
	nfds_t numfds = _servSize;
	int pollRet;
	int newSock;
	struct sockaddr_storage their_addr;
	ssize_t valread;
	int addrLen = sizeof their_addr;
	std::string b;
	char buf[BUFSIZ] = {0};
	std::string line, res;
	std::ifstream page("simple.html", std::ios::binary);

	if (!page.is_open())
		std::cout << REDCOL"Cant open" << RESCOL << std::endl;
	else
		while (std::getline(page, line))
			res += line + "\n";
	page.close();
	std::string ms;
	ms += "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(ms.length() +
																						res.length()) + "\n\n" + res;

	while (true)
	{
		if ((pollRet = poll(_fdset, numfds, TIMEOUT)) < 0)
		{
			std::cout << REDCOL"Can't poll" << RESCOL << std::endl;
			throw CoreException();
		}
		if (pollRet == 0) // && connection !keep-alive
		{
			std::cout << REDCOL"TIMEOUT at fd " << _sockfd[0]  << RESCOL << std::endl;
			close(_fdset[0].fd);
			return;
		}
		for(nfds_t i = 0; i < numfds; ++i)
		{
			int j = 0;
			if (_fdset[i].revents & POLLRDNORM)
			{
				newSock = accept(_fdset[i].fd, (struct sockaddr *) &their_addr, (socklen_t *) &addrLen);
	//			_fdset[i].revents = 0;
				fcntl(newSock, F_SETFL | O_NONBLOCK);
				for (j = static_cast<int>(numfds); j < OPEN_MAX; ++j)
				{
					if (_fdset[j].fd < 0)
					{
						_fdset[j].fd = newSock;
						_fdset[j].events = POLLRDNORM;
						numfds++;
						break;
					}
					if (j == OPEN_MAX)
					{
						std::cerr << "Can't serve more clients. Exit.." << std::endl;
						exit(EXIT_FAILURE);
					}
				}
//				if (j > static_cast<int>(numfds))
//					numfds = static_cast<nfds_t>(j);
				if (--pollRet <= 0)
					continue;
			}
			for (j = (int)_servSize; j < static_cast<int>(numfds); ++j)
			{
				if ((newSock = _fdset[j].fd) < 0)
					continue;
				if (_fdset[j].revents & (POLLRDNORM | POLLERR))
				{
					valread = recv(newSock, buf, BUFSIZ, 0);
					std::cout << valread << std::endl;
					if (valread < 0)
					{
						std::cout << REDCOL"no request" << RESCOL << std::endl;
						close(newSock);
						_fdset[j].fd = -1;
						//numfds--;
					}
					else if (valread == 0)
					{
						std::cout << "Connection close by client" << std::endl;
						close(newSock);
						_fdset[j].fd = -1;
						//numfds--;
					}
					else
					{
						send(newSock, ms.c_str(), ms.length(), 0);
//						close(newSock);
//						_fdset[j].fd = -1;
//						numfds--;
					}
					if (--pollRet <= 0)
						break;
				}
			}
		}
	}
//		if ((pollRet = poll(_fdset, numfds, TIMEOUT)) < 0)
//		{
//			std::cout << REDCOL"Can't poll" << RESCOL << std::endl;
//			throw CoreException();
//		}
//		if (pollRet == 0) // && connection !keep-alive
//		{
//			std::cout << REDCOL"TIMEOUT at fd " << _sockfd[0]  << RESCOL << std::endl;
//			close(_fdset[0].fd);
//			return;
//		}
//		for (nfds_t i = 0; i < numfds; ++i)
//		{
//			if (_fdset[i].revents & POLLIN)
//			{
//				std::cout << "POLLIN in fd" << _fdset[i].fd << std::endl;
//				if (_fdset[i].fd == _servers.at(i).getFd() && _fdset[i].revents != 0)
//				{
//					_fdset[i].revents = 0;
//					if ((newSock = accept(_fdset[i].fd, (struct sockaddr *) &their_addr, (socklen_t *) &addrLen)) < 0)
//					{
//						perror("in: acept");
//						exit(EXIT_FAILURE);
//					}
//					fcntl(newSock, F_SETFL | O_NONBLOCK);
//					if (numfds < OPEN_MAX)
//					{
//						_fdset[numfds].fd = newSock;
//						_fdset[numfds].events = POLLIN;
//						numfds++;
//					}
//					else
//					{
//						std::cout << REDCOL"can't serve more clients!" << RESCOL << std::endl;
//						close(newSock);
//					}
//				}
//				else
//				{
//					std::string b;
//					char buf[BUFSIZ] = {0};
//					valread = recv(_fdset[i].fd, buf, BUFSIZ, 0);
//					b = buf;
//					std::cout << b << std::endl;
//					if (valread < 0)
//						std::cout << REDCOL"no request" << RESCOL << std::endl;
//					std::cout << ms << std::endl;
//					send(_fdset[i].fd, ms.c_str(), ms.length(), 0);
//					close(_fdset[i].fd);
//					_fdset[i].fd = -1;
//					_fdset[i].events = 0;
//					numfds--;
//				}
//			}
//		}
	}
