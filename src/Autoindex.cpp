

#include "../inc/Autoindex.hpp"

Autoindex::Autoindex(void)
{

}

Autoindex::~Autoindex(void)
{

}

void Autoindex::getNames(DIR *dir)
{
	dirent *open;
	while ((open = readdir(dir)) != nullptr)
		_names.push_back(open->d_name);
}

bool Autoindex::makePage(const std::string &path, const std::string & locPath)
{
	DIR *dir = opendir(path.c_str());
	if (!dir)
		return (false);
	else
	{
		std::string _locPath(locPath);
		if (_locPath.length() > 1 && locPath[locPath.length() - 1] != '/')
			_locPath += '/';
		getNames(dir);
		_page.append("<!DOCTYPE html>\n");
		_page.append("<html lang=\"en\">\n");
		_page.append("<head>\n");
		_page.append("<meta charset=\"UTF-8\">\n");
		_page.append("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n");
		_page.append("<meta name=\"viewport\" content=\"width=device-width, initial-scale=2.0\">\n");
		_page.append("<title> Index of "  + path + "</title>\n");
		_page.append("</head>\n");
		_page.append("<body bgcolor=\"White\">\n");
		_page.append("<h1> index of "  + path + "</h1>\n");
		for (std::vector<std::string>::iterator it = _names.begin(); it != _names.end(); ++it)
			_page.append("<td><a href=" + _locPath + *it
					+ ">" + *it + "</a></td><h1>\n</h1>");
	}
	closedir(dir);
	return (true);
}

const std::string &Autoindex::getPage(void)
{
	return (_page);
}


