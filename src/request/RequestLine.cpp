/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:38:24 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/11 14:35:59 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestLine.hpp"
#include "HttpParser.hpp"
#include <string>

RequestLine::RequestLine( std::vector<std::string> const & line ) : method( line[0] ), req_target( line[1] ), version( line[2] ) {
	
//	HttpParser::notImplementedMethod( method );
	HttpParser::parseReqTarget( req_target );
//	path = HttpParser::parsePath( uri ); //borrar
//	query = HttpParser::parseQuery( uri ); //borrar
}

RequestLine::RequestLine( RequestLine const & src ) { *this = src; }

RequestLine::RequestLine() {}

RequestLine::~RequestLine() {}

RequestLine &	RequestLine::operator=( RequestLine const & rhs ) {
	if ( this != &rhs ) {
		this->method = rhs.method;
		this->req_target = rhs.req_target;
//		this->path = rhs.path;
//		this->query = rhs.query;
		this->version = rhs.version;
	}
	return *this;
}

std::string	RequestLine::getMethod() const { return this->method; }

std::string	RequestLine::getReqTarget() const { return this->req_target; }

//std::string	RequestLine::getPath() const { return this->path; }  //borrar

//std::string	RequestLine::getQuery() const { return this->query; } //borrar

std::string	RequestLine::getVersion() const { return this->version; }

