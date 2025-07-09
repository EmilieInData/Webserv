/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:38:24 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/08 18:36:17 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestLine.hpp"
#include "HttpParser.hpp"
#include <string>

RequestLine::RequestLine( std::vector<std::string> const & line ) : method( line[0] ), uri( line[1] ), version( line[2] ) {
	
//	HttpParser::notImplementedMethod( method );
	path = HttpParser::parsePath( uri );
	query = HttpParser::parseQuery( uri );
}

RequestLine::RequestLine( RequestLine const & src ) { *this = src; }

RequestLine::RequestLine() {}

RequestLine::~RequestLine() {}

RequestLine &	RequestLine::operator=( RequestLine const & rhs ) {
	if ( this != &rhs ) {
		this->method = rhs.method;
		this->uri = rhs.uri;
		this->path = rhs.path;
		this->query = rhs.query;
		this->version = rhs.version;
	}
	return *this;
}

std::string	RequestLine::getMethod() const { return this->method; }

std::string	RequestLine::getUri() const { return this->uri; }

std::string	RequestLine::getPath() const { return this->path; }

std::string	RequestLine::getQuery() const { return this->query; }

std::string	RequestLine::getVersion() const { return this->version; }

