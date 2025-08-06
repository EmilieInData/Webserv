/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:04:30 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/06 11:35:50 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
#define HEADER_HPP

#include "Utils.hpp"

#define HEADNL "\r\n"

/* TODO building a basic header class
to compose the right one to put at
the beginning of the response*/

class Response;

class Header
{
private:
	Response 	*_response;
	std::string _header;
	std::string _statusCode;
	std::string _contentType;
	std::string _protocol;
	std::string _contentLength;
	std::string _connectionType;
	std::string _cacheControl;

	Header &operator=(Header const &copy);
	Header();
	Header(Header const &copy);

public:
	Header(Response &response);
	~Header();

	void		setContentType();  //
	void		setProtocol();
	void		setStatusCode();
	void		setContentLength();
	void		setConnectionType();
	void		setCacheControl();
	void		buildHeader();
	std::string getHeader();

	/* TODO do we need getters
	for single elements when we mostly
	build it inside the class? */
};

#endif

/* needed in header
http protocol
status code
content type
content length
connection type
cache control

+ content
*/
/*
REFERENCE:

_response =
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: " +
	contentType +
	"\r\n"
	"Content-Length: " +
	contentLength +
	"\r\n"
	"Connection: close\r\n"
	"Cache-Control: no-cache\r\n"
	"\r\n" +
	content;
*/