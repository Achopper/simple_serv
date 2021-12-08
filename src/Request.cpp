
#include "../inc/Request.hpp"
#include "../inc/serv_includes.hpp"

Request::Request ( void )
{
	_isFirstLineSet = 0;
	_isHeadersEnd = 0;
	_isBodyEnd = 0;
	_bodySize = 0;
	// _parsStatus = PARSE_FIRST_LINE
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

	void	Request::setUrl(){}
	void	Request::setBuf(){}
	void	Request::setBody(){}
	void	Request::setMethod(){}
	void	Request::setHttpVersion(){}
	void	Request::setQueryString(){}

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
	std::string	Request::getHttpVersion(){
		return _httpVersion;
	}
	std::string	Request::getQueryString(){
		return _queryString;
	}
std::map<std::string, std::string>	Request::getHeadersMap(){
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
	_buf += req;
	size_t endLine = _buf.find("\r\n") ;
	size_t endHeaders = _buf.find("\r\n\r\n") ;
	
	while(endLine != static_cast<size_t>(-1) && !_isBodyEnd)
	{
		if (!_isFirstLineSet)
		{	
			std::string	str;
			for ( std::string::iterator it=_buf.begin(); *it!='\r'; ++it)
				str += *it ;

			std::vector<std::string> firstLine = split2(str, " ");
			if (firstLine.size() == 3)
			{
				_method = firstLine[0] ;
				_url = firstLine[1] ;
				_httpVersion = firstLine[2] ;
				_isFirstLineSet = 1;
			}
			else
				return ;// err // throw exeption()
			
			_buf.erase(0, endLine + 2) ;
			str.clear() ;
		}
		else if (_isFirstLineSet && !_isHeadersEnd)
		{	
			std::string	str2;
			for ( std::string::iterator it=_buf.begin(); *it!='\r'; ++it)
				str2 += *it ;
							
			std::vector<std::string> headLines = split2(str2, ": ");
			if (headLines.size() == 2)
				_headersMap[headLines[0]] = headLines[1];
			// else
				// throw exeption()
			// std::cout << "endLine " << endLine << "  " << "endHeaders " << endHeaders << std::endl ;

			if (endHeaders == endLine )
			{
				_buf.erase(0, endLine + 4) ;
				str2.clear();
				_isHeadersEnd = 1;
			}
			else
			{
				_buf.erase(0, endLine + 2) ;
				str2.clear();
			}
			std::string сonLen("Content-Length");
			if (!_headersMap[сonLen].empty())
				_bodySize = static_cast<size_t>(stoll(_headersMap[сonLen]));//С++11
		}
		else if (_isHeadersEnd && _method == "POST")
		{
			for ( std::string::iterator it=_buf.begin(); it!=_buf.end() && _bodySize; ++it)
			{
				_body += *it ;
				--_bodySize;
			}
			if (_bodySize == 0)
				_isBodyEnd = 1 ;
		}
		endLine = _buf.find("\r\n") ;
		endHeaders = _buf.find("\r\n\r\n") ;

	}
	// for (std::map<std::string, std::string>::iterator it = _headersMap.begin(); it != _headersMap.end(); ++it)
	// std::cout << "|" << it->first << "|" << " : " << "|" << it->second << "|" << std::endl;
}
