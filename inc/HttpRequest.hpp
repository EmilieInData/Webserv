/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:03 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/04 10:25:53 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HTTPREQUEST_HPP
#define	HTTPREQUEST_HPP

#include "ServerManager.hpp"
#include "HttpParser.hpp"
#include "HttpParserTester.hpp"
#include "RequestLine.hpp"
#include "Uri.hpp"
#include "ServerData.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

class	RequestLine;
class	Uri;
class	ServerManager;

class	HttpRequest	{
private:	
	RequestLine	*						req_line;
	Uri *								uri;
	std::pair<std::string, std::string>	host;
	std::pair<std::string, std::string>	_fullPath;
//	RequestHeaders	headers;
//	RequestBody		body;	

public:
	HttpRequest();
	HttpRequest( std::pair<int, std::string> incoming, std::string fullRequest, ServerManager & server ); // FABIO paired string with fd.
	HttpRequest( HttpRequest const & src );
	~HttpRequest();

	HttpRequest &	operator=( HttpRequest const & rhs );	
	
	std::string							getHttpMethod() const;
	std::string							getRequestUri() const;
	std::string							getPath() const;
	std::string							getQuery() const;
	std::string							getHttpVersion() const;
	std::pair<std::string, std::string>	getFullPath() const; // FABIO added this because I need full path for the response
	void 								setFullPath(ServerData const &serv);
};

#endif
