

#include "../inc/Core.hpp"
// #include "../inc/Config.hpp"


Config::Config(const std::string &conf)
{
	readConfig(conf);
	parseConfig();
}

Config::Config(const Config &obj)
{
	*this = obj;
}

Config& Config::operator=(const Config &obj)
{
	if (this != &obj)
	{
		_conf = obj._conf;
		_err = obj._err;
		_servers = obj._servers;
		_servCount = obj._servCount;
	}
	return (*this);
}

Config::~Config(void)
{}

std::string Config::readConfig(const std::string &conf)
{
	std::ifstream file(conf);
	std::string res;

	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
			res += line + "\n";
	}else
	{
		std::cout << REDCOL"Can't open config file" << RESCOL << std::endl;
		exit(EXIT_FAILURE);
	}
	file.close();
	preParse(res);
	return (_conf);
}

bool delimetr(char c)
{
	return (std::isspace(c) || c == '\n');
}

bool noDelimetr(char c)
{
	return !(delimetr(c));
}

std::vector<std::string> Config::splitConf()
{
	std::vector<std::string> res;
	std::string tok;
	std::string::iterator start = _conf.begin(), end;

	while (start != _conf.end())
	{
		start = std::find_if(start, _conf.end(), noDelimetr);
		end = std::find_if(start, _conf.end(), delimetr);
		tok = std::string(start, end);
		start = end;
		if (tok.length() > 0)
			res.push_back(tok);
	}
	return (res);
}

std::vector<std::string> Config::preParse(std::string const &toParse)
{
	std::vector<std::string> res;
	char commStart = '#';
	char commEnd = '\n';
	std::string::size_type start = 0, end;

	while(start != toParse.length())
	{
		end = toParse.find(commStart, start);
		if (end != std::string::npos)
		{
			if (start != end)
				_conf += toParse.substr(start, end - start);
			end = toParse.find(commEnd, end);
			if (end == std::string::npos)
				break;
		}
		else
		{
			end = toParse.length();
			_conf += toParse.substr(start, end - start);
			break;
		}
		start = end;
	}
	res = splitConf();
	return (res);
}

void Config::parseConfig()
{
	std::vector<std::string> splittedConf = splitConf();

	if (splittedConf.empty())
	{
		_err.append("It's as if you betrayed an empty config in your arguments. You don't have to do that, please.");
		return ;
	}
	for (std::vector<std::string>::iterator word = splittedConf.begin(); word != splittedConf.end();)
	{
		if (*word == "server")
		{
			if (!parseServerBlock(splittedConf, ++word))
				return;
		}
		//else if()
		else
		{
			_err.append("Unknown config parametr: " + *word);
			return ;
		}
//		*++word;
	}

}

bool Config::checkSemicolon(std::vector<std::string>::iterator &word)
{
	std::string::size_type semicol;

	semicol = word->find(";", 0);
	if (semicol != std::string::npos && word->length()  == semicol + 1)
		return (true);
	_err.append(REDCOL"Expecting \";\" after " + *word + "\n" RESCOL);
	return (false);

}

bool Config::parseServerBlock(std::vector<std::string> &conf, std::vector<std::string>::iterator &word)
{
	Server server;
	std::string::size_type pos;

	if (*word != "{")
	{
		_err.append(REDCOL"Error in config. Expected \"{\" after \"server\"\n" RESCOL);
		return (false);
	}
	*++word;
	for (; word != conf.end(); )
	{
		if (*word == "listen")
		{
			pos = (++word)->find(':', 0);
			if (pos == std::string::npos || !checkSemicolon(word) ||
				!(server.setAddr(std::string(std::begin(*word), word->end() - 1), pos)))
				_err.append(REDCOL"Wrong host adress: " + *word + "\n" RESCOL);
		}
		else if (*word == "location")
			parseLocationBlock(conf, ++word, server);
		else if (*word == "server_name")
		{
			if (!checkSemicolon(++word)
			|| !server.setServName((word)->substr(0, word->length() - 1)))
				word++;
		}
		else if (*word == "root")
		{
			if (!checkSemicolon(++word) ||
			!server.setRoot((word)->substr(0, word->length() - 1)))
				_err.append(REDCOL"Wrong root adress \n" RESCOL);
		}
		else if (*word == "error_page")
		{
			std::string code = *++word;
			if (!checkSemicolon(++word) ||
				!server.setErrorPage((word)->substr(0, word->length() - 1), code, _err))
				_err.append(REDCOL"Wrong errorPage adress \n" RESCOL);
		}
		else if (*word == "}")
		{
			*++word;
			break;
		}
		else
		{
			_err.append(REDCOL"Unknown config parametr in server block: " + *word + "\n" RESCOL);
			word++;
		}
		*++word;
	}
	if (_err.length() > 0)
		return (false);
	if (!checkUnicLocation(server))
	{
		_err.append(REDCOL"Location path must be unique\n" RESCOL);
		return (false);
	}
	setServer(server);
	std::cout << "http://"  << server.getServIp() << ":" << server.getPort() << std::endl;
	_servCount++;
	return (true);
}

bool Config::parseLocationBlock(std::vector<std::string> &conf, std::vector<std::string>::iterator &word, Server & server)
{
	Location location;

	if (!location.setPath(*word++) || *word != "{")
		_err.append(REDCOL"Error in config. Expected \"{\" after \"location path\"\n" RESCOL);
	word++;
	for (; word != conf.end();)
	{
		if (*word == "root")
		{
			if(!checkSemicolon(++word) || !location.setRoot((word)->substr(0, (word)->length() - 1)))
				_err.append(REDCOL"Wrong root directive in location block\n" RESCOL);
		}
		else if (*word == "index")
		{
			if (!checkSemicolon(++word) || !location.setIndex((word)->substr(0, word->length() - 1)))
				_err.append( REDCOL"Wrong index directive\n" RESCOL);
		}
		else if (*word == "client_max_body_size")
		{
			if (!checkSemicolon(++word) || !location.setClientBodySize(word->substr(0, word->length() - 1)))
				_err.append(REDCOL"Wrong client size\n" RESCOL);
		}
		else if (*word == "autoindex")
		{
			if (!checkSemicolon(++word) || !location.setAutoindex(word->substr(0, word->length() - 1)))
				_err.append(REDCOL"Wrong autoindex, must be on || off\n" RESCOL);
		}
		else if (*word == "methods")
		{
			if (!checkSemicolon(++word) || !location.setMethods(word->substr(0, word->length() - 1)))
				_err.append(REDCOL"Wrong method list\n" RESCOL);
		}
//		else if (*word == "return")
//		{
//
//		}
		else if (*word == "}")
			break;
		else
			_err.append(REDCOL"Unknown config parametr in location block: " + *word + "\n" RESCOL);
		*++word;
	}
	if (_err.length() > 0 )
		return (false);
	server.setLocList(location);
	return (true);
}

bool Config::checkUnicLocation(Server const & server) const
{
	std::vector<Location> tmp = server.getLocList();
	for (std::vector<Location>::iterator it = tmp.begin(); it != tmp.end(); ++it)
	{
		for (std::vector<Location>::iterator jt = it + 1; jt != tmp.end(); ++jt)
			if (it->getPath() == jt->getPath())
				return (false);
	}
	return (true);
}

std::string Config::getErrors() const
{
	return (this->_err);
}

std::vector<Server> Config::getServers() const
{
	return (this->_servers);
}

std::string Config::getConf() const
{
	return (_conf);
}

uint32_t Config::getServCount() const
{
	return _servCount;
}

void Config::setServer(Server &server)
{
	_servers.push_back(server);
}
