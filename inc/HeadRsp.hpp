/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadRsp.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:04:30 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/09/12 13:52:25 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADRSP_HPP
#define HEADRSP_HPP

#include "Utils.hpp"

#define HEADNL "\r\n"

/* TODO building a basic header class
to compose the right one to put at
the beginning of the response*/

class Response;

class HeadRsp
{
private:
	Response   *_response;
	std::string _header;
	std::string _statusCode;
	std::string _contentType;
	std::string _protocol;
	std::string _contentLength;
	std::string _connectionType;
	std::string _cacheControl;
	std::string _cookieString;

	HeadRsp &operator=(HeadRsp const &copy);
	HeadRsp();
	HeadRsp(HeadRsp const &copy);

public:
	HeadRsp(Response &response);
	~HeadRsp();

	void		setContentType(); // TODO check from script as well
	void		setProtocol();
	void		setRspStatusCode();
	void		setContentLength();
	void		setConnectionType();
	void		setCacheControl();
	void		buildHeader();
	void		setCookieString();
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