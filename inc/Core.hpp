
#ifndef CORE_HPP
#define CORE_HPP

#define MAXCLIENTS 1024
#define LOCALHOST "127.0.0.1" //TODO del after parse config

#include "serv_includes.hpp"
#include "Config.hpp"


typedef struct sockaddr_in sockaddr_in;
typedef struct pollfd pollfd;
typedef struct sockaddr_storage sockaddr_storage;

class Core
{
private:

	//uint32_t 					_servSize;
	int 						_sockfd[10];		//TODO make dynamic(server count)
	pollfd 						_fdset[MAXCLIENTS];
	//std::list<Client> 		_clientList;
	//std::vector<Server>		_servers

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
			return ("Core Error");
		}
	};
};


#endif

//TODO close socket
//TODO free addrInfo