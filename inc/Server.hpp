
#ifndef SERVER_HPP
#define SERVER_HPP

#include "serv_includes.hpp"

typedef struct sockaddr_in sockaddr_in;

class Server
{
private:

	sockaddr_in addr;
	std::string servIp;
	std::string serverName;
	std::string port;

public:

	Server();

	~Server();


};


#endif //SERVER_HPP
