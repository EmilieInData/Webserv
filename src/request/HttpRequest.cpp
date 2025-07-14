/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:08 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/14 13:07:30 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "HttpParser.hpp"
#include "HttpParserTester.hpp"
#include "Uri.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

HttpRequest::HttpRequest() : req_line( NULL ), uri( NULL ) { 
	HttpParserTester::parseHttpMessageTest();
	HttpParserTester::parseRequestLineTest();
	HttpParserTester::parseUriTest();
}

HttpRequest::HttpRequest( std::string const & message ) : req_line( NULL ), uri( NULL ) {
	try {

		std::cout << message << std::endl;

		std::vector<std::string>			lines = HttpParser::parseHttpMessage( message );
		std::vector<std::string>::iterator	it = lines.begin(), ite = lines.end();

		while ( it != ite && (*it).empty() )
			++it;

		req_line = new RequestLine( HttpParser::parseRequestLine( *it ));

		std::string host( "www.example.org" );

		uri = new Uri( req_line->getReqTarget(), host );

		//HttpParser::notImplementedMethod( req_line->method, header->find( "path" ) );

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


