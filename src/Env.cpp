#include "../inc/Core.hpp"
// #include "../inc/Env.hpp"

Env::Env(){}

Env::~Env(){}

Env::Env(Env const &other){
	*this = other;
}
Env &Env::operator=(const Env &other){
		if (this != &other)
	{
		_envMap = other._envMap;
		_envArr = other._envArr;
	}
	return (*this);
}

std::map<std::string, std::string>& Env::getEnvMap(){return _envMap;}
char	*Env::getEnvArr(){return _envArr;}

void	Env::setEnvArr(){}
// void	Env::addServEnvToMap(Server &server){
	// _envMap["AUTH_TYPE"] = server.get...();
	// _envMap["GETAWAY_INTERFACE"] = server.get...();
	// _envMap["PATH_INFO"] = server.get...();
	// _envMap["PATH_TRANSLATED"] = server.get...();
	// _envMap["REMOTE_ADDR"] = server.get...();
	// _envMap["REMOTE_HOST"] = server.get...();
	// _envMap["REMOTE_PORT"] = server.get...();
	// _envMap["REMOTE_USER"] = server.get...();
	// _envMap["SERVER_ADDR"] = server.get...();
	// _envMap["SERVER_NAME"] = server.get...();
	// _envMap["SERVER_PORT"] = server.get...();
	// _envMap["SERVER_PROTOCOL"] = server.get...();
	// _envMap["SERVER_SOFTWARE"] = server.get...();
	// _envMap["SCRIPT_NAME"] = server.get...();
	// _envMap["SCRIPT_FILENAME"] = server.get...();

// // Специальные переменные сервера Apache:
// // 	DOCUMENT_ROOT
// // 	SERVER_ADMIN
// // 	SERVER_SIGNATURE
// }
void	Env::addHttpEnvToMap(Request &request){
	
	// std::map<std::string, std::string>::iterator it = request.getHeadersMap().find("Host");
	// if (it  != request.getHeadersMap().end())
	// 	std::cout << "Host is here!!!!!" << std::endl;

	_envMap["HTTP_HOST"] = request.getHeadersMap()["Host"];
	_envMap["HTTP_ACCEPT"] = request.getHeadersMap()["Accept"];
	_envMap["HTTP_USER_AGENT"] = request.getHeadersMap()["User-Agent"];
	_envMap["HTTP_ACCEPT_LANGUAGE"] = request.getHeadersMap()["Accept-Language"];
	_envMap["HTTP_ACCEPT_ENCODING"] = request.getHeadersMap()["Accept-Encoding"];
	_envMap["HTTP_ACCEPT_CHARSET"] = request.getHeadersMap()["Accept-Charset"];
	_envMap["HTTP_CONNECTION"] = request.getHeadersMap()["Connection"];
	_envMap["HTTP_X_FORWARDED_FOR"] = request.getHeadersMap()[""]; //?
	_envMap["HTTP_REFERER"] = request.getHeadersMap()["Referer"];
	_envMap["HTTP_REFERER"] = request.getHeadersMap()["Referer"];

	_envMap["CONTENT_LENGTH"] = request.getHeadersMap()["Content-Length"];
	_envMap["CONTENT_TYPE"] = request.getHeadersMap()["Content-Type"];
	_envMap["QUERY_STRING"] = request.getQueryString();

	// _envMap[""] = request.getHeadersMap()[""];

	for (std::map<std::string, std::string>::iterator it = _envMap.begin(); it != _envMap.end(); ++it)
		std::cout << "|" << it->first << "|" << " : " << "|" << it->second << "|" << std::endl;
}
