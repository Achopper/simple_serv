
#include "../inc/Request.hpp"
#include "../inc/serv_includes.hpp"

Request::Request ( void )
{
	// _isUrlSet = 0;
	// _isMetodSet = 0;
	// _isHttpVerSet = 0;
	_isFirstLineSet = 0;
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
		// _isHeadersEnd
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
	std::string	Request::getReqBody(){
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

void	Request::parseReq(std::string req){
	_buf += req;
	size_t endFind = _buf.find("\r\n") ;
	
	while(endFind != static_cast<size_t>(-1))
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
				return ;// err
			
			_buf.erase(0, endFind + 2) ;
			str.clear() ;
		}
		if (_isFirstLineSet != 0)
		{	
			std::string	str2;
			for ( std::string::iterator it=_buf.begin(); *it!='\r'; ++it)
				str2 += *it ;
							
			std::vector<std::string> headLines = split2(str2, ": ");
			if (headLines.size() == 2)
				_headersMap[headLines[0]] = headLines[1];

			
			_buf.erase(0, endFind + 2) ;
			str2.clear();
		}
		endFind = _buf.find("\r\n") ;
	}
	// for (std::map<std::string, std::string>::iterator it = _headersMap.begin(); it != _headersMap.end(); ++it)
	// std::cout << "|" << it->first << "|" << " : " << "|" << it->second << "|" << std::endl;
}
