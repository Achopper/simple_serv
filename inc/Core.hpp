
#ifndef CORE_HPP
#define CORE_HPP

#define MAXCLIENTS 1024
#define REDCOL "\33[31"

#include <netdb.h>
#include <list>
#include <iostream>
#include "Client.hpp"
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <unistd.h>



typedef struct sockaddr_in sockaddr_in;
typedef struct pollfd pollfd;
typedef struct sockaddr_storage sockaddr_storage;

class Core
{
private:

	int 						_sockfd[10];		//TODO make dynamic(server count)
	pollfd 						_fdset[MAXCLIENTS];
	//std::list<Client> 			_clientList;

public:

	Core();
	//Core(Config &config);
	~Core();

	void startServ();

private:
	bool 						initSocets( void );
	void 						mainLoop( void );


	class CoreException : public std::exception
	{
		virtual const char* what() const throw()
		{
			return (REDCOL"Core Error");
		}
	};
};


#endif

//TODO close socket
//TODO free addrInfo