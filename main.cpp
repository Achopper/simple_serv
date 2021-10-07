#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define LOCALHOST "127.0.0.1"

int main()
{
	struct sockaddr_in adrr;
	int adrLen = sizeof(adrr);
	int serv_fd;
	int newSock;
	int valread;
	char ip4[INET_ADDRSTRLEN];

	memset(adrr.sin_zero, 0, sizeof(adrr.sin_zero));
	adrr.sin_family = AF_INET;
	inet_pton(AF_INET, LOCALHOST, &adrr.sin_addr);
	//adrr.sin_addr.s_addr = htons(INADDR_ANY);
	adrr.sin_port = htons(PORT);
	std::ifstream page("simple.html");
	std::string p;
	std::string res;
	while (std::getline(page, p))
		res += p;
	//std::cout << res <<  std::endl;
	inet_ntop(AF_INET, &(adrr.sin_addr), ip4, INET_ADDRSTRLEN);
	std::cout << ip4 << std::endl;
	if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("cant create socket!");
		return (1);
	}
	if (bind(serv_fd, (struct sockaddr*)&adrr, sizeof(adrr)) < 0)
	{
		perror("cant bind!");
		return (1);
	}
	if (listen(serv_fd, 3) < 0)
	{
		perror("in: listen");
		exit(EXIT_FAILURE);
	}
	while (true)
	{
		if ((newSock = accept(serv_fd, (struct sockaddr *) &adrr, (socklen_t *) &adrLen)) < 0)
		{
			perror("in: acept");
			exit(EXIT_FAILURE);
		}
		char buf[BUFSIZ] = {0};
		valread = recv(newSock, buf, BUFSIZ, 0);
		std::cout << buf << std::endl;
		if (valread < 0)
			std::cout << "no bytes read" << std::endl;

		std::string ms = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(ms.length() +
				res.length()) + "\n\n" + res;
		//char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 16\n\nHello from serv!";
		send(newSock, ms.c_str(), ms.length(), 0);
		close(newSock);
	}
	return 0;
}
