#include "../inc/Location.hpp"


Location::Location() :
_clientMaxBodySize(DEF_CLI_MAX_BDY_SZ),
_index(""),
_autoindex(false)
{
	_methods["GET"] = true;
	_methods["POST"] = false;
	_methods["DELETE"] = false;
}

Location::Location(const Location &obj)
{
	*this = obj;
}

Location::~Location()
{
}

Location &Location::operator=(const Location &obj)
{
	if (this != &obj)
	{
		_path = obj._path;
		_methods = obj._methods;
		_clientMaxBodySize = obj._clientMaxBodySize;
		_index = obj._index;
		_root = obj._root;
		_autoindex = obj._autoindex;
	}
	return (*this);
}

bool Location::setPath(const std::string &path)
{
	if (path[0] != '/' || path[path.length() - 1] != '/')
	{
		std::cout << REDCOL"Location block must start and end with '/'" << RESCOL << std::endl;
		return (false);
	}
	path.length() > 1 ? _path = path.substr(0, path.length() - 1) : _path = path;
	return (true);
}

bool Location::setAutoindex(std::string const & autoindex)
{
	if (autoindex =="on")
	{
		_autoindex = true;
		return (true);
	}
	else if (autoindex == "off")
		return true;
	else
		return false;
}

bool Location::setRoot(const std::string &root)
{
	if (root[0] != '/')
	{
		std::cout << REDCOL"Root path must start with '/'" << RESCOL <<  std::endl;
		return (false);
	}
	_root = root;
	return (true);
}

bool Location::setIndex(const std::string &index)
{
	_index = index;
	return (true);
}

bool Location::setClientBodySize(const std::string &num)
{
	for (std::string::const_iterator it = num.begin(); it != num.end(); ++it)
		if (!std::isdigit(*it))
			return false;
	_clientMaxBodySize = static_cast<size_t>(std::atoi(num.c_str()));
	return (true);
}

bool Location::setMethods(const std::string &methods)
{
	std::vector<std::string> mt;
	for (std::string::size_type start = 0, end = 0; end != methods.length();)
	{
		end = methods.find('_', start);
		if (end == std::string::npos)
		{
			mt.push_back(methods.substr(start, methods.length()));
			break;
		}
		mt.push_back(methods.substr(start, end - start));
		start = end + 1;
	}
	for (std::vector<std::string>::iterator it = mt.begin(); it != mt.end(); ++it)
	{
		if (!_methods.count(*it))
			return (false);
		_methods[*it] = true;
	}
	return (true);
}

bool Location::getAutoindex() const
{
	return _autoindex;
}

std::string Location::getPath() const
{
	return (_path);
}

std::string Location::getRoot() const
{
	return (_root);
}

std::string Location::getIndex() const
{
	return (_index);
}

size_t Location::getClientSize() const
{
	return (_clientMaxBodySize);
}

std::map<std::string, bool> Location::getMethods(void) const
{
	return (_methods);
}




