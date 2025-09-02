/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadRsp.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:04:28 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/27 12:31:30 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeadRsp.hpp"
#include "Response.hpp"

HeadRsp::HeadRsp(Response &response) : _response(&response)
{
	setContentType();
	setProtocol();
	setStatusCode();
	setContentLength();
	setConnectionType();
	setCacheControl();
	buildHeader();
}

HeadRsp::~HeadRsp() {};

void HeadRsp::setContentType()
{
	_contentType = "Content-Type: " + _response->getType() + HEADNL;
}

void HeadRsp::setProtocol()
{
	/* TODO needed a way to extract
	protocol from request
	for now this is left as standard */
	_protocol = "HTTP/1.1";
}

void HeadRsp::setStatusCode()
{
	/* TODO here we need to find
	a way to get the appropriate
	status code. For now it's
	always 200 */

	_statusCode = "200 OK";
}

void HeadRsp::setConnectionType()
{
	/* TODO extract connection type from
	request */
	_connectionType = "Connection: close" + std::string(HEADNL);
}

void HeadRsp::setContentLength()
{
	_contentLength = "Content-Length: " + _response->getLength() + HEADNL;
}

void HeadRsp::setCacheControl()
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

void HeadRsp::buildHeader()
{
	_header = _protocol + " " + _statusCode + HEADNL + _contentType + _contentLength +
			  _connectionType + _cacheControl + HEADNL;
	// std::cout << GREEN << "[ HEADER ]\n\n" << _header << std::endl;
}

std::string HeadRsp::getHeader()
{
	return _header;
}