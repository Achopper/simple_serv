#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080


int main()
{
	struct sockaddr_in adrr;
	int adrLen = sizeof(adrr);
	int serv_fd;
	int newSock;
	int valread;

	memset(adrr.sin_zero, 0, sizeof(adrr.sin_zero));
	adrr.sin_family = AF_INET;
	adrr.sin_addr.s_addr = INADDR_ANY;
	adrr.sin_port = htons(PORT);

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
		char buf[1024] = {0};
		valread = read(newSock, buf, 1024);
		std::cout << buf << std::endl;
		if (valread < 0)
			std::cout << "no bytes read" << std::endl;
		char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 16\n\nHello from serv!";;
		send(newSock, hello, strlen(hello), 0);
		close(newSock);
	}
	return 0;
}
