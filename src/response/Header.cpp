/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:04:28 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/06 11:34:49 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"
#include "Response.hpp"

Header::Header(Response &response) : _response(&response)
{
	setContentType();
	setProtocol();
	setStatusCode();
	setContentLength();
	setConnectionType();
	setCacheControl();
	buildHeader();
}

Header::~Header() {};

void Header::setContentType()
{
	_contentType = "Content-Type: " + _response->checkType() + HEADNL;
}

void Header::setProtocol()
{
	/* TODO needed a way to extract
	protocol from request
	for now this is left as standard */
	_protocol = "HTTP/1.1";
}

void Header::setStatusCode()
{
	/* TODO here we need to find
	a way to get the appropriate
	status code. For now it's
	always 200 */

	_statusCode = "200 OK";
}

void Header::setConnectionType()
{
	/* TODO extract connection type from
	request */
	_connectionType = "Connection: close" + std::string(HEADNL);
}

void Header::setContentLength()
{
	_contentLength =
		"Content-Length: " + intToString(_response->getResponse().length()) + HEADNL;
}

void Header::setCacheControl()
{
	std::string cache;

	if (_contentType == "text/html")
		cache = "public, max-age=3600";
	else if (_contentType == "image/jpg" || _contentType == "image/png" ||
			 _contentType == "image/gif" || _contentType == "text/javascript")
		cache = "public, max-age=300, must-revalidate";
	else
		cache = "no-cache, no-store, must-revalidate";

	_cacheControl = "Cache-Control: " + cache + HEADNL;
}

void Header::buildHeader()
{
	_header = _protocol + " " + _statusCode + HEADNL + _contentType + _contentLength +
			  _connectionType + _cacheControl + HEADNL + HEADNL;
	std::cout << GREEN << "[ HEADER ]\n\n" << _header << std::endl;
}

std::string Header::getHeader()
{
	return _header;
}