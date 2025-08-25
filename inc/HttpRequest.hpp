/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:32:05 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/24 16:06:33 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#define SKIP		1
#define	REQ_LINE	2
#define	HEADERS		3
#define BODY		4
#define BOUNDARY	5
#define HEADERS2	6
#define BODY2		7
#define	DONE		0
#define ERR			-1
#define CRLF		"\r\n"
#define TWO_CRLF	"\r\n\r\n"

#include <iostream>
#include <stdexcept>
#include <string>
#include "HttpParser.hpp"
#include "HttpParserTester.hpp"
#include "RequestLine.hpp"
#include "ServerData.hpp"
#include "ServerManager.hpp"
#include "Uri.hpp"
#include "Headers.hpp"
#include "Utils.hpp"

class RequestLine;
class Uri;
class ServerManager;
class Headers;

class HttpRequest
{
private:
	RequestLine							*req_line;
	Uri									*uri;
	std::pair<std::string, std::string>	host;
	std::pair<std::string, std::string>	_fullPath;
	std::string							location;
	Headers								*headers;
	std::string							body;
	std::size_t							body_len;
	std::size_t							max_body_size;
	std::string							boundary;
	bool								boundary_flag;
	int									code;
	int									state;
	int									body_state;
	std::string							fullRequest;
	std::pair<int, std::string>			incoming;
	ServerManager &						server;	

	HttpRequest();

	void	checkHost( std::map<std::string, std::vector<std::string> >::const_iterator it );
	void	finalHeadersParsingRoutine();
	void	setFullPath(ServerData const &serv);
	void	setLocation( std::map<std::string, LocationConf> & location,std::string const &path );
	void	manyBodiesRoutine( std::size_t found );

public:
	HttpRequest( ServerManager & server );
	HttpRequest(std::pair<int, std::string> incoming, ServerManager &server);// FABIO paired string with fd.
	HttpRequest( HttpRequest const &src );
	~HttpRequest();

	HttpRequest &operator=(HttpRequest const &rhs);

	void								sendBuffer( char *buffer, ssize_t bytes );
	void								setStatusCode( std::string error );
	std::string							getHttpMethod() const;
	std::string							getRequestUri() const;
	std::string							getPath() const;
	std::string							getQuery() const;
	std::string							getHttpVersion() const;
	std::pair<std::string, std::string> getFullPath() const;	// FABIO added this because I need full path for the response
	int									getStatusCode() const;
	int									getParsingState() const;
	

	//PROVISOIR
//	std::map<std::string, std::vector<std::string> >::iterator getHeader( std::string const & title );
};

#endif
