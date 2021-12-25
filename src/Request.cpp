
// #include "../inc/Request.hpp"
#include "../inc/Core.hpp"
#include "../inc/serv_includes.hpp"

Request::Request ( void )
{
	_isBodyEnd = false ;
	_isChunked = false ;
	_isRequestEnd = false ;
	_isHeadersEnd = false ;
	_isFirstLineSet = false ;
	_readingBodySize = true ;
	_readingBody = false ;
	_bodySize = 0;
	_firstLineSize = 0;
	_firstLineMaxSize = 2063;//2048 + 15
}

Request::Request(const Request &obj)
{
	*this = obj;
}

Request::~Request(void)
{
}

Request &Request::operator=(const Request &obj)
{
	if (this != &obj)
	{
		_isFirstLineSet = obj._isFirstLineSet;
		_isHeadersEnd = obj._isHeadersEnd;
		_method = obj._method;
		_url = obj._url;
		_httpVersion = obj._httpVersion;
		_headersMap = obj._headersMap;
		_body = obj._body;
		_queryString = obj._queryString;
		_buf = obj._buf;
	}
	return (*this);
}

void	Request::addBuf(std::string const  & req){

	_buf += req;
}

void	Request::setUri(std::string& str){
	_uri = str;
}
void	Request::setUrl(std::string& str, size_t& findQ){
	if (findQ != std::string::npos)
		_url = str.substr(0, findQ);
	else
		_url = str;
}
void	Request::setMethod(std::string& str){
	_method = str;
}
void	Request::setHttpVersion(std::string& str){
	_httpVersion = str ;
}
void	Request::setQueryString(std::string& str, size_t &findQ){
	if (findQ == std::string::npos)
		return ;
	_queryString = str.substr(findQ + 1, str.size()) ;
}
void	Request::setBody(){

	checkBodyHeder();
	for ( std::string::iterator it=_buf.begin(); it!=_buf.end() && _bodySize; ++it)
	{
		_body += *it ;
		--_bodySize;
	}
	if (_bodySize == 0)
		_isBodyEnd = true ;
}
void	Request::setChunkedBody(){
	std::string str;
	while (!_isBodyEnd)
	{
		if (_readingBodySize)
		{
			for ( std::string::iterator it=_buf.begin(); *it!='\r'; ++it)
				str += *it ;
			_bodySize = static_cast<size_t>(stoll(str));//atoi(c_str(str))
			if (_bodySize == 0)
			{
				_isBodyEnd = true;
				return ;
			}
			_readingBody = true;
			_readingBodySize = false;
		}
		else if(_readingBody)
		{
			if(_buf.size() >= _bodySize)
			{
				for ( std::string::iterator it=_buf.begin(); it!=_buf.end() && _bodySize; ++it)
				{
					str += *it ;
					_body += *it ;
					--_bodySize;
				}
				_readingBody = false;
				_readingBodySize = true;
			}
		}
		_buf.erase(0, str.size() + 2) ;
		str.clear();
	}
}
void	Request::setBodySize(){
	std::string сonLen("Content-Length");
	if (!_headersMap[сonLen].empty())
		_bodySize = static_cast<size_t>(stoll(_headersMap[сonLen]));//С++11
}
void	Request::setIsChunked(){
	std::string transEncod("Transfer-Encoding");
	if (!_headersMap[transEncod].empty())
		if (_headersMap[transEncod] == "chunked")
			_isChunked = true ;
}
void	Request::setFirstLine(size_t &endLine){
	std::string	str;	
	for ( std::string::iterator it=_buf.begin(); *it!='\r'; ++it)
	{
		str += *it ;
		++_firstLineSize;
		if (_firstLineSize == _firstLineMaxSize)
		{
			_errCode = "414" ;
			throw  std::exception();
		}
	}
	std::vector<std::string> firstLine = split2(str, " ");
	if (firstLine.size() != 3)
	{
		_errCode = "400";
		throw std::exception();
	}
	checkMethod(firstLine[0]);
	setMethod(firstLine[0]) ;

	size_t findQ = firstLine[1].find("?");
	setUri(firstLine[1]) ;
	setUrl(firstLine[1], findQ) ;
	setQueryString(firstLine[1], findQ) ;

	checkHttpVersion(firstLine[2]);
	setHttpVersion(firstLine[2]) ;
	
	_isFirstLineSet = true;
	_buf.erase(0, endLine + 2) ;
}
void	Request::setHeadersMap(size_t & endLine, size_t & endHeaders){

	if (_buf == "\r\n")
	{
		_buf.erase(0, endLine + 2);
		return;
	}
	std::string	str = _buf.substr(0, endLine);
	std::vector<std::string> headLines = split2(str, ": ");
	if (headLines.size() == 2)
		_headersMap[headLines[0]] = headLines[1];
	else
	{
		_errCode = "400";
		throw std::exception();
	}
	if (endHeaders == endLine )
	{
		_buf.erase(0, endLine + 4) ;
		_isHeadersEnd = true;
	}
	else
		_buf.erase(0, endLine + 2) ;
	str.clear();
}

