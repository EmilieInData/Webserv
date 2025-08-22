/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:08 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/22 14:51:30 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
//#include <fcntl.h>
#include <stdlib.h>

HttpRequest::HttpRequest( ServerManager & server ) : req_line( NULL ), uri( NULL ), headers( NULL ), server( server ) { 
	HttpParserTester::parseHttpMessageTest();
	HttpParserTester::parseRequestLineTest();
	HttpParserTester::parseHostTest();
	HttpParserTester::parseUriTest();
	HttpParserTester::parseHeadersTest();

}

HttpRequest::HttpRequest(std::pair<int, std::string> incoming, ServerManager &server) : 
req_line( NULL ), uri( NULL ), headers( NULL ), boundary( "" ), code( 200 ), state( SKIP ), 
incoming( incoming ), server( server ) {
	headers = new Headers();
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
        if (this->req_line) 
			delete this->req_line;
        this->req_line = rhs.req_line ? new RequestLine(*rhs.req_line) : NULL;
		if (this->uri) 
			delete this->uri;
        this->uri = rhs.uri ? new Uri(*rhs.uri) : NULL;
		if (this->headers) 
			delete this->headers;
		this->headers = rhs.headers ? new Headers(*rhs.headers) : NULL;
		this->host = rhs.host;
		this->_fullPath = rhs._fullPath;
		this->body = rhs.body;
		this->code = rhs.code;
		this->state = rhs.state;
		this->fullRequest = rhs.fullRequest;
		this->incoming = rhs.incoming;
		this->location = rhs.location;
	}
    return *this;
}

void	HttpRequest::sendBuffer( char * buffer, ssize_t bytes ) {

	for ( int i = 0; i < bytes; i++ )
		this->fullRequest.push_back( buffer[i] );
	
	std::size_t	found = fullRequest.find( CRLF );
//	std::cout << "FULLREQUEST: " << fullRequest << std::endl;

	static int	i = 0;
	
	try {
		while ( found != std::string::npos && this->state != BODY ) {
			std::string tmp = fullRequest.substr( 0, found );
			fullRequest.erase( 0, found + 2 );
			
		//	if ( i < 15) 
			std::cout << "TMP " << i++ << ": " << tmp << std::endl;
			//si no es body verifier que les char sont printable

			switch( this->state ) {
				case SKIP:
					if ( tmp.empty())
						break;
					else this->state = REQ_LINE;
				case REQ_LINE:
					this->req_line = new RequestLine( HttpParser::parseRequestLine( tmp ));
					this->state = HEADERS;
					break;
				case HEADERS:
					if ( tmp.empty()) {
						finalHeadersParsingRoutine();
						break;
					}
					this->headers->setHeader( tmp );
					this->state = HEADERS;
					break;
			}
			found = fullRequest.find( CRLF );
		}
		
		if ( this->state == BODY ) {

			if ( !this->boundary.empty()) {
				this->body += this->fullRequest;
				std::size_t	f = fullRequest.find( CRLF );
				static int j = 0;
			while ( f != std::string::npos ) {
				std::string tmp = fullRequest.substr( 0, f );
				fullRequest.erase( 0, f + 2 );
				
				if ( j < 5 )
					std::cout << "BODYTMP " << j++ << ": " << tmp << std::endl;
				f = fullRequest.find( CRLF );
			}


			}
			else {
				this->body += this->fullRequest;
					if ( this->body.length() >= this->body_len ) {
					if ( this->body.length() > this->body_len )
						this->body.erase( this->body_len, this->body.length());
					this->state = DONE;
				}
			}
		}

	} catch ( std::invalid_argument e ) {
		this->setStatusCode( e.what());
	}

//	std::cout << "STATE IN FCT: " << this->state << std::endl;

//	this->state = DONE; //solo poner en caso de debug para que no se quede colgado

}

