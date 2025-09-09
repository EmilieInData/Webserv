/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:51:24 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/09/09 18:09:52 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "HttpRequest.hpp"
#include "ServerManager.hpp"
#include "Utils.hpp"

#define PIPE_READ_END 0
#define PIPE_WRITE_END 1

Response::Response(HttpRequest const &request) : _request(&request)
{
	_clientFd	   = -1;
	_response	   = "";
	_location	   = "";
	_statusCode	   = _request->getStatusCode();
	_method		   = "";
	_contentType   = "";
	_contentLength = "";
	_blockLoc	   = _request->getBlockLoc();
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
	_statusCode = _request->getStatusCode();
	_method		= method;
	if (_statusCode >= 400)
	{
		_location = "/home/fdi-cecc/webserv/www/error_pages/404.html";
		std::cout << RED << "[STATCO]> " << _statusCode << " [LOC]> " << _location << RESET
				  << std::endl; // DBG
		return;
	}
	std::cout << PINK << "FullPath.first(set content): " << fullPath.first << "\n"
			  << "FullPath.second: " << fullPath.second << RESET << std::endl; // TO BORROW
	if (!_blockLoc.getReturnDirective().empty())
		_location = fullPath.first + _blockLoc.getReturnDirective()[1];
	else if (fullPath.second == "/" || fullPath.second.empty())
		_location = fullPath.first + "/index.html";
	else
		_location = fullPath.first + fullPath.second;
	std::cout << RED << "[STATCO]> " << _statusCode << " [LOC]> " << _location << RESET << std::endl;
}

void Response::setClientFd(int clientFd)
{
	_clientFd = clientFd;
}

std::string Response::prepFile()
{
	// Check if it's a binary file (image)
	//check if _location is empty first? TODO

	std::cout << RED << "LOCATION (prepFile): " << _location << RESET << std::endl;

	if (isBinary(_location))
	{
		// std::cout << PINK << "inside BINARY (prepFile): " << RESET << std::endl;
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
		_contentType = "text/html";
		// std::cout << PINK << std::string(__func__) + " cnt type = " + _contentType << RESET << std::endl; // DBG
		DIR *dir = opendir(_location.c_str());
		if (!dir)
			return ""; //check if is enought
		std::string index = _location + "/index.html";
		if (access(index.c_str(), F_OK) == 0)
		{
			_location += "/index.html";
			std::ifstream page(_location.c_str());
			if (!page.is_open())
				return ""; // TODO ERROR MESSAGE?
			std::ostringstream pageContent;
			pageContent << page.rdbuf();
			page.close();
			closedir(dir);
			return pageContent.str();
		}
		return doAutoindex(_request->getFullPath().second, dir);
	}
	else
	{
		// std::cout << PINK << "inside ELSE (prepFile): " << RESET << std::endl; TO BORROW
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
		// std::cout << BLUE << "URI: " << uri << "D_name: " << entry->d_name << RESET
		// 		  << std::endl; // TO BORROW
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

extern const std::map<int, std::string> statusCodeMap;

void Response::prepResponse()
{
	std::string content;
	bool		isErrorPage = (_statusCode >= 400);

	_contentType = _request->getRspType();
	std::cout << PINK << "Content type(prepResponse) : " << _contentType << std::endl; // DBG

	if (_contentType == "cgi-script")
	{
		content		= _request->getServ().getScript().getOutputBody();
		_cgiHeaders = _request->getServ().getScript().getOutputHeaders();
		std::map<std::string, std::string>::const_iterator it = _cgiHeaders.find("Content-Type");
		if (it != _cgiHeaders.end())
			_contentType = it->second;
		else
			_contentType = "text/plain";
	}

	if (isErrorPage && _contentType != "cgi-script")
	{
		const std::map<int, std::string> errorPages = _blockLoc.getErrorPage();

		// Check if the status code exists in the errorPages map
		std::map<int, std::string>::const_iterator errorPageIt = errorPages.find(_statusCode);
		if (errorPageIt != errorPages.end())
		{
			_location = _request->getFullPath().first + "/error_pages/" + errorPageIt->second; // TODO change error page locatio in parsing
			std::cout << RED << "[errorpage found] " << _location << RESET << std::endl; // DBG
		}
		else
		{
			_location = "/home/fdi-cecc/webserv/www/error_pages/error.html";
			std::cout << RED << "[errorpage not found] " << _location << RESET << std::endl; // DBG
		}

		_contentType							= "text/html";
		content									= prepFile();
		std::string				   reasonPhrase = "Unknown Status";
		std::map<int, std::string> statusMap	= getStatusCodeMap();
		if (statusMap.find(_statusCode) != statusMap.end())
			reasonPhrase = statusMap[_statusCode];

		std::stringstream ss;
		ss << _statusCode;

		replaceContent(content, "{{STATUS_CODE}}", ss.str());
		replaceContent(content, "{{REASON_PHRASE}}", reasonPhrase);
	}
	else
		content = prepFile();

	std::ostringstream output;
	output << content.length();
	_contentLength = output.str();

	HeadRsp header(*this);
	_response = header.getHeader() + content;
}

void Response::printRawResponse() // DBG
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

std::map<std::string, std::string> Response::getCgiHeaders() const
{
	return _cgiHeaders;
}

int Response::getStatusCode() const
{
	return _statusCode;
}
