
#ifndef CORE_HPP
#define CORE_HPP


#include "serv_includes.hpp"
#include "Config.hpp"
#include "Client.hpp"
#include "Response.hpp"


class Core
{
private:

	uint32_t 							_servSize;
	std::vector<int> 					_sockfd;
	pollfd 								_fdset[OPEN_MAX];
	std::list<Client> 					_clientList;
	std::vector<Server>					_servers;

public:

	Core								( Config &config );
	~Core								( void );

	void startServ						( void );

private:

	bool 		initSocets				( void );
	void 		mainLoop				( void );
	bool 		acceptClientConnect		( std::vector<Server>::iterator& it, nfds_t& num );
	void 		readRequest				( std::list<Client>::iterator &it, nfds_t& num );
	bool 		sendResponce			( std::list<Client>::iterator &it, nfds_t& num);


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
