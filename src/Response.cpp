
#include "../inc/Response.hpp"

Response::Response(std::string &method) : _method(method)
{
}

Response::Response(const Response &obj)
{
	*this = obj;
}

Response::~Response(void)
{

}

Response &Response::operator=(const Response &obj)
{
	if (this != &obj)
	{
		_response = obj._response;
		_body = obj._body;
	}
	return (*this);
}

bool Response::setResp(const std::string &response)
{
	_response = response;
	return (true);
}

bool Response::setBody(const std::string &body)
{
	_body = body;
	return (true);
}

void Response::setCode(const std::string &code)
{
	_code = code;
}

std::string Response::getCode() const
{
	return (_code);
}

const std::string &Response::getBody() const
{
	return (_body);
}

const std::string &Response::getResp() const
{
	return (_response);
}

std::string Response::getMethod() const
{
	return (_method);
}

bool Response::getPage(std::string const &path)
{
	std::string line, res;
	std::ifstream page(path, std::ios::binary);
	if (!page.is_open())
	{
		std::cout << REDCOL"Cant open" << RESCOL << std::endl;
		return (false);
	}
	else
		while (std::getline(page, line))
			res += line + "\n";
	_response += "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(_response.length() +
			res.length()) + "\n\n" + res;
	page.close();
	return true;
}


bool Response::GET(Client &client)
{
	const Server& server = client.getServer();
	const std::vector<Location> & loclist = server.getLocList();
	for (std::vector<Location>::const_iterator it = loclist.begin(); it != loclist.end(); ++it)
	{
		if (it->getPath() == client.path)
		{
			getPage(server.getRoot() + it->getRoot() + "/" + it->getIndex());
			return (true);
		}
	}
	return (false);
}





