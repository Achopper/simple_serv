

#ifndef SERV_INCLUDES_HPP
#define SERV_INCLUDES_HPP

#define DEBUG_MODE 1
#define REDCOL "\33[31m"
#define RESCOL "\33[00m"
#define GREENCOL "\33[32m"
//#define MAXCLIENTS 1024
#define PROT HTTP/1.1
#define TIMEOUT -1
#define CLI_TIMEOUT_SEC 20

typedef struct pollfd pollfd;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr_storage sockaddr_storage;

#include <list>
#include <vector>
#include <iostream>
#include <fstream>

#include <sys/fcntl.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <machine/types.h>
#include <unistd.h>



#endif //SERV_INCLUDES_HPP
