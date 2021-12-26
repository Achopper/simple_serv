
#include "../inc/DefaultPage.hpp"

DefaultPage::DefaultPage()
{

}

DefaultPage::~DefaultPage()
{

}

std::string DefaultPage::makePage(const std::string &errorCode, std::string const & errmsg,
								   std::string const &servVer)
{

	std::string res = "<!DOCTYPE html>\n";
	res.append("<html lang=\"en\">\n");
	res.append("<head>\n");
	res.append("<meta charset=\"UTF-8\">\n");
	res.append("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n");
	res.append("<meta name=\"viewport\" content=\"width=device-width, initial-scale=2.0\">\n");
	res.append("<title>" + errorCode + " " + errmsg + "</title>\n");
	res.append("</head>\n");
	res.append("<body bgcolor=\"PowderBlue\">\n");
	res.append("<center>\n");
	res.append("<h1>" + errorCode + " " + errmsg + "</h1>\n");
	if (errorCode == "405")
		res.append("<h2>" + servVer + " allow only GET POST HEAD DELETE </h1>\n");
	res.append("</center>\n");
	res.append("<hr>\n");
	res.append("<center>" + servVer + "</center>\n");
	res.append("</body>\n");
	res.append("</html>\n");
	return (res);
}
