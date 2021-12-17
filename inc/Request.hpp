

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>
#include <vector>

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

	std::string								_method;
	std::string								_url;
	std::string								_httpVersion;
	std::map<std::string, std::string>		_headersMap;
	std::string 							_queryString;
	std::string								_buf;
	std::string								_body;
	size_t									_bodySize;


public:

	Request									( void );
	Request									( Request const& obj );
	~Request								( void );
	Request&								operator=( Request const& obj );

	void									setUrl();
	void									setBuf();
	void									setBody();
	void									setMethod();
	void									setHttpVersion();
	void									setQueryString();

	std::string								getUrl();
	std::string								getBuf();
	std::string								getBody();
	std::string								getMethod();
	std::string								getHttpVersion();
	std::string								getQueryString();

	std::map<std::string, std::string>		getHeadersMap();

	void									parseReq(std::string const & req);


};

std::vector<std::string> split2(const std::string& str, const std::string& delim) ;

#endif //CLIENT_HPP
