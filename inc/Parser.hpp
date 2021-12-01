#ifndef PARSER_HPP
#define PARSER_HPP

#include "Core.hpp"

class Parser
{
	public:

		Parser		( void );
		~Parser		( void );
		Parser		( Parser const& obj );
		Parser&		operator=( Parser const& obj );

		void		setUrl();
		void		setBuf();
		void		setBody();
		void		setMethod();
		void		setHttpVersion();
		void		setQueryString();

		std::string	getUrl();
		std::string	getBuf();
		std::string	getBody();
		std::string	getMethod();
		std::string	getHttpVersion();
		std::string	getQueryString();

	private:

		std::string								_method;
		std::string								_url;
		std::string								_httpVersion;
		//std::map<std::string, std::string>	headers; // header -> content
		std::string								_body;
		std::string 							_queryString;
		std::string								_buf;
};

#endif //PARSER_HPP