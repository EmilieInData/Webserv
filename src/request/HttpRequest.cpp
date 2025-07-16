/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:08 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/16 17:45:18 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "HttpParser.hpp"
#include "HttpParserTester.hpp"
#include "RequestLine.hpp"
#include "Uri.hpp"
#include "Server.hpp"
#include "ServerConf.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

HttpRequest::HttpRequest() : req_line( NULL ), uri( NULL ) { 
	HttpParserTester::parseHttpMessageTest();
	HttpParserTester::parseRequestLineTest();
	HttpParserTester::parseUriTest();
}

HttpRequest::HttpRequest( std::string const & message, Server & server) : req_line( NULL ), uri( NULL ) {
	try {

		std::cout << message << std::endl;

		std::string							tmp_host;
		std::vector<std::string>			lines = HttpParser::parseHttpMessage( message, tmp_host );
		std::vector<std::string>::iterator	it = lines.begin(), ite = lines.end();

//		host = HttpParser::parseHost( tmp_host ); // HACER EL PARSEO DE HOSTTTTT 

		while ( it != ite && (*it).empty() )
			++it;

		req_line = new RequestLine( HttpParser::parseRequestLine( *it ));

		std::cout << "host from parseHttpMessage: " << tmp_host << std::endl;

		uri = new Uri( req_line->getReqTarget(), tmp_host );

		ServerConf serv = HttpParser::checkIfServerExist( server.getServersList(), tmp_host ); //solo host.getName() ?????
		HttpParser::checkIfPathExist( serv.getLocations(), uri->getPath()); // 404 not found si el uri no existe en servidor
		HttpParser::notAllowedMethod( serv.getItLocations( uri->getPath()), serv.getAllowedMethods(), req_line->getMethod());

	} catch ( std::invalid_argument e ) {
		std::cout << e.what() << std::endl;
	}
}

HttpRequest::HttpRequest( HttpRequest const & src ) { *this = src; }

HttpRequest::~HttpRequest() {
	if ( req_line )
		delete req_line;
	if ( uri )
		delete uri;
}

HttpRequest& HttpRequest::operator=(const HttpRequest& rhs) {
    if (this != &rhs) {
        if (req_line) delete req_line;
        req_line = rhs.req_line ? new RequestLine(*rhs.req_line) : NULL;
		if (uri) delete uri;
        uri = rhs.uri ? new Uri(*rhs.uri) : NULL;
    }
    return *this;
}

std::string	HttpRequest::getHttpMethod() const { return this->req_line->getMethod(); }

std::string	HttpRequest::getRequestUri() const { return this->uri->getUri(); }

std::string	HttpRequest::getPath() const { return this->uri->getPath(); }

std::string	HttpRequest::getQuery() const { return this->uri->getQuery(); }

std::string	HttpRequest::getHttpVersion() const { return this->req_line->getVersion(); }


