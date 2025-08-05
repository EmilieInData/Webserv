/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:04:28 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/05 17:29:08 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

Header::Header(Response response) : _response(response) {};

Header::~Header() {};

void Header::setContentType()
{
	_contentType = _response.checkType();
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

	_statusCode = "200";
}

void Header::setConnectionType()
{
	/* TODO extract connection type from
	request */
	_connectionType = "close";
}

void Header::setContentLength()
{
	_contentLength = _response.getResponse().length();
}

void Header::setCacheControl()
{
	if (_contentType == "text/html")
		_cacheControl = "public, max-age=3600";
	else if (_contentType == "image/jpg" || _contentType == "image/png" ||
			 _contentType == "image/gif" || _contentType == "text/javascript")
		_cacheControl = "public, max-age=300, must-revalidate";
	else
		_cacheControl = "no-cache, no-store, must-revalidate";
}
