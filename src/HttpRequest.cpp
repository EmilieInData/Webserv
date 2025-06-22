/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:08 by cle-tron          #+#    #+#             */
/*   Updated: 2025/06/22 14:33:07 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "HttpParser.hpp"
#include "HttpParserTester.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

HttpRequest::HttpRequest() { 
	HttpParserTester::parseHttpMessageTest();
	HttpParserTester::parseRequestLineTest();
}

HttpRequest::HttpRequest( std::string const & message ) {
	try {
		std::vector<std::string>			lines = HttpParser::parseHttpMessage( message );
		std::vector<std::string>::iterator	it = lines.begin(), ite = lines.end();

		while ( it != ite && (*it).empty() )
			++it;

		req_line = HttpParser::parseRequestLine( *it );



	} catch ( std::invalid_argument e ) {
		std::cout << e.what() << std::endl;
	}
}

HttpRequest::HttpRequest( HttpRequest const & src ) { *this = src; }

HttpRequest::~HttpRequest() {
	if ( req_line )
		delete req_line;
}

HttpRequest &	HttpRequest::operator=( HttpRequest const & rhs ) {
	if ( this != &rhs ) {
		this->req_line = rhs.req_line;
	}
	return *this;
}

std::string	HttpRequest::getHttpMethod() const { return this->req_line->getMethod(); }

std::string	HttpRequest::getRequestUri() const { return this->req_line->getUri(); }

std::string	HttpRequest::getHttpVersion() const { return this->req_line->getVersion(); }


