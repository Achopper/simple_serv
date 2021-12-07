
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
	if (_request.getHttpVersion() != PROT && _code != "408")
		_code = "505";
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
			_body += line + "\n";
		_code.length() > 0 ? "red" :_code = "200";
	}
	page.close();
	return true;
}
void Response::makeRedirect(std::vector<Location> const & locList, std::vector<Location>::const_iterator & it)
{
	for (std::vector<Location>::const_iterator _iter = locList.begin(); _iter != locList.end(); ++_iter)
		if (_iter->getPath() == it->getPathToRedir())
		{
			it = _iter;
			_code = "301";
			break;
		}
}

bool Response::checkLocation(std::string const &locPath, std::string const &reqPath, std::string & url,
							 bool &isF)
{
	std::string _reqPath;
	std::string _filename;
	std::string _prefix;
	std::string::size_type pos = reqPath.find('.');
	if (pos != std::string::npos)
	{
		_filename = std::string(std::strrchr(reqPath.c_str(), '/'));
		_filename = _filename.substr(1, _filename.length());
		isF = true;
	}
	if ((strrchr(reqPath.c_str(), '/') - &reqPath[0] == 0) && _filename.length() == 0)
		_prefix = reqPath;
	else if ((strrchr(reqPath.c_str(), '/') - &reqPath[0] == 0) && _filename.length() > 1)
		_prefix = '/';
	else
		_prefix = reqPath.substr(0, (unsigned long)(strrchr(reqPath.c_str(), '/') - &reqPath[0]));
	if (locPath == _prefix || _prefix == reqPath)
	{
		if (isF)
			url = '/' + _filename;
		else
			_prefix.length() > 1 ? url = _prefix + '/' : url = _prefix;
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
		for (std::vector<Location>::const_iterator _iter = loclist.begin(); _iter != loclist.end(); ++_iter)
		{
			if (_request.getUrl() == _iter->getPath())
				iter = _iter;
		}
		if (checkLocation(iter->getPath(), _request.getUrl(), url, isFile))
		{
			if (iter->isRedirect())
			{
				url = iter->getPathToRedir();
				makeRedirect(loclist, iter);
			}
			if (!(iter->getMethods()[_request.getMethod()]))
			{
				_code = "405";
				return (false);
			}
#if DEBUG_MODE > 0
			std::cout << "location path: " << REDCOL << iter->getPath() << std::endl <<
					  "request path: " << _request.getUrl() << std::endl <<
					  "new path: " << url << RESCOL << std::endl;
#endif
			if (iter->getIndex().empty() && !iter->getAutoindex())
			{
				_body = page.makePage("", "Welcome to ", _server.getServName());
				_code.length() > 0 ? "red" :_code = "200";
			}
			else if (iter->getAutoindex() && iter->getIndex().empty() && !isFile)
			{
				Autoindex autoindex;
				if (_request.getUrl() != iter->getPath())
					autoindex.makePage(_server.getRoot() + iter->getRoot() + url, url);
				else
					autoindex.makePage(_server.getRoot() + iter->getRoot(), url);
				_body = autoindex.getPage();
				_code.length() > 0 ? "red" :_code = "200";
			}
			else if (_request.getUrl() == iter->getPath())
			{
				if (!getPage(_server.getRoot() + iter->getRoot() + '/' + iter->getIndex()))
				{
					_code = "404";
					return (false);
				}
			}
			else
				if (!getPage(_server.getRoot() + iter->getRoot() + url + iter->getIndex()))
				{
					_code = "404";
					return (false);
				}
			if (_body.length() > iter->getClientSize())
			{
				_code = "413";
				return (false);
			}
			return (true);
		}
	}
	_code = "403";
	return (false);
}






