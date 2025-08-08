/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:03 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/07 16:36:31 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include "HttpParser.hpp"
#include "HttpParserTester.hpp"
#include "RequestLine.hpp"
#include "ServerData.hpp"
#include "ServerManager.hpp"
#include "Uri.hpp"

class RequestLine;
class Uri;
class ServerManager;

class HttpRequest
{
private:
	RequestLine											*req_line;
	Uri													*uri;
	std::pair<std::string, std::string>					 host;
	std::pair<std::string, std::string>					 _fullPath;
	std::map<std::string, std::vector<std::string> >	headers;
	std::string	body;

public:
	HttpRequest();
	HttpRequest(std::pair<int, std::string> incoming, std::string fullRequest,
				ServerManager &server);	 // FABIO paired string with fd.
	HttpRequest(HttpRequest const &src);
	~HttpRequest();

	HttpRequest &operator=(HttpRequest const &rhs);

	std::string							getHttpMethod() const;
	std::string							getRequestUri() const;
	std::string							getPath() const;
	std::string							getQuery() const;
	std::string							getHttpVersion() const;
	std::pair<std::string, std::string> getFullPath() const;	// FABIO added this because I need full path for the response
	void								setFullPath(ServerData const &serv);
	

	//PROVISOIR
	std::map<std::string, std::vector<std::string> >::iterator getHeader( std::string const & title );
};

#endif
