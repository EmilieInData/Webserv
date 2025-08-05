/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:51:24 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/29 11:45:09by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() {}

Response::~Response() {}

void Response::setResponse(std::string response)
{
	_response = response;
}

void Response::setContent(std::pair<std::string, std::string> fullPath, std::string method)
{
	if (fullPath.second == "/" || fullPath.second.empty())
		_location = fullPath.first + "/index.html"; // TODO check AutoIndex
	else
		_location = fullPath.first + fullPath.second;
	
	_method = method;
}

void Response::setClientFd(int clientFd)
{
	_clientFd = clientFd;
}

std::string Response::prepFile()
{
	// Check if it's a binary file (image)

	if (isBinary(_location))
	{
		std::ifstream file(_location.c_str(), std::ios::binary);
		if (!file.is_open())
			return "";

		std::ostringstream buffer;
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}
	else
	{
		std::ifstream page(_location.c_str());
		if (!page.is_open())
			return "";

		std::ostringstream pageContent;
		pageContent << page.rdbuf();
		page.close();
		return pageContent.str();
	}
}

std::string Response::checkType()
{
	std::string extension;
	size_t dotPos = _location.find_last_of('.');
	if (dotPos != std::string::npos)
		extension = _location.substr(dotPos);

	if (extension == ".html" || extension == ".htm")
		return "text/html";
	else if (extension == ".jpg" || extension == ".jpeg")
		return "image/jpeg";
	else if (extension == ".png")
		return "image/png";
	else if (extension == ".gif")
		return "image/gif";
	else if (extension == ".css")
		return "text/css";
	else if (extension == ".js")
		return "text/javascript";
	else
		return "application/octet-stream";
}

void Response::prepResponse()
{
	std::string content = prepFile();
	std::ostringstream output;
	output << content.length();
	std::string contentLength = output.str();
	std::string contentType = checkType();

	/* TODO the response here is hardcoded
	and for now it's just for testing */
	_response =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: " +
		contentType + "\r\n"
					  "Content-Length: " +
		contentLength + "\r\n"
						"Connection: close\r\n"
						"Cache-Control: no-cache\r\n"
						"\r\n" +
		content;
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

	size_t totalSent = 0;
	size_t totalSize = _response.size();
	const char *data = _response.c_str();
	int attempts = 0;
	const int maxAttempts = 100;

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

std::string Response::getResponse()
{
	return _response;
}
