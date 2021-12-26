

#ifndef SERV_INCLUDES_HPP
#define SERV_INCLUDES_HPP

#define DEBUG_MODE 0
#define REDCOL "\33[31m"
#define RESCOL "\33[00m"
#define GREENCOL "\33[32m"

#define PROT "HTTP/1.1"
//Define download directory && cgi location name
#define DOWNLOAD_DIR "/downloads"
#define CGI_NAME "/cgi-bin"
#define TIMEOUT -1
#define CLI_TIMEOUT_SEC 30
#define DEF_CLI_MAX_BDY_SZ 1000*1024


typedef struct pollfd pollfd;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr_storage sockaddr_storage;


#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>

#include <sys/fcntl.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>



#endif //SERV_INCLUDES_HPP
