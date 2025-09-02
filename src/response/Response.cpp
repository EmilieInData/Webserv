/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:51:24 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/09/02 11:36:27 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "HttpRequest.hpp"
#include "Utils.hpp"

#define PIPE_READ_END 0
#define PIPE_WRITE_END 1

Response::Response(HttpRequest const &request) : _request(&request)
{

	_clientFd	   = -1;
	_response	   = "";
	_location	   = "";
	_method		   = "";
	_contentType   = "";
	_contentLength = "";
	_autoindex	   = _request->getAutoindex();
}

Response::~Response() {}

Response::Response(Response const &copy)
	: _clientFd(copy._clientFd), _response(copy._response), _location(copy._location),
	  _method(copy._method), _contentLength(copy._contentLength)
{
	_output << copy._output.str();
}

void Response::setResponse(std::string response)
{
	_response = response;
}

void Response::setContent(std::pair<std::string, std::string> fullPath, std::string method)
{
	std::cout << PINK << "FullPath.first: " << fullPath.first << "\n"
			  << "FullPath.second: " << fullPath.second << RESET << std::endl; // TO BORROW
	if (fullPath.second == "/redirect" ||
		fullPath.second == "/redirect/") //TODO check with another code (303) and send good errorpages
		_location = fullPath.first + "/redirect/index.html";
	else if (fullPath.second == "/" || fullPath.second.empty())
		_location = fullPath.first + "/index.html";
	else
		_location = fullPath.first + fullPath.second;
	std::cout << PINK << "_LOcation: " << _location << RESET << std::endl; // TO BORROW
	_method = method;
}

void Response::setClientFd(int clientFd)
{
	_clientFd = clientFd;
}

std::string Response::prepFile()
{
	// Check if it's a binary file (image)
	//check if _location is empty first? TODO

	if (isBinary(_location))
	{
		std::ifstream file(_location.c_str(), std::ios::binary);
		if (!file.is_open())
			return ""; // TODO ERROR MESSAGE?
		std::ostringstream buffer;
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}
	else if (isFolder(_location))
	{
		// if (not exist) //find location bloc, done by cleo in http request
		// 	//error
		DIR *dir = opendir(_location.c_str());
		if (!dir)
			return ""; //return error to open directory
		if (access(_location.c_str(), R_OK) != 0)
		{
			closedir(dir);
			return ""; //return error miss right to read what's inside
		}
		if (this->getAutoindex() == false)
			return ""; //return error not allowed to read inside
		return doAutoindex(_request->getFullPath().second, dir);
	}
	else
	{
		std::ifstream page(_location.c_str());
		if (!page.is_open())
			return ""; // TODO ERROR MESSAGE?
		std::ostringstream pageContent;
		pageContent << page.rdbuf();
		page.close();
		return pageContent.str();
	}
}

std::string Response::doAutoindex(std::string uri, DIR *dir)
{
	std::ostringstream html;
	struct dirent	  *entry;

	doHtmlAutoindex(uri, html);

	while ((entry = readdir(dir)) != NULL)
	{
		if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..") // Ignore "." & ".."
			continue;
		html << "      <li><a href=\"" << uri;
		if (uri[uri.size() - 1] != '/')
			html << "/";
		html << entry->d_name << "\">" << entry->d_name << "</a></li>\n";
		std::cout << BLUE << "URI: " << uri << "D_name: " << entry->d_name << RESET
				  << std::endl; // TO BORROW
	}
	html << "    </ul>\n</div>\n</body>\n</html>\n";
	closedir(dir);
	return html.str();
}

void Response::doHtmlAutoindex(std::string &uri, std::ostringstream &html)
{
	html << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
	html << "    <meta charset=\"UTF-8\">\n";
	html << "    <title>Index of " << uri << "</title>\n";
	html << "    <style>\n";
	html << "        body {\n";
	html << "            background-color: white;\n";
	html << "            font-family: Arial, sans-serif;\n";
	html << "            display: flex;\n";
	html << "            flex-direction: column;\n";
	html << "            justify-content: center;\n";
	html << "            align-items: center;\n";
	html << "            min-height: 100vh;\n";
	html << "            margin: 0;\n";
	html << "            padding: 20px;\n";
	html << "            box-sizing: border-box;\n";
	html << "        }\n";
	html << "        .purple-text {\n";
	html << "            color: #b388eb;\n";
	html << "            font-size: 48px;\n";
	html << "            text-align: center;\n";
	html << "            margin-bottom: 20px;\n";
	html << "        }\n";
	html << "        .image-container {\n";
	html << "            text-align: center;\n";
	html << "            margin-bottom: 20px;\n";
	html << "        }\n";
	html << "        .image-container img {\n";
	html << "            max-width: 100%;\n";
	html << "            height: auto;\n";
	html << "            max-height: 300px;\n";
	html << "        }\n";
	html << "        .list-container {\n";
	html << "            text-align: center;\n";
	html << "        }\n";
	html << "        ul {\n";
	html << "            list-style: none;\n";
	html << "            padding: 0;\n";
	html << "        }\n";
	html << "        li {\n";
	html << "            margin: 8px 0;\n";
	html << "        }\n";
	html << "        a {\n";
	html << "            text-decoration: none;\n";
	html << "            color: #7b2cbf;\n";
	html << "            font-size: 20px;\n";
	html << "        }\n";
	html << "        a:hover {\n";
	html << "            text-decoration: underline;\n";
	html << "        }\n";
	html << "    </style>\n";
	html << "</head>\n<body>\n";
	html << "    <div class=\"purple-text\">Index of " << uri << "</div>\n";
	html << "    <div class=\"image-container\">\n";
	html << "        <img src=\"/static/cat.png\" alt=\"Autoindex banner\">\n";
	html << "    </div>\n";
	html << "    <div class=\"list-container\">\n";
	html << "    <ul>\n";
}

void Response::prepResponse()
{
	std::string content;
	_contentType = _request->getRspType();

	content = prepFile();

	std::ostringstream output;
	output << content.length();
	_contentLength = output.str();

	HeadRsp header(*this);
	_response = header.getHeader() + content;
}

void Response::printRawResponse()
{
	std::cout << PINK << "[RAW RESPONSE]" << RESET << std::endl;
	for (size_t i = 0; i < _response.size(); i++)
	{
		if (_response[i] == '\r')
			std::cout << "\\r";
		else if (_response[i] == '\n')
			std::cout << "\\n\n";
		else
			std::cout << _response[i];
	}
	std::cout << PINK << "\n[END OF RESPONSE]" << RESET << std::endl;
}

void Response::sendResponse()
{
	prepResponse();

	size_t		totalSent	= 0;
	size_t		totalSize	= _response.size();
	const char *data		= _response.c_str();
	int			attempts	= 0;
	const int	maxAttempts = 100;

	while (totalSent < totalSize && attempts < maxAttempts)
	{

		ssize_t sent = send(_clientFd, data + totalSent, totalSize - totalSent, 0);
		if (sent > 0)
			totalSent += sent;
		else if (sent == 0)
		{
			std::cerr << "Connection closed by client during send" << std::endl;
			break;
		}
		else
		{
			usleep(1000);
			attempts++;
		}
	}

	if (totalSent != totalSize)
		printBoxError("Warning, send incomplete");
}

std::string Response::getType()
{
	return _contentType;
}

std::string Response::getLength()
{
	return _contentLength;
}

std::string Response::getResponse()
{
	return _response;
}

bool Response::getAutoindex() const
{
	return _autoindex;
}
