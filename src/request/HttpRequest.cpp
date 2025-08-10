/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:08 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/10 14:00:43 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() : req_line( NULL ), uri( NULL ), headers( NULL ) { 
	HttpParserTester::parseHttpMessageTest();
	HttpParserTester::parseRequestLineTest();
	HttpParserTester::parseHostTest();
	HttpParserTester::parseUriTest();

}

HttpRequest::HttpRequest(std::pair<int, std::string> incoming, std::string fullRequest, ServerManager &server) : 
req_line( NULL ), uri( NULL ), headers( NULL ), code( 200 ) {
	try {
		std::string											tmp_host;
		std::pair<std::vector<std::string>, std::string>	lines = HttpParser::parseHttpMessage( fullRequest, tmp_host );
		std::vector<std::string>::iterator					it = lines.first.begin();
		std::vector<std::string>::iterator					ite = lines.first.end();

		host = HttpParser::parseHost( tmp_host ); // ACABAR EL PARSEO DE HOST CON FCT DE EMILIE  

		// std::cout << "tmp_host: \"" << tmp_host << "\" /host pair first: \"" << host.first;
		// std::cout << "\", second: \"" << host.second <<  "\"" << std::endl;

		while ( it != ite && (*it).empty() )
			++it;

		req_line = new RequestLine( HttpParser::parseRequestLine( *it ));

		uri = new Uri( req_line->getReqTarget(), host.first );

		ServerData serv = HttpParser::checkIfServerExist( server.getServersList(), incoming );
		setFullPath(serv);
		HttpParser::checkIfPathExist( serv.getLocations(), uri->getPath()); // 404 not found si el uri no existe en servidor
		HttpParser::notAllowedMethod( serv.getItLocations( uri->getPath()), serv.getAllowedMethods(), req_line->getMethod());

		++it;
	
		headers = new Headers( it, ite );

/*		//HEADERS
		while ( it != ite && !(*it).empty()) {
			std::vector<std::string>	tmp = HttpParser::split( *it, ':' );
			std::vector<std::string>	value = HttpParser::split( tmp[1], ',' );

			headers[ tmp[0] ] = value;

			++it;
		}

		//PRINTHEADERS
		std::map<std::string, std::vector<std::string> >::iterator	it_h, ite_h = headers.end();
		std::vector<std::string>::iterator							it_v, ite_v;

		for ( it_h = headers.begin(); it_h != ite_h; ++it_h ) {
			std::cout << "key: " << it_h->first << "	 values: ";
			ite_v = it_h->second.end();
			for ( it_v = it_h->second.begin(); it_v != ite_v; ++it_v )
				std::cout << *it_v << " ";
			std::cout << std::endl;
		}

		//BODY
		std::size_t found = fullRequest.rfind( "\r\n\r\n" );//Nooooooooooooooooooo
		body = fullRequest.substr( found + 4 , fullRequest.length() - found + 4);

		std::cout << "Body: " << body << std::endl;
*/

		body = lines.second;

	} catch ( std::invalid_argument e ) {
		char	code_str[4];

		std::strncpy( code_str, e.what(), 3 );
		code_str[3] = '\0';

		code = std::atoi( code_str );
		std::cout << "ERROR CODE: " << code << std::endl;
		std::cout << e.what() << std::endl;
	}
}

HttpRequest::HttpRequest( HttpRequest const & src ) { *this = src; }

HttpRequest::~HttpRequest() {
	if ( req_line )
		delete req_line;
	if ( uri )
		delete uri;
	if ( headers )
		delete headers;
}

HttpRequest& HttpRequest::operator=(const HttpRequest& rhs) {
    if (this != &rhs) {
        if (req_line) delete req_line;
        req_line = rhs.req_line ? new RequestLine(*rhs.req_line) : NULL;
		if (uri) delete uri;
        uri = rhs.uri ? new Uri(*rhs.uri) : NULL;
		host = rhs.host; 
		//anadir header 
		//anadir body
	}
    return *this;
}

/*
std::map<std::string, std::vector<std::string> >::iterator HttpRequest::getHeader( std::string const & title ) { 
	std::map<std::string, std::vector<std::string> >::iterator it = this->headers.find( title ); 

	if ( it == this->headers.end())
		std::cout << "Be careful! " << title << " header dont exist!" << std::endl;

	return it;
}*/

std::string	HttpRequest::getHttpMethod() const { return this->req_line->getMethod(); }

std::string	HttpRequest::getRequestUri() const { return this->uri->getUri(); }

std::string	HttpRequest::getPath() const { return this->uri->getPath(); }

/* TODO we need to check also for the case when we get an absolute path i think
 getFullPath() just adds the root to the path in the Uri from the request */

void HttpRequest::setFullPath(ServerData const &serv)
{
	_fullPath = std::make_pair<std::string, std::string>(serv.getRoot(), uri->getPath());
}

std::pair<std::string, std::string> HttpRequest::getFullPath() const
{
	return _fullPath;
}

std::string	HttpRequest::getQuery() const { return this->uri->getQuery(); }

std::string	HttpRequest::getHttpVersion() const { return this->req_line->getVersion(); }


