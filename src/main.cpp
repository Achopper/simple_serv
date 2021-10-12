#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "8080"
//#define LOCALHOST "127.0.0.1"
#define PROTOCOL "http"

struct addrinfo* getInfo()
{
	struct addrinfo *res;
	struct addrinfo hints;

	memset(&hints, 0, sizeof hints); // очистка структуры
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // потоковый сокет TCP
	//hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(nullptr, PORT, &hints, &res) != 0)
	{
		std::cout << "Cant get addrinfo" << std::endl;
		exit(EXIT_FAILURE);
	}
	return (res);
}

int main()
{
	struct sockaddr_in *addr;
	struct addrinfo* addrinfo = getInfo();
	struct sockaddr_storage their_addr;
	int adrLen = sizeof their_addr;
	int serv_fd;
	int newSock;
	long valread;
	char ip4[INET_ADDRSTRLEN];


//	memset(adrr.sin_zero, 0, sizeof(adrr.sin_zero));
//	adrr.sin_family = AF_INET;
//	inet_pton(AF_INET, LOCALHOST, &adrr.sin_addr.s_addr);
	//adrr.sin_port = htons(PORT);
	std::ifstream page("simple.html");
	std::string line;
	std::string res;
	while (std::getline(page, line))
		res += line;
	addr = (struct sockaddr_in*)addrinfo->ai_addr;
	inet_ntop(addrinfo->ai_family, &(addr->sin_addr), ip4, INET_ADDRSTRLEN);
	std::cout << "http://"  << ip4 << ":" << PORT << std::endl;
	if ((serv_fd = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol)) < 0)
	{
		perror("cant create socket!");
		return (1);
	}
	if (bind(serv_fd, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0)
	{
		perror("cant bind!");
		return (1);
	}
	if (listen(serv_fd, 20) < 0)
	{
		perror("in: listen");
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(addrinfo);
	while (true)
	{
		if ((newSock = accept(serv_fd, (struct sockaddr *)&their_addr, (socklen_t *)&adrLen)) < 0)
		{
			perror("in: acept");
			exit(EXIT_FAILURE);
		}
		char buf[BUFSIZ] = {0};
		valread = recv(newSock, buf, BUFSIZ, 0);
		std::cout << buf << std::endl;
		if (valread < 0)
			std::cout << "no request" << std::endl;
		std::string ms;
		ms += "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(ms.length() +
				res.length()) + "\n\n" + res;
		std::cout << ms << std::endl;
		send(newSock, ms.c_str(), ms.length(), 0);
		close(newSock);
	}
	return 0;
}
