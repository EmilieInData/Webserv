/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:51:24 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/26 12:38:11 by fdi-cecc         ###   ########.fr       */
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
	_content = content;
}

void Response::setClientFd(int clientFd)
{
	_clientFd = clientFd;
}

void Response::prepResponse()
{
	std::ostringstream output;
	output << _content.length();
	std::string content_length = output.str();
	
	_response = 
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html\r\n"
	"Content-Length: " + content_length + "\r\n"
	"Connection: close\r\n"
	"Cache-Control: no-cache\r\n"
	"\r\n" + _content;
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