void	HttpRequest::finalHeadersParsingRoutine() {
	if ( this->headers->getHeader( "cookie" ) != this->headers->getHeaderEnd() ) 
		this->headers->setManyValuesHeader( "cookie" );
	if ( this->headers->getHeader( "content-type" ) != this->headers->getHeaderEnd() ) 
		this->headers->setManyValuesHeader( "content-type" );
	this->headers->printHeader();
	checkHost( this->headers->getHeader( "host" ));
	this->uri = new Uri( req_line->getReqTarget(), this->host.first );
	ServerData serv = HttpParser::checkIfServerExist( this->server.getServersList(), this->incoming );
	setFullPath(serv);
	this->max_body_size = serv.getBodySize();
//	std::cout << "FULLPATH: " << this->_fullPath.first << " " << this->_fullPath.second << std::endl;
//	std::cout << "PATH: " << this->uri->getPath() << std::endl;
	setLocation( serv.getLocations(), this->_fullPath.second );
	HttpParser::checkIfPathExist( this->_fullPath );
	HttpParser::notAllowedMethod( serv.getItLocations( this->location ), serv.getAllowedMethods(), this->req_line->getMethod());
	if ( this->headers->getHeader( "content-type" ) != this->headers->getHeaderEnd()) {
		this->boundary = HttpParser::parseContentTypeBoundary( this->headers->getHeaderValue( "content-type"));
		this->state = BODY;
	}
	else if ( this->headers->getHeader( "content-length" ) != this->headers->getHeaderEnd() ) {
		this->body_len = HttpParser::parseContentLengthHeader( this->headers->getHeaderOnlyOneValue( "content-length", 0 ), this->max_body_size );
		this->state = BODY;
	}
	else
		this->state = DONE;
}

void	HttpRequest::setStatusCode( std::string error ) {
	char	code_str[4];

	std::strncpy( code_str, error.c_str(), 3 );

	code_str[3] = '\0';

	this->state = ERR;

	this->code = std::atoi( code_str );
	std::cout << "ERROR CODE: " << code << std::endl;
	std::cout << error << std::endl;
}

void HttpRequest::setLocation( std::map<std::string, LocationConf> & location, std::string const & path ) {
	std::size_t found = path.rfind( "/" );

//	if ( found == std::string::npos ) //BADREQUEST
		
	this->location = path.substr( 0, found + 1 );
//	std::cout << "LOCATION REQ: " << this->location << std::endl;

	std::map<std::string, LocationConf>::iterator	it = location.find( this->location );

//	for ( std::map<std::string, LocationConf>::iterator itt = location.begin(); itt != location.end(); ++itt)
//		std::cout << "LOCATION CONF: " << itt->first << std::endl;

	if ( it == location.end() ) throw std::invalid_argument( E_404 ); 

//	std::cout << "LOCATION EXIST IN SERVER: " << (*it).first << std::endl;
}

void	HttpRequest::checkHost( std::map<std::string, std::vector<std::string> >::const_iterator it ) {
	if ( it == this->headers->getHeaderEnd()) throw std::invalid_argument( E_400 ); 

	if ( this->headers->getHeaderOnlyOneValue( "host", 0 ).empty()) throw std::invalid_argument( E_400 ); 
	
	this->host = HttpParser::parseHost( this->headers->getHeaderOnlyOneValue( "host", 0 )); 
}


std::string	HttpRequest::getHttpMethod() const { return this->req_line->getMethod(); }

std::string	HttpRequest::getRequestUri() const { return this->uri->getUri(); }

std::string	HttpRequest::getPath() const { return this->uri->getPath(); }

/* TODO we need to check also for the case when we get an absolute path i think
 getFullPath() just adds the root to the path in the Uri from the request */

void HttpRequest::setFullPath(ServerData const &serv) {
	_fullPath = std::make_pair<std::string, std::string>(serv.getRoot(), uri->getPath());
}

std::pair<std::string, std::string> HttpRequest::getFullPath() const { return _fullPath; }

std::string	HttpRequest::getQuery() const { return this->uri->getQuery(); }

std::string	HttpRequest::getHttpVersion() const { return this->req_line->getVersion(); }

int	HttpRequest::getStatusCode() const { return this->code; }

int	HttpRequest::getParsingState() const { return this->state; }


