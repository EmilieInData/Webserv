/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:49:32 by esellier          #+#    #+#             */
/*   Updated: 2025/09/11 19:08:08 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

bool isInt(std::string const &value)
{
	int num;

	for (size_t i = 0; i < value.length(); i++)
	{
		if (value[i] > '9' || value[i] < '0' || i > 5)
			return false;
	}
	num = strToInt(value);
	if (num < 0 || num > 65535)
		throw std::invalid_argument(" Parsing error, 'listen' port number is"
									" not correct, only int between 0 & 65535 accepted\n");
	return true;
}

bool isLocal(std::string const &value)
{
	int num;

	if (value.length() < 11 || value.substr(0, 10) != "localhost:")
		return false;
	for (size_t i = 10; i < value.length(); i++)
	{
		if (value[i] > '9' || value[i] < '0')
			return false;
	}
	num = strToInt(value.substr(10));
	if (num < 0 || num > 65535)
		throw std::invalid_argument(" Parsing error, 'listen' local port number is"
									" not correct, only int between 0 & 65535 accepted\n");
	return true;
}

bool isSocket(std::string const &value)
{
	int flag = 0;

	for (size_t i = 0; i < value.length(); i++)
	{
		if ((value[i] > '9' || value[i] < '0') && value[i] != '.' && value[i] != ':')
			return false;
		if (value[i] == '.')
		{
			if (!value[i + 1] || value[i + 1] == '.' || value[i + 1] == ':' || i == 0)
				throw std::invalid_argument(" Parsing error, 'listen' socket address is not correct\n");
			flag++;
		}
		if (value[i] == ':')
		{
			if (!value[i + 1] || value[i + 1] == '.' || value[i + 1] == ':' || flag != 3)
				throw std::invalid_argument(" Parsing error, 'listen' socket"
											" address is not correct\n");
		}
	}
	return true;
}

int strToInt(std::string const &value)
{
	int				   num;
	std::istringstream str(value);

	str >> num;
	if (str.fail())
		throw std::runtime_error(" error to convert the string argument in int");
	return num;
}

bool checkSocketAddress(std::string const &value)
{
	int						 tmp;
	size_t					 j = 0;
	std::vector<std::string> nums;

	for (size_t i = 0; i < value.length(); i++)
	{
		if (value[i] > '9' || value[i] < '0')
		{
			nums.push_back(value.substr(j, i - j));
			j = i + 1;
		}
	}
	nums.push_back(value.substr(j));
	for (size_t i = 0; i < nums.size(); i++)
	{
		if (i > 4)
			return false;
		if (nums[i].length() > 5)
			return false;
		tmp = strToInt(nums[i]);
		if (i < 4 && (tmp < 0 || tmp > 255))
			return false;
		if (i == 4 && (tmp < 0 || tmp > 65535))
			return false;
	}
	return true;
}

std::string socketToIp(std::string const &value)
{
	std::string str = "";

	for (size_t i = 0; i < value.length(); i++)
	{
		if (value[i] == ':')
		{
			str = value.substr(0, i);
			break;
		}
	}
	if (str == "")
		throw std::invalid_argument(" Parsing error, 'listen' socket address"
									" is not correct\n");
	return str;
}

int socketToPort(std::string const &value)
{
	std::string str = "";

	for (size_t i = 0; i < value.length(); i++)
	{
		if (value[i] == ':')
		{
			str = value.substr(i + 1);
			break;
		}
	}
	if (str == "")
		throw std::invalid_argument(" Parsing error, 'listen' socket address"
									" is not correct\n");
	int num = strToInt(str);
	return num;
}

bool checkDns(std::vector<std::string> &tmp)
{
	for (size_t i = 0; i < tmp.size(); i++)
	{
		for (size_t j = 0; j < tmp[i].length(); j++)
		{
			if ((tmp[i][j] >= '0' && tmp[i][j] <= '9') || (tmp[i][j] >= 'a' && tmp[i][j] <= 'z') || (tmp[i][j] >= 'A' && tmp[i][j] <= 'Z') || tmp[i][j] == '.' || tmp[i][j] == '-')
				continue;
			return false;
		}
		if (!checkLabel(tmp[i]))
			return false;
	}
	return true;
}

bool checkLabel(std::string const &str)
{
	std::vector<std::string> tmp;

	if (str.length() > 255)
		throw std::invalid_argument(" Parsing error, 'server_name' arguments"
									" may not exceed 255 characters\n");
	int j = 0;
	for (size_t i = 0; i < str.length(); i++) //cutted in label in a vector
	{
		if (str[i] == '.')
		{
			if (i == 0 || i == str.length() - 1)
				throw std::invalid_argument(" Parsing error, 'server_name' arguments"
											" cannot begin or end by '.'\n");
			if (str[i + 1] && str[i + 1] == '.')
				throw std::invalid_argument(" Parsing error, 'server_name' arguments"
											" cannot have two consecutive '.'\n");
			tmp.push_back(str.substr(j, i - j));
			j = i + 1;
		}
	}
	if (tmp.empty())
		j = 0;
	tmp.push_back(str.substr(j));
	for (size_t i = 0; i < tmp.size(); i++) //checking labels
	{
		if (tmp[i].length() > 63)
			throw std::invalid_argument(" Parsing error, 'server_name' argument's"
										" labels may not exceed 63 characters\n");
		if (tmp[i][0] == '-' || tmp[i][tmp[i].length() - 1] == '-')
			throw std::invalid_argument(" Parsing error, 'server_name' argument's"
										" labels cannot begin or end by '-'\n");
	}
	return true;
}

unsigned int strToSize(std::string const &value)
{
	std::string tmp;

	for (size_t i = 0; i < value.length(); i++)
	{
		if (value[i] < '0' || value[i] > '9')
		{
			tmp = value.substr(0, i);
			if (i + 1 != value.length())
				throw std::invalid_argument(" Parsing error, wrong client_max_body_size' arguments\n");
		}
	}
	unsigned int num = strToInt(tmp);
	if (value[value.length() - 1] == 'k' || value[value.length() - 1] == 'K')
		num = num * 1024;
	else if (value[value.length() - 1] == 'm' || value[value.length() - 1] == 'M')
		num = num * 1024 * 1024;
	else
		throw std::invalid_argument(" Parsing error, wrong client_max_body_size' arguments\n");
	return (num);
}

std::map<int, std::pair<std::string, std::string> > defaultErrorPages()
{
	std::map<int, std::pair<std::string, std::string> > map;

	map[400] = std::make_pair("400 - Bad Request", "/4xx.html");
	map[401] = std::make_pair("401 - Unauthorized", "/4xx.html");
	map[403] = std::make_pair("403 - Forbidden", "/4xx.html");
	map[404] = std::make_pair("404 - Not Found", "/404.html");
	map[405] = std::make_pair("405 - Method Not Allowed", "/4xx.html");
	map[410] = std::make_pair("410 - Gone", "/4xx.html");
	map[413] = std::make_pair("413 - Request Entity Too Large", "/4xx.html");
	map[414] = std::make_pair("414 - URI Too Long", "/4xx.html");
	map[500] = std::make_pair("500 - Internal Server Error", "/50x.html");
	map[501] = std::make_pair("501 - Not Implemented", "/50x.html");
	return map;
}

bool isErrorPage(std::string const &value)
{
	if (value == "400" || value == "401" || value == "403" || value == "404" || value == "405" || value == "410" || value == "413" || value == "414" || value == "500" || value == "501")
		return true;
	return false;
}

// bool isHtmlAddress(std::string const &value)
// {
// 	if (value == "/404.html" || value == "/50x.html" || value == "/4xx.html")
// 		return true;
// 	return false;
// }

//pas d’espaces ou caractères interdits.

bool isHtmlAddress(std::string const &value)
{
	if (value[0] != '/')
		return false;
	for (size_t i = 0; i < value.length(); i++)
	{
		if (value[i] != '_' && value[i] != '-' && value[i] != '.' && value[i] != '/' && !isalnum(value[i]))
			return false;
	}
	return true;
}

// void checkErrorPage(std::map<int, std::string> const &value)
// {
// 	std::map<int, std::string>::const_iterator it;

// 	for (it = value.begin(); it != value.end(); it++)
// 	{
// 		if ((it->first == 404 && it->second != "/404.html") ||
// 			((it->first == 500 || it->first == 501) && it->second != "/50x.html") ||
// 			((it->first == 400 || it->first == 401 || it->first == 403 || it->first == 405 ||
// 			  it->first == 410 || it->first == 413 || it->first == 414) &&
// 			 it->second != "/4xx.html"))
// 			throw std::invalid_argument(" Parsing error, 'error_page' number &"
// 										" html adress don't match\n");
// 	}
// 	return;
// }

void setupSignal()
{
	signal(SIGINT, handleSignal);
}

void handleSignal(int signum)
{
	if (signum == SIGINT)
		std::cout << "\r\033[K" << std::flush;
	return;
}

const std::string timeStamp()
{
	std::string timeStamp;
	char		buffer[64];

	struct timeval tv;
	gettimeofday(&tv, NULL);

	struct tm *timeData = localtime(&tv.tv_sec);
	strftime(buffer, sizeof(buffer), "[%H:%M:%S", timeData);

	int milliseconds = tv.tv_usec / 1000;
	int microseconds = tv.tv_usec % 1000;

	std::ostringstream milSec;
	milSec << buffer << "." << std::setfill('0') << std::setw(3) << milliseconds << "." << std::setfill('0') << std::setw(3) << microseconds << "] ";

	timeStamp = "\033[35m" + milSec.str() + "\033[0m";
	return timeStamp;
}

size_t visLen(const std::string &str)
{
	size_t visLen  = 0;
	bool   escCode = false;

	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == '\033')
			escCode = true;
		else if (escCode == true && str[i] == 'm')
			escCode = false;
		else if (escCode == false)
			visLen++;
	}

	return visLen;
}

std::string intToString(const int &num)
{
	std::ostringstream str;
	str << num;
	return str.str();
}

bool isBinary(std::string location)
{
	std::string extension;
	size_t		dotPos = location.find_last_of('.');
	if (dotPos != std::string::npos)
		extension = location.substr(dotPos);

	return (extension == ".jpg" || extension == ".jpeg" || extension == ".png" || extension == ".gif" || extension == ".ico");
}

// bool isFolder(std::string location)
// {
// 	char c = location[location.size() -1];
// 	if (c == '/')
// 		return true;
// 	return false;
// }

bool isFolder(std::string location)
{
	struct stat statbuf;
	if (stat(location.c_str(), &statbuf) != 0)
	{
		return false;
	}
	return S_ISDIR(statbuf.st_mode);
}
// vector<std::string>	HttpParser::split( std::string & const str, std::string & const delimiter ) {}

void deleteArray(char **array)
{
	for (size_t i = 0; array[i] != NULL; i++)
		delete[] array[i];
	delete[] array;
}

const std::string getHttpDate()
{
	char	   buffer[100];
	time_t	   now		= time(0);
	struct tm *gmt_time = gmtime(&now);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt_time);
	return std::string(buffer);
}

std::string upperKey(std::string const &key)
{
	std::string normalized = key;
	bool		capitalize = true;
	for (size_t i = 0; i < normalized.length(); ++i)
	{
		if (capitalize && std::isalpha(normalized[i]))
		{
			normalized[i] = std::toupper(normalized[i]);
			capitalize	  = false;
		}
		else if (normalized[i] == '-')
		{
			capitalize = true;
		}
		else
		{
			normalized[i] = std::tolower(normalized[i]);
		}
	}
	return normalized;
}

void replaceContent(std::string &htmlContent, const std::string &target, const std::string &newContent)
{
	if (target.empty())
		return;
	size_t startPos = 0;
	while ((startPos = htmlContent.find(target, startPos)) != std::string::npos)
	{
		htmlContent.replace(startPos, target.length(), newContent);
		startPos += newContent.length(); // Move past the replacement
	}
}

std::map<int, std::string> getStatusCodeMap()
{
	std::map<int, std::string> m;
	m[100] = "Continue";
	m[101] = "Switching Protocols";
	m[200] = "OK";
	m[201] = "Created";
	m[202] = "Accepted";
	m[204] = "No Content";
	m[301] = "Moved Permanently";
	m[302] = "Found";
	m[304] = "Not Modified";
	m[400] = "Bad Request";
	m[401] = "Unauthorized";
	m[403] = "Forbidden";
	m[404] = "Not Found";
	m[405] = "Method Not Allowed";
	m[500] = "Internal Server Error";
	m[501] = "Not Implemented";
	m[502] = "Bad Gateway";
	m[503] = "Service Unavailable";
	return m;
}

std::string generateCookieId()
{
	std::stringstream cookieId;
	cookieId << std::time(NULL);
	cookieId << rand();

	return cookieId.str();
}

std::string getQueryValue(const std::string &query, const std::string &key)
{
	if (query.empty())
		return "";

	std::string keyToFind = key + "=";
	size_t		pos		  = query.find(keyToFind);

	while (pos != std::string::npos)
	{
		if (pos == 0 || query[pos - 1] == '&')
		{
			size_t valueStart = pos + keyToFind.length();
			size_t valueEnd	  = query.find('&', valueStart);

			if (valueEnd == std::string::npos)
				return query.substr(valueStart);
			else
				return query.substr(valueStart, valueEnd - valueStart);
		}
		pos = query.find(keyToFind, pos + 1);
	}

	return "";
}

std::string getCookieValue(const std::string &cookie, const std::string &key)
{
	if (cookie.empty())
		return "";

	std::string keyToFind = key + "=";
	size_t		pos		  = cookie.find(keyToFind);

	while (pos != std::string::npos)
	{
		if (pos == 0 || (cookie[pos - 1] == ' ' && cookie[pos - 2] == ';'))
		{
			size_t valueStart = pos + keyToFind.length();
			size_t valueEnd	  = cookie.find(';', valueStart);

			if (valueEnd == std::string::npos)
				return cookie.substr(valueStart);
			else
				return cookie.substr(valueStart, valueEnd - valueStart);
		}
		pos = cookie.find(keyToFind, pos + 1);
	}
	return "";
}