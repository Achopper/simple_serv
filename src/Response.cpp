
#include "../inc/Response.hpp"

std::map<std::string, std::string> Response::setStatusCode()
{
	std::map<std::string, std::string> res;
	res.insert(std::pair<std::string, std::string>("100", "Continue"));
	res.insert(std::pair<std::string, std::string>("200", "OK"));

	res.insert(std::pair<std::string, std::string>("403", "Forbidden"));
	res.insert(std::pair<std::string, std::string>("404", "Not Found"));
	res.insert(std::pair<std::string, std::string>("413", "Payload Too Large"));
	res.insert(std::pair<std::string, std::string>("405", "Method Not Allowed"));
	res.insert(std::pair<std::string, std::string>("408", "Request Timeout"));

	res.insert(std::pair<std::string, std::string>("505", "HTTP Version Not Supported"));



	return (res);
}

std::map<std::string, std::string> Response::setContentType()
{
	std::map<std::string, std::string> res;

	res["html"] = "text/html";
	res["ico"] = "image/x-icon";

	return (res);
}

std::map<std::string, std::string>Response::_statusCodes = Response::setStatusCode();
std::map<std::string, std::string>Response::_contentType = Response::setContentType();

Response::Response(std::string &method, Client & client) : _method(method), _client(client)
{
}

Response::Response(const Response &obj) : _client(obj._client)
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
		_code = obj._code;
		_method = obj._method;
		_client = obj._client;
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

void Response::addCodetoResp(const std::string &code)
{
	_response = PROT " " + code + " " + _statusCodes[code] + "\r\n";
}

void Response::addContentLen(const std::string::size_type &len)
{
	_response.append("Content-Length: " + std::to_string(len) + "\r\n\r\n");
}

void Response::addContentType(std::string const & filePath)
{
	std::string fpath = filePath;
	std::string::size_type pos = fpath.find('.');
	if (pos == std::string::npos)
		fpath = "html";
	_response.append("Content-Type: " + _contentType[fpath.substr(pos + 1, fpath.length())] + "\r\n");
}

void Response::addServerName(std::string const & serverName)
{
	std::string tmp = serverName;
	if (serverName.length() < 1)
		tmp = "Unnamed";
	_response.append("Server-name: " + tmp + "\r\n");
}

void Response::fillResponse()
{
	DefaultPage errorPage;
	std::map<std::string, std::string> er = _client.getServer().getErrPage();
	if (_client.prot != PROT && _code != "408")
		_code = "505";
	addCodetoResp(_code);
	if ((_code.at(0) == '4' || _code.at(0) == '5') && er.count(_code) > 0)
		getPage(_client.getServer().getRoot() + er[_code]);
	else if ((_code.at(0) == '4' || _code.at(0) == '5'))
		_body = errorPage.makePage(_code, _statusCodes[_code], _client.getServer().getServName());
	addContentType(_client.path);
	addServerName(_client.getServer().getServName());
	addContentLen(_body.length());

	_response += _body;
}

bool Response::getPage(std::string const &path)
{
	_body = "";
	std::string line;
	std::ifstream page(path, std::ios::binary);
	if (!page.is_open())
	{
		std::cout << REDCOL"Cant open" << RESCOL << std::endl;
		_code = "404";
		return (false);
	}
	else
	{
		_code = "200";
		while (std::getline(page, line))
			_body += line + "\n";
	}
	page.close();
	return true;
}


bool Response::GET(Client &client)
{
	DefaultPage page;
	const Server& server = client.getServer();
	const std::vector<Location> & loclist = server.getLocList();
	for (std::vector<Location>::const_iterator it = loclist.begin(); it != loclist.end(); ++it)
	{
		if (client.path == it->getPath())
		{
			if (it->getIndex().empty())
			{
				_body = page.makePage("", "Welcome to ", server.getServName());
				_code = "200";
			}
			else if (!getPage(server.getRoot() + it->getRoot() + "/" + it->getIndex()))
				return (false);
			if (_body.length() > it->getClientSize())
			{
				_code = "413";
				return (false);
			}
			return (true);
		}
	}
	_code = "404";
	return (false);
}





