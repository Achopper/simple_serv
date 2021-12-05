

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>

// #define PARSE_FIRST_LINE 1
// #define PARSE_HEADERS 2
// #define PARSE_BODY 3
// #define PARSE_END 4


class Request
{

private:

	bool									_isFirstLineSet;
	bool									_isHeadersEnd;
	bool									_isBodyEnd;

	std::string								_errCode;
	std::string								_method;
	std::string								_url;
	std::string								_httpVersion;
	std::map<std::string, std::string>		_headersMap;
	std::string 							_queryString;
	std::string								_buf;
	std::string								_body;
	size_t									_bodySize;
	int										_firstLineMaxSize;
	int										_firstLineSize;


public:

	Request									( void );
	Request									( Request const& obj );
	~Request								( void );
	Request&								operator=( Request const& obj );

	void									setUrl();
	void									setBuf();
	void									setBody();
	void									setMethod();
	void									setBodySize();
	void									setHttpVersion();
	void									setQueryString();
	void									setHeadersMap(size_t endLine, size_t endHeaders);
	void									setFirstLine(size_t endLine);

	std::string								getUrl();
	std::string								getBuf();
	std::string								getBody();
	std::string								getMethod();
	std::string								getHttpVersion();
	std::string								getQueryString();

	std::map<std::string, std::string>		getHeadersMap();

	void									parseReq(std::string req);
	void									checkHttpVersion(std::string httpVersion);
	void									checkMethod(std::string method);


};

std::vector<std::string> split2(const std::string& str, const std::string& delim) ;

#endif //CLIENT_HPP
