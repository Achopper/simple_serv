
#ifndef CORE_HPP
#define CORE_HPP


#include "serv_includes.hpp"
#include "Config.hpp"
#include "Client.hpp"


typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr_storage sockaddr_storage;

class Core
{
private:

	uint32_t 						_servSize;
	int 							_sockfd[10];		//TODO make dynamic(server count)
	pollfd 							_fdset[MAXCLIENTS];
	std::vector<Client> 			_clientList;
	std::vector<Server>				_servers;

public:

	Core							( Config &config );
	~Core							( void );

	void startServ					( void );

private:

	bool 		initSocets			( void );
	void 		mainLoop			( void );
	void 		acceptCliConnect	( const int sock, Server const & server, int &newSock, nfds_t &numFds);


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