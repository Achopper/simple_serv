#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <poll.h>
#include <sstream>
#include <fstream>
#include <sys/wait.h>
#include <curl/curl.h>

#define BUFLEN  4096

#define STDIN		0
#define STDOUT		1
#define STDERR		2

std::string get_response(std::string msg){

    std::stringstream http;
    std::stringstream html;

    html << "<!DOCTYPE html>\r\n";
    html << "<html>\r\n";
    html << "<head>\r\n";
    html << "<meta charset = \"cp1251\">\r\n";
    html << "<title>Ecole server</title>\r\n";
    html << "<head>\r\n";
    html << "<body>\r\n";
    html << "<h2>" << msg << "</h2>\r\n";
    html << "<body>\r\n";
    html << "<html>\r\n";

    http << "HTTP/1.1 200 OK\r\n";
    http << "Connection: keep-alive\r\n";
    http << "Content-type: text/html\r\n";
    http << "Content-length: " << html.str().length() << "\r\n";
    http << "\r\n";
    http << html.str();

    return http.str();
}

int main(int ac, char **av, char **env){

    std::string msg = "You are in another CGI!";

    std::string response = get_response(msg);
    send(1, response.c_str(), response.length() + 1, 0);
}

//g++ cgi.cpp -o cgi
