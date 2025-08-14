/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:32:05 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/14 18:07:37 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#define	REQ_LINE	1
#define	HEADERS		2
#define BODY		3
#define	DONE		0

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
	Headers								*headers;
	std::string							body;
	int									code;
	int									state;
	std::string							fullRequest;
	std::pair<int, std::string>			incoming;
	ServerManager &						server;		

public:
//	HttpRequest();
//	HttpRequest( ServerManager & server );
	HttpRequest(std::pair<int, std::string> incoming, ServerManager &server);// FABIO paired string with fd.
	HttpRequest( HttpRequest const &src );
	~HttpRequest();

	HttpRequest &operator=(HttpRequest const &rhs);

	void								sendBuffer( char *buffer, ssize_t bytes );
	void								playParsing();

	std::string							getHttpMethod() const;
	std::string							getRequestUri() const;
	std::string							getPath() const;
	std::string							getQuery() const;
	std::string							getHttpVersion() const;
	std::pair<std::string, std::string> getFullPath() const;	// FABIO added this because I need full path for the response
	void								setFullPath(ServerData const &serv);
	int									getStatusCode() const;
	int									getParsingState() const;
	

	//PROVISOIR
//	std::map<std::string, std::vector<std::string> >::iterator getHeader( std::string const & title );
};

#endif
