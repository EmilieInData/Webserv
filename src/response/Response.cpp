/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:51:24 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/09/15 16:20:21 by esellier         ###   ########.fr       */
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
//	std::cout << "ERROR PAGE :" << _blockLoc.getErrorPage().begin()->second << std::endl;
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
		return;
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
	std::cout << RED << "LOCATION (prepFile): " << _location << RESET << std::endl;

	if (isBinary(_location))
	{
		std::ifstream file(_location.c_str(), std::ios::binary);
		std::ostringstream buffer;
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}
	else if (isFolder(_location))
	{
		_contentType = "text/html";
		DIR *dir = opendir(_location.c_str());
		std::string index = _location + "/index.html";
		if (access(index.c_str(), F_OK) == 0)
		{
			_location += "/index.html";
			std::ifstream page(_location.c_str());
			std::ostringstream pageContent;
			pageContent << page.rdbuf();
			page.close();
			closedir(dir);
			return pageContent.str();
		}
		if (_request->getHttpMethod() == "GET")
			return doAutoindex(_request->getFullPath().second, dir);
		else
		{
			closedir(dir);
			return "";
		}
	}
	else
	{
		std::cout << "DBG prepFile()" << std::endl;
		std::ifstream page(_location.c_str());
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
	html << "        <img src=\"/static/02_cat.png\" alt=\"Autoindex banner\">\n";
	html << "    </div>\n";
	html << "    <div class=\"list-container\">\n";
	html << "    <ul>\n";
}

extern const std::map<int, std::string> statusCodeMap;

void Response::prepResponse( std::pair<int, std::string> incoming )
{
	std::string content;

	_contentType = _request->getRspType();

	if ( this->_statusCode != 200 )
		errorRoutine(content, incoming);
	else if (_contentType == "cgi-script")
	{
		content		= _request->getServ().getScript().getOutputBody();
		_cgiHeaders = _request->getServ().getScript().getOutputHeaders();
		std::map<std::string, std::string>::const_iterator it = _cgiHeaders.find("Content-Type");
		if (it != _cgiHeaders.end())
			_contentType = it->second;
		else
			_contentType = "text/plain";
	}
	else
		content = prepFile();

	if (_statusCode == 301 || _statusCode == 302 ||_statusCode == 307 ||_statusCode == 308)
		return;	
	std::ostringstream output;
	output << content.length();
	_contentLength = output.str();

	HeadRsp header(*this);
	_response = header.getHeader() + content;
	
}

void	Response::errorRoutine(std::string & content, std::pair<int, std::string> incoming) {
	switch ( this->_statusCode )
	{
		case 201:
		{
			std::cout << RED << __func__ << RESET << std::endl; // DBG
			break;
		}
		case 204:
			break;
		case 301:
		case 302:
		case 307:
		case 308:
		{
			std::map<int, std::string> status = getStatusCodeMap(); 
			std::stringstream str;
			str << _statusCode;

			_response = "HTTP/1.1 {{STATUS_CODE}} {{REASON_PHRASE}}\r\n""Location: {{LOCATION}}\r\n""\r\n";
			replaceContent(_response, "{{STATUS_CODE}}", str.str());
			replaceContent(_response, "{{REASON_PHRASE}}", status[_statusCode]);
			replaceContent(_response, "{{LOCATION}}", _request->getUriFirst()  + _blockLoc.getReturnDirective()[1]);
			break;
		}
		default:
		{
			const std::map<int, std::string> errorPages	= _blockLoc.getErrorPage();
			if (!errorPages.empty())
			{
				std::map<int, std::string>::const_iterator errorPageIt = errorPages.find(_statusCode);
				if (errorPageIt != errorPages.end())
					_location = _request->getFullPath().first + errorPageIt->second;
				else
					_location = _request->getFullPath().first + "/error_pages/error.html"; 
			}
			else { 
				ServerData serv = HttpParser::checkIfServerExist(this->_request->getServersList(), incoming);
				std::map<int, std::string> const  errorPages2 = serv.getErrorPage();

				std::map<int, std::string>::const_iterator errorPageIt = errorPages2.find(_statusCode);
				if (errorPageIt != errorPages2.end())
					_location = serv.getRoot() + errorPageIt->second;
				else
					_location = serv.getRoot() + "/error_pages/error.html"; 
		
			}
			std::cout << "LOCATIONNNNNNN: " << _location << std::endl;
			_contentType = "text/html";
			content = prepFile();

			std::cout << "CONTENT: " << content << std::endl;

			std::string reasonPhrase = "Unknown Status";
			std::map<int, std::string> statusMap = getStatusCodeMap(); 
			if (statusMap.find(_statusCode) != statusMap.end())
				reasonPhrase = statusMap[_statusCode];
			
			std::stringstream ss;
			ss << _statusCode;
			replaceContent(content, "{{STATUS_CODE}}", ss.str()); 
			replaceContent(content, "{{REASON_PHRASE}}", reasonPhrase);
			break;
		}
	}
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

void Response::setCookie(const std::string &cookieValue)
{
	_cookieToSet = cookieValue;
}

std::string Response::getCookie() const
{
	return _cookieToSet;
}
