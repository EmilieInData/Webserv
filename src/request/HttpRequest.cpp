/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:08 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/17 10:33:32 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include <fcntl.h>

HttpRequest::HttpRequest( ServerManager & server ) : req_line( NULL ), uri( NULL ), headers( NULL ), server( server ) { 
	HttpParserTester::parseHttpMessageTest();
	HttpParserTester::parseRequestLineTest();
	HttpParserTester::parseHostTest();
	HttpParserTester::parseUriTest();
	HttpParserTester::parseHeadersTest();

}

HttpRequest::HttpRequest(std::pair<int, std::string> incoming, ServerManager &server) : 
req_line( NULL ), uri( NULL ), headers( NULL ), code( 200 ), state( SKIP ), incoming( incoming ), server( server ) {
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
	}
    return *this;
}

void	HttpRequest::sendBuffer( char * buffer, ssize_t bytes ) {

	for ( int i = 0; i < bytes; i++ )
		this->fullRequest.push_back( buffer[i] );
	
	std::size_t	found = fullRequest.find( CRLF );
//	std::cout << "FULLREQUEST: " << fullRequest << std::endl;
	if ( found == std::string::npos ) return; // opcional ?

	static int			i = 0;

	
	try {
		while ( found != std::string::npos ) {
			std::string tmp = fullRequest.substr( 0, found );
			fullRequest.erase( 0, found + 2 );

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
						this->headers->printHeader();
						checkHost( this->headers->getHeader( "host" ));
						this->uri = new Uri( req_line->getReqTarget(), this->host.first );
						ServerData serv = HttpParser::checkIfServerExist( this->server.getServersList(), this->incoming );
						setFullPath(serv);
						HttpParser::checkIfPathExist( serv.getLocations(), this->uri->getPath()); // 404 not found si el uri no existe en servidor
						HttpParser::notAllowedMethod( serv.getItLocations( this->uri->getPath()), serv.getAllowedMethods(), this->req_line->getMethod());

						if ( this->headers->getHeader( "content-length" ) != this->headers->getHeaderEnd() )
							this->state = BODY;
						else
							this->state = DONE;
						break;
					}
					this->headers->setHeader( tmp );
					this->state = HEADERS;
					break;
				case BODY:
					std::cout << "BODY HERE !" << std::endl;
					this->state = DONE;
					break;
			}
			found = fullRequest.find( CRLF );
		}
	} catch ( std::invalid_argument e ) {
		char	code_str[4];

		std::strncpy( code_str, e.what(), 3 );
		code_str[3] = '\0';

		this->state = ERR;

		code = std::atoi( code_str );
		std::cout << "ERROR CODE: " << code << std::endl;
		std::cout << e.what() << std::endl;
	}
	

	std::cout << "STATE IN FCT: " << this->state << std::endl;

//	this->state = DONE; //solo poner en caso de debug para que no se quede colgado

}

void	HttpRequest::playParsing( std::string & tmp) {

	try {
		std::string											tmp_host;
		std::pair<std::vector<std::string>, std::string>	lines = HttpParser::parseHttpMessage( tmp, tmp_host);
		std::vector<std::string>::iterator					it = lines.first.begin();
		std::vector<std::string>::iterator					ite = lines.first.end();

		this->host = HttpParser::parseHost( tmp_host ); // ACABAR EL PARSEO DE HOST CON FCT DE EMILIE //ok 

		// std::cout << "tmp_host: \"" << tmp_host << "\" /host pair first: \"" << host.first;
		// std::cout << "\", second: \"" << host.second <<  "\"" << std::endl;

		while ( it != ite && (*it).empty() ) //OK
			++it;


	//	this->state = REQ_LINE;
		this->req_line = new RequestLine( HttpParser::parseRequestLine( *it )); //ok

		this->uri = new Uri( req_line->getReqTarget(), host.first ); //ok

		ServerData serv = HttpParser::checkIfServerExist( this->server.getServersList(), this->incoming );//ok
		setFullPath(serv);//ok
		
/*		std::cout << "GETPATH(): " << this->uri->getPath() << std::endl;
		std::cout << "GETFULLPATH(): " << this->getFullPath().first << " " << this->getFullPath().second << std::endl;
		std::cout << "INCOMING: " << this->incoming.first << " " << this->incoming.second << std::endl;
		std::cout << "GETURI(): " << this->getRequestUri() << std::endl;
		std::cout << "GETSERVERNAME(): " << serv.getServerName()[0] << " " << serv.getServerName()[1] << " " << serv.getServerName()[2] << std::endl;
		//hacer algo con servername
		std::string	path_location = "/www/server01" + this->uri->getPath();

		HttpParser::checkIfPathExist( serv.getLocations(), path_location); // 404 not found si el uri no existe en servidor
		HttpParser::notAllowedMethod( serv.getItLocations( path_location), serv.getAllowedMethods(), this->req_line->getMethod());
*/
//Hacer el check if path exists con access() ???? 
//		std::string	path_location = "/www/server01" + this->uri->getPath();
//		if ( access( path_location.c_str(), F_OK ) != 0 ) throw std::invalid_argument( E_404 );
		HttpParser::checkIfPathExist( serv.getLocations(), this->uri->getPath()); // 404 not found si el uri no existe en servidor //ok
		HttpParser::notAllowedMethod( serv.getItLocations( this->uri->getPath()), serv.getAllowedMethods(), this->req_line->getMethod()); //ok



		++it; 
	
		this->state = HEADERS; //OK
		this->headers = new Headers( ); //ok

		if ( this->headers->getHeader( "content-length" ) != this->headers->getHeaderEnd() ) { //ok
			this->state = BODY;
			this->body = lines.second;
		}
		
		this->state = DONE; //OK

	} catch ( std::invalid_argument e ) {
		char	code_str[4];

		std::strncpy( code_str, e.what(), 3 );
		code_str[3] = '\0';

		this->state = ERR;
		this->code = std::atoi( code_str );
		std::cout << "ERROR CODE: " << this->code << std::endl;
		std::cout << e.what() << std::endl;
	}
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


