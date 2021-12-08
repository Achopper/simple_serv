
#include "../inc/Response.hpp"

std::map<std::string, std::string> Response::setStatusCode()
{
	std::map<std::string, std::string> res;
	res.insert(std::pair<std::string, std::string>("100", "Continue"));
	res.insert(std::pair<std::string, std::string>("200", "OK"));
	res.insert(std::pair<std::string, std::string>("301", "Moved Permanently"));

	res.insert(std::pair<std::string, std::string>("403", "Forbidden"));
	res.insert(std::pair<std::string, std::string>("404", "Not Found"));
	res.insert(std::pair<std::string, std::string>("405", "Method Not Allowed"));
	res.insert(std::pair<std::string, std::string>("408", "Request Timeout"));
	res.insert(std::pair<std::string, std::string>("413", "Payload Too Large"));

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

Response::Response(Server &server,Request & request ) :  _server(server), _request(request)
{
}

Response::Response()
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
		_code = obj._code;
		_method = obj._method;
		_server = obj._server;
		_request = obj._request;
		_maxLen = obj._maxLen;
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

void Response::setServer(const Server &server)
{
	_server = server;
}

void Response::setRequest(const Request &request)
{
	_request = request;
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
	std::map<std::string, std::string> er = _server.getErrPage();
	addCodetoResp(_code);
	if ((_code.at(0) == '4' || _code.at(0) == '5') && er.count(_code) > 0)
		getPage(_server.getRoot() + er[_code]);
	else if ((_code.at(0) == '4' || _code.at(0) == '5'))
		_body = errorPage.makePage(_code, _statusCodes[_code], _server.getServName());
	addContentType(_request.getUrl());
	addServerName(_server.getServName());
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
		while (std::getline(page, line))
		{
			_body += line + "\n";
			if (_body.length() > _maxLen)
			{
				_code = "413";
				page.close();
				return (false);
			}
		}
		_code.length() > 0 ? "red" :_code = "200";
	}
	page.close();
	return true;
}
void Response::makeRedirect(std::vector<Location> const & locList, std::vector<Location>::const_iterator & it,
							std::string & url)
{
	for (std::vector<Location>::const_iterator _iter = locList.begin(); _iter != locList.end(); ++_iter)
		if (_iter->getName() == it->getPathToRedir())
		{
			it = _iter;
			url = it->getPath();
			_code = "301";
			break;
		}
}

bool Response::checkLocation(std::vector<Location>::const_iterator &iter, std::string const &reqPath, std::string & url,
							 bool &isF)
{
	std::string::size_type nameLen = iter->getName().length();
	std::string::size_type pos;
	std::string _filename;
	std::string _prefix;
	std::string _postfix;

	_prefix = reqPath.substr(0, nameLen);
	_postfix = reqPath.substr(_prefix.length(), reqPath.length() - _prefix.length());
	if ( !_postfix.empty() && _postfix.at(0) != '/')
		_postfix = '/' + _postfix;
	pos = _postfix.find('.');
	if (pos != std::string::npos)
	{
		isF = true;
		if (_postfix == _filename)
			_postfix = "";
		_filename = std::string(std::strrchr(reqPath.c_str(), '/'));
		_postfix = _postfix.substr(0,_postfix.length() - _filename.length());
	}
	if (_prefix == iter->getName())
	{
		url = iter->getPath() + _postfix + _filename;
		return (true);
	}
	return (false);
}

bool Response::GET()
{
	DefaultPage page;
	std::string url;
	bool isFile = false;
	const std::vector<Location> & loclist = _server.getLocList();
	for (std::vector<Location>::const_iterator iter = loclist.begin(); iter != loclist.end(); ++iter)
	{
		if (checkLocation(iter, _request.getUrl(), url, isFile))
		{
			if (!iter->getPathToRedir().empty())
				makeRedirect(loclist, iter, url);
			_maxLen = iter->getClientSize();
			if (!iter->getMethods()[_request.getMethod()])
			{
				_code = "405";
				return (false);
			}
			if (!iter->getAutoindex() && iter->getIndex().empty() && url.empty())
				_body = page.makePage("200", "Welcome to", _server.getServName());
			else if (iter->getIndex().length() == 0 && iter->getAutoindex() && !isFile)
			{
				Autoindex autoindex;
				if (!autoindex.makePage(url, iter->getName()))
				{
					_code = "404";
					return (false);
				}
				if ((_body = autoindex.getPage()).length() > _maxLen)
				{
					_code = "413";
					return (false);
				}
				_code.length() > 0 ? "red" : _code = "200";
				return (true);
			}
			else if (isFile && iter->getIndex().empty())
			{
				if (!getPage(url))
					return (false);
				return (true);
			}
			else if (!iter->getIndex().empty())
			{
				if (*url.end() != '/')
					url.append("/");
				if (!getPage(url + iter->getIndex()))
					return (false);
				return (true);
			}
			else
				break;
		}
	}
	_code = "403";
	return (false);
}

bool Response::DELETE()
{
	std::string url;
	bool isFile = false;
	const std::vector<Location> & loclist = _server.getLocList();
	for (std::vector<Location>::const_iterator iter = loclist.begin(); iter != loclist.end(); ++iter)
	{
		if (checkLocation(iter, _request.getUrl(), url, isFile))
		{
			if (!iter->getPathToRedir().empty())
			{
				makeRedirect(loclist, iter, url);
				_code = "301";
			}
			if (!iter->getMethods()[_request.getMethod()])
			{
				_code = "405";
				return (false);
			}
			if (!isFile)
				break;
			else
			{
				if (unlink(url.data()) != 0)
				{
					if	(errno == ENOENT)
					{
						_code = "404";
						return (false);
					}
					else
						break;
				}
				_code = "200";
				return (true);
			}
		}
	}
	_code = "403";
	return false;
}

bool Response::POST(void)
{
	return false;
}






