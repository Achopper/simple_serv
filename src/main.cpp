#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <array>
#include <deque>

#include "../inc/Core.hpp"

#define PORT "2021"
#define MAXCLIENT 1024
//#define LOCALHOST "127.0.0.1"
#define PROTOCOL "http"

//struct addrinfo* getInfo()
//{
//	struct addrinfo *res;
//	struct addrinfo hints;
//
//	memset(&hints, 0, sizeof hints); // очистка структуры
//	hints.ai_family = PF_INET; // IPv4
//	hints.ai_socktype = SOCK_STREAM; // потоковый сокет TCP
//	if (getaddrinfo(nullptr, PORT, &hints, &res) != 0)
//	{
//		std::cout << "Cant get addrinfo" << std::endl;
//		exit(EXIT_FAILURE);
//	}
//	return (res);
//}

int main()
{
	Core core;

	try
	{
		core.startServ();
	}catch (std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
//	struct sockaddr_in *addr;
//	struct addrinfo* addrinfo = getInfo();
//	struct pollfd pfds[100];
//	struct sockaddr_storage their_addr;
//	int adrLen = sizeof their_addr, serv_fd, yes = 1, pollrec, newSock = -1;
//	nfds_t numfds = 1;
//	long valread;
//	char ip4[INET_ADDRSTRLEN];
//
//	std::ifstream page("simple.html");
//	std::string line;
//	std::string res;
//	while (std::getline(page, line))
//		res += line + "\n";
//	addr = (struct sockaddr_in*)addrinfo->ai_addr;
//	inet_ntop(addrinfo->ai_family, &(addr->sin_addr), ip4, INET_ADDRSTRLEN); //get local IP
//	std::cout << "http://"  << ip4 << ":" << PORT << std::endl;
//	if ((serv_fd = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol)) < 0)
//	{
//		perror("cant create socket!");
//		return (1);
//	}
//	setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&yes), sizeof yes); //make
//	// socket reusable after disconnect
//	fcntl(serv_fd, F_SETFL | O_NONBLOCK);
//	if (bind(serv_fd, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0)
//	{
//		perror("cant bind!");
//		return (1);
//	}
//	if (listen(serv_fd, SOMAXCONN) < 0)
//	{
//		perror("in: listen");
//		exit(EXIT_FAILURE);
//	}
//	pfds[0].fd = serv_fd;
//	pfds[0].events = POLLIN;
//	freeaddrinfo(addrinfo);
//	while (true)
//	{
//		if ((pollrec = poll(pfds, numfds, 3000)) < 0)
//		{
//			perror("in: poll");
//			exit(EXIT_FAILURE);
//		}
//		if	(pollrec == 0)
//		{
//			std::cout << "TIMEOUT" << std::endl;
//			close(serv_fd);
//			break;
//		}
//		for (nfds_t i = 0; i < numfds; ++i)
//		{
//			if (pfds[i].revents & POLLIN)
//			{
//				std::cout << "POLLIN in fd" << pfds[i].fd << std::endl;
//				pfds[i].revents = 0;
//				if (i == 0)
//				{
//					if ((newSock = accept(pfds[i].fd, (struct sockaddr *) &their_addr, (socklen_t *) &adrLen)) < 0)
//					{
//						perror("in: acept");
//						exit(EXIT_FAILURE);
//					}
//					fcntl(newSock, F_SETFL | O_NONBLOCK);
//					if (numfds < MAXCLIENT)
//					{
//						pfds[numfds].fd = newSock;
//						pfds[numfds].events = POLLIN;
//						numfds++;
//					} else {
//						std::cout << "can't serve more clients!" << std::endl;
//						close(newSock);
//					}
//				}
//				else
//				{
//					std::string b;
//					char buf[BUFSIZ] = {0};
//					valread = recv(pfds[i].fd, buf, BUFSIZ, 0);
//					b = buf;
//					std::cout << b << std::endl;
//					if (valread < 0)
//						std::cout << "no request" << std::endl;
//					std::string ms;
//					ms += "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(ms.length() +
//																										res.length()) +
//						  "\n\n" + res;
//					//std::cout << ms << std::endl;
//					send(pfds[i].fd, ms.c_str(), ms.length(), 0);
//					close(pfds[i].fd);
//					pfds[i].fd = -1;
//					pfds[i].events = 0;
//				}
//			}
//		}
//	}
	return 0;
}
