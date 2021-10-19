
#include <arpa/inet.h> //TODO del after get IP from config
#include "../inc/Core.hpp"
#include <fstream>

Core::Core()
{
	memset (&_fdset, 0, sizeof(_sockfd));
}

Core::~Core()
{
}


void Core::startServ()
{
	initSocets();


		mainLoop();
}

struct addrinfo* getInfo(const char* &port)
{
	struct addrinfo *res;
	struct addrinfo hints;

	memset(&hints, 0, sizeof hints); // очистка структуры
	hints.ai_family = PF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // потоковый сокет TCP
	if (getaddrinfo(nullptr, port, &hints, &res) != 0)
	{
		std::cout << "Cant get addrinfo" << std::endl;
		exit(EXIT_FAILURE);
	}
	return (res);
}

bool Core::initSocets()
{

	int 						yes = 1, serv_size = 1; 	 //num servers in config
	const char* port = 			"2021";						//replace by config value
	struct addrinfo 			*addrInfo = getInfo(port);

	struct sockaddr_in *addr; //TODO del after get IP from config
	char ip4[INET_ADDRSTRLEN];

	addr = (struct sockaddr_in*)addrInfo->ai_addr;
	inet_ntop(addrInfo->ai_family, &(addr->sin_addr), ip4, INET_ADDRSTRLEN); //TODO del after get IP from config
	std::cout << "http://"  << ip4 << ":" << port << std::endl;

	for (int i = 0; i < serv_size; ++i)
	{
		if ((_sockfd[i] = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol)) < 0)
		{
			std::cout << "cant create socket" << std::endl;
			throw CoreException();
		}
		setsockopt(_sockfd[i], SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&yes), sizeof yes);
		fcntl(_sockfd[i], F_SETFL | O_NONBLOCK);
		if (bind(_sockfd[i], addrInfo->ai_addr, addrInfo->ai_addrlen) < 0)
		{
			std::cout << "Can't bind socket" << std::endl;
			throw CoreException();
		}
		if (listen(_sockfd[i], SOMAXCONN))
		{
			std::cout << "Can't listen socket" << std::endl;
			throw CoreException();
		}
	}
	freeaddrinfo(addrInfo);
	return (true);
}

void Core::mainLoop(void)
{
	nfds_t servNum = 1; //TODO num servers in config
	nfds_t numfds = servNum;
	int pollRet = 0;
	int newSock;
	struct sockaddr_storage their_addr;
	ssize_t valread;
	int addrLen = sizeof their_addr;
	std::string line, res;
	std::ifstream page("simple.html", std::ios::binary);
	if (!page.is_open())
		std::cout << "Cant open" << std::endl;
	else
	{
		while (std::getline(page, line))
			res += line + "\n";
	}
	for (nfds_t i = 0; i < servNum; ++i)
	{
		_fdset[i].fd = _sockfd[i];
		_fdset[i].events = POLLIN;
	}
	while (true)
	{
		if ((pollRet = poll(_fdset, numfds, 5000)) < 0)
		{
			std::cout << "Can't poll" << std::endl;
			throw CoreException();
		}
		if (pollRet == 0)
		{
			std::cout << "TIMEOUT at fd " << _sockfd[0] << std::endl;
			close(_fdset[0].fd);
			return;
		}
		for (nfds_t i = 0; i < numfds; ++i)
		{
			if (_fdset[i].revents & POLLIN)
			{
				_fdset[i].revents = 0;
				std::cout << "POLLIN in fd" << _fdset[i].fd << std::endl;
				if (i == 0)
				{
					if ((newSock = accept(_fdset[i].fd, (struct sockaddr *) &their_addr, (socklen_t *) &addrLen)) <
						0)
					{
						perror("in: acept");
						exit(EXIT_FAILURE);
					}
					fcntl(newSock, F_SETFL | O_NONBLOCK);
					if (numfds < MAXCLIENTS)
					{
						_fdset[numfds].fd = newSock;
						_fdset[numfds].events = POLLIN;
						numfds++;
					}
					else
					{
						std::cout << "can't serve more clients!" << std::endl;
						close(newSock);
					}
				}
				else
				{
					std::string b;
					char buf[BUFSIZ] = {0};
					valread = recv(_fdset[i].fd, buf, BUFSIZ, 0);
					b = buf;
					std::cout << b << std::endl;
					if (valread < 0)
						std::cout << "no request" << std::endl;
					std::string ms;
					ms += "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(ms.length() +
																										res.length())
						  +
						  "\n\nHello" + res;
					std::cout << ms << std::endl;
					send(_fdset[i].fd, ms.c_str(), ms.length(), 0);
					close(_fdset[i].fd);
					_fdset[i].fd = -1;
					_fdset[i].events = 0;
				}
			}
		}
	}
}

