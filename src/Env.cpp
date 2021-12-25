#include "../inc/Core.hpp"
#include "../inc/Env.hpp"


Env::Env():_arrRows(0){}

Env::~Env(){
}

Env::Env(Env const &other){
	*this = other;
}
Env &Env::operator=(const Env &other){

	if (this != &other)
	{
		_envMap = other._envMap;
		_envArr = other._envArr;
		_arrRows = other._arrRows;
		_server = other._server;

	}
	return (*this);
}

std::map<std::string, std::string>& Env::getEnvMap()
{
	return _envMap;
}

char	**Env::getEnvArr()
{
	return _envArr;
}

void	Env::addEnvToMap(){

	extern char **environ;
	std::string str;
	std::vector<std::string> strVec;
	size_t j = 0;

	for(size_t k = 0; environ[k] ; ++k)
	{
		for(; j < strlen(environ[k]); ++j)
			str += environ[k][j];
		strVec.push_back(str);
		j = 0;
		str.clear();
	}
	std::vector<std::string>::iterator it = strVec.begin();
	std::vector<std::string>::iterator ite = strVec.end();
	for (;it != ite; ++it)
	{
		std::string s = *it;
		std::string first = s.substr(0, s.find('='));
		std::string second = s.substr(s.find("=")+1);
		_envMap[first] = second;
	}
}

size_t	&Env::getArrRows()
{
	return _arrRows;
}

char	**Env::makeEnvArr(){

	char **envArr ;
	addEnvToMap();

    envArr = new char *[_envMap.size() + 1];
    envArr[_envMap.size()] = NULL;
    std::map<std::string, std::string>::const_iterator it = _envMap.begin();
    for (; it != _envMap.end(); ++it, ++_arrRows)
	{
        envArr[_arrRows] = strdup((it->first + "=" + it->second).c_str());
	}

	for(size_t k = 0; k < _arrRows ; ++k)
	{
		for(size_t j = 0; j < strlen(envArr[k]); ++j)
			std::cout << envArr[k][j];
    	std::cout << std::endl;
	}
	return envArr;
}

// void	Env::setEnvArr(){

// 	addEnvToMap();

//     _envArr = new char *[_envMap.size() + 1];
// 	if (!_envArr)
// 		throw std::exception();
//     _envArr[_envMap.size()] = NULL;
//     std::map<std::string, std::string>::const_iterator it = _envMap.begin();
//     for (; it != _envMap.end(); ++it, ++_arrRows)
// 	{
//         _envArr[_arrRows] = strdup((it->first + "=" + it->second).c_str());
// 	}
// 	// print arr
// 	for(size_t k = 0; k < _arrRows ; ++k)
// 	{
// 		for(size_t j = 0; j < strlen(_envArr[k]); ++j)
// 			std::cout << _envArr[k][j];
//     	std::cout << std::endl;
// 	}
// }

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
	_envMap["REQUEST"] = request.getMethod();
	_envMap["URL"] = request.getUrl();



	// for (std::map<std::string, std::string>::iterator it = _envMap.begin(); it != _envMap.end(); ++it)
	// 	std::cout << "|" << it->first << "|" << " : " << "|" << it->second << "|" << std::endl;
}

void Env::setServer(Server const *server)
{
	_server = server;
}
