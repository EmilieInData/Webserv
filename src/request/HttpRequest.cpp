/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:08 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/14 18:07:08 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
/*
HttpRequest::HttpRequest() : req_line( NULL ), uri( NULL ), headers( NULL ) { 
	HttpParserTester::parseHttpMessageTest();
	HttpParserTester::parseRequestLineTest();
	HttpParserTester::parseHostTest();
	HttpParserTester::parseUriTest();
	HttpParserTester::parseHeadersTest();

}
*/
HttpRequest::HttpRequest(std::pair<int, std::string> incoming, ServerManager &server) : 
//HttpRequest::HttpRequest( ServerManager &server ) : 
req_line( NULL ), uri( NULL ), headers( NULL ), code( 200 ), incoming( incoming ), server( server ) {
	
/*
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


		state = REQ_LINE;
		req_line = new RequestLine( HttpParser::parseRequestLine( *it ));

		uri = new Uri( req_line->getReqTarget(), host.first );

		ServerData serv = HttpParser::checkIfServerExist( server.getServersList(), incoming );
		setFullPath(serv);
		HttpParser::checkIfPathExist( serv.getLocations(), uri->getPath()); // 404 not found si el uri no existe en servidor
		HttpParser::notAllowedMethod( serv.getItLocations( uri->getPath()), serv.getAllowedMethods(), req_line->getMethod());

		++it; 
	
		state = HEADERS;
		headers = new Headers( it, ite );

		if ( headers->getHeader( "content-length" ) != headers->getHeaderEnd() ) {
			state = BODY;
			body = lines.second;
		}
		
		state = DONE;

	} catch ( std::invalid_argument e ) {
		char	code_str[4];

		std::strncpy( code_str, e.what(), 3 );
		code_str[3] = '\0';

		code = std::atoi( code_str );
		std::cout << "ERROR CODE: " << code << std::endl;
		std::cout << e.what() << std::endl;
	}*/
}

HttpRequest::HttpRequest( HttpRequest const & src ) : server( src.server ) { *this = src; }

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

void	HttpRequest::sendBuffer( char * buffer, ssize_t bytes ) {
	(void)bytes;

	std::string	tmp( buffer );

	this->fullRequest = tmp;

	playParsing();
}

void	HttpRequest::playParsing() {

	try {
		std::string											tmp_host;
		std::pair<std::vector<std::string>, std::string>	lines = HttpParser::parseHttpMessage( this->fullRequest, tmp_host);
		std::vector<std::string>::iterator					it = lines.first.begin();
		std::vector<std::string>::iterator					ite = lines.first.end();

		this->host = HttpParser::parseHost( tmp_host ); // ACABAR EL PARSEO DE HOST CON FCT DE EMILIE  

		// std::cout << "tmp_host: \"" << tmp_host << "\" /host pair first: \"" << host.first;
		// std::cout << "\", second: \"" << host.second <<  "\"" << std::endl;

		while ( it != ite && (*it).empty() )
			++it;


		this->state = REQ_LINE;
		this->req_line = new RequestLine( HttpParser::parseRequestLine( *it ));

		this->uri = new Uri( req_line->getReqTarget(), host.first );

		ServerData serv = HttpParser::checkIfServerExist( this->server.getServersList(), this->incoming );
		setFullPath(serv);
		HttpParser::checkIfPathExist( serv.getLocations(), this->uri->getPath()); // 404 not found si el uri no existe en servidor
		HttpParser::notAllowedMethod( serv.getItLocations( this->uri->getPath()), serv.getAllowedMethods(), this->req_line->getMethod());

		++it; 
	
		this->state = HEADERS;
		this->headers = new Headers( it, ite );

		if ( this->headers->getHeader( "content-length" ) != this->headers->getHeaderEnd() ) {
			this->state = BODY;
			this->body = lines.second;
		}
		
		this->state = DONE;

	} catch ( std::invalid_argument e ) {
		char	code_str[4];

		std::strncpy( code_str, e.what(), 3 );
		code_str[3] = '\0';

		this->code = std::atoi( code_str );
		std::cout << "ERROR CODE: " << this->code << std::endl;
		std::cout << e.what() << std::endl;
	}
}


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

int	HttpRequest::getStatusCode() const { return this->code; }

int	HttpRequest::getParsingState() const { return this->state; }