std::string	Request::getUri(){
	return _uri;
}
std::string	Request::getUrl(){
	return _url;
}
std::string	Request::getBuf(){
	return _buf;
}
std::string	Request::getBody(){
	return _body;
}
std::string	Request::getMethod(){
	return _method;
}
std::string	Request::getErrCode(){
	return _errCode;
}
std::string	Request::getHttpVersion(){
	return _httpVersion;
}
std::string	Request::getQueryString(){
		return _queryString;
	}
bool	Request::getIsRequestEnd(){
	return _isRequestEnd;
}
std::map<std::string, std::string>&	Request::getHeadersMap(){
	return _headersMap;
}

std::vector<std::string> split2(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}


void	Request::parseReq(std::string const & req){
	
	addBuf(req);
	size_t endLine = _buf.find("\r\n") ;
	size_t endHeaders = _buf.find("\r\n\r\n") ;

	while(endLine != static_cast<size_t>(-1) && !_isBodyEnd)
	{
	//	std::cout << req << std::endl;
//		if (_buf == "\r\n")
//			return;
		if (!_isFirstLineSet)
			setFirstLine(endLine);
		else if ((_isFirstLineSet && !_isHeadersEnd))
		{
			if (_buf == "\r\n")
			{
				_isHeadersEnd = true;
				//_isBodyEnd = true;
			}
			else
			{
				setHeadersMap(endLine, endHeaders);
				setBodySize();
				setIsChunked();
			}
		}
		if (_isHeadersEnd && (_method == "POST" || _method == "DELETE"))
		{
			if (!_isChunked)
				setBody();
			else
				setChunkedBody();
		}

		if (((_method == "GET" || _method == "HEAD") && _isHeadersEnd)
		|| ((_method == "POST" || _method == "DELETE") && _isBodyEnd))
		{	
			_isRequestEnd = true;
			if (_method == "GET" || _method == "HEAD")
				break;
		}
		endLine = _buf.find("\r\n") ;
		endHeaders = _buf.find("\r\n\r\n") ;
	}
	// for (std::map<std::string, std::string>::iterator it = _headersMap.begin(); it != _headersMap.end(); ++it)
	// std::cout << "|" << it->first << "|" << " : " << "|" << it->second << "|" << std::endl;
}

void	Request::checkMethod(std::string &method){
	if (method != "POST" && method != "GET" && method != "DELETE" && method != "HEAD")
	{
		std::string msg = "Wrong method";
		_errCode = "405";
		throw std::exception();
	}
}
void	Request::checkHttpVersion(std::string& httpVersion){
	if (httpVersion != "HTTP/1.1")
	{
		_errCode = "505";
		throw std::exception();
	}
}
void	Request::checkBodyHeder(){
	std::string сonLen("Content-Length");
	if (_headersMap[сonLen].empty())
	{
		std::cout << "!!!!!!!!! no сonLen";
		throw std::exception();
	}
}