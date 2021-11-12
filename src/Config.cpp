

#include "../inc/Config.hpp"


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
	_err.append("Expecting \";\" after " + *word);
	return (false);

}

bool Config::parseServerBlock(std::vector<std::string> &conf, std::vector<std::string>::iterator &word)
{
	Server server;
	std::string::size_type pos;

	if (*word != "{")
	{
		_err.append("Error in config. Expected \"{\" after \"server\"");
		return (false);
	}
	*++word;
	for (; word != conf.end(); )
	{
		if (*word == "listen")
		{
			pos = (++word)->find(':', 0);
			if (pos == std::string::npos)
			{
				_err.append("Wrong host adress: " + *word);
				return (false);
			}
			if (!checkSemicolon(word))
				return (false);
			if (!(server.setAddr(std::string(std::begin(*word), word->end() - 1), pos)))
			{
				_err.append(REDCOL"Wrong host adress: " + *word + RESCOL);
				return (false);
			}
		}
		else if (*word == "server_name")
		{
			if (!checkSemicolon(++word))
				return (false);
			server.setServName((word)->substr(0, word->length() - 1));
		}
		else if (*word == "root")
		{
			if (!checkSemicolon(++word))
				return (false);
			server.setRoot((word)->substr(0, word->length() - 1));
		}
		else if (*word == "}")
		{
			*++word;
			break;
		}
		else
		{
			_err.append("Unknown config parametr in server block: " + *word);
			return (false);
		}
		*++word;
	}
	setServer(server);
	_servCount++;
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
