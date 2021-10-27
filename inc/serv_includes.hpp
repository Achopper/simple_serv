

#ifndef SERV_INCLUDES_HPP
#define SERV_INCLUDES_HPP

#define DEBUG_MODE 1
#define REDCOL "\33[31m"
#define RESCOL "\33[00m"
#define MAXCLIENTS 1024
#define PROT HTTP/1.1
#define TIMEOUT -1

typedef struct pollfd pollfd;

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
