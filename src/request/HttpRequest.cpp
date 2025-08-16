/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:08 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/16 14:53:03 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include <fcntl.h>
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
req_line( NULL ), uri( NULL ), headers( NULL ), code( 200 ), state( SKIP ), incoming( incoming ), server( server ) {
	headers = new Headers();
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

	//anadir el buffer al buffer existente(fullRequest) con push_back
	//bucle while encuentra CRLF en el buffer
	//buscar el primer CRLF en el buffer 
	//si no lo encuentra esperar / salir del bucle ?
	//cortar el primer CRLF parsear y eliminar 
	//elegir fct del parseo con un switch en fct del state 
	//mandar el string en parametro sin el CRLF

	for ( int i = 0; i < bytes; i++ )
		this->fullRequest.push_back( buffer[i] );
	
	std::size_t	found = 0;
	int			i = 0;

	
	try {
		while ( found != std::string::npos ) {
			found = fullRequest.find( CRLF );
			std::string tmp = fullRequest.substr( 0, found );
			fullRequest.erase( 0, found + 2 );

			std::cout << "TMP " << i++ << ": " << tmp << std::endl;

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

	this->state = DONE;

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
		HttpParser::checkIfPathExist( serv.getLocations(), this->uri->getPath()); // 404 not found si el uri no existe en servidor
		HttpParser::notAllowedMethod( serv.getItLocations( this->uri->getPath()), serv.getAllowedMethods(), this->req_line->getMethod());



		++it; 
	
	//	this->state = HEADERS; //OK
		this->headers = new Headers( ); //ok

		if ( this->headers->getHeader( "content-length" ) != this->headers->getHeaderEnd() ) { //ok
//			this->state = BODY;
			this->body = lines.second;
		}
		
//		this->state = DONE; //OK

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
	


//	if ( host.second[0].empty()) throw std::invalid_argument( E_400 ); 
	this->host = HttpParser::parseHost( this->headers->getHeaderOnlyOneValue( "host", 0 )); 
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


