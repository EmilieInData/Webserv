/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:51:24 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/26 13:08:49 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() {}

Response::~Response() {}

void Response::setResponse(std::string response)
{
	_response = response;
}

void Response::setContent(std::string content)
{
	_location = content;
}

void Response::setClientFd(int clientFd)
{
	_clientFd = clientFd;
}

std::string Response::prepFile()
{
	std::ifstream page(_location.c_str());

	if (!page.is_open())
		return NULL;
	
	std::ostringstream pageContent;
	pageContent << page.rdbuf();
	page.close();
	return pageContent.str();
}

void Response::prepResponse()
{
	std::string content = prepFile();
	std::ostringstream output;
	output << content.length();
	std::string content_length = output.str();
	
	_response = 
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html\r\n"
	"Content-Length: " + content_length + "\r\n"
	"Connection: close\r\n"
	"Cache-Control: no-cache\r\n"
	"\r\n" + content;
}

void Response::sendResponse()
{
	prepResponse();
	std::cout << "Sending response (" << _response.size() << " bytes):" << std::endl;
	std::cout << "[RAW RESPONSE]" << std::endl;
	for (size_t i = 0; i < _response.size(); i++) {
		if (_response[i] == '\r') std::cout << "\\r";
		else if (_response[i] == '\n') std::cout << "\\n\n";
		else std::cout << _response[i];
	}
	std::cout << "\n[END RAW]" << std::endl;

	ssize_t sent = send(_clientFd, _response.c_str(), _response.size(), 0);
	std::cout << "Bytes sent: " << sent << " of " << _response.size() << std::endl;
	if (sent < 0)
		std::cerr << "Send failed" << std::endl;
}
