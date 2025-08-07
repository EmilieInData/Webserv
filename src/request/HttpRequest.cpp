/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:08 by cle-tron          #+#    #+#             */
/*   Updated: 2025/08/04 16:08:23 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() : req_line( NULL ), uri( NULL ) { 
	HttpParserTester::parseHttpMessageTest();
	HttpParserTester::parseRequestLineTest();
	HttpParserTester::parseHostTest();
	HttpParserTester::parseUriTest();

}

HttpRequest::HttpRequest(std::pair<int, std::string> incoming, std::string fullRequest, ServerManager &server) : req_line(NULL), uri(NULL) {
	try {

		// std::cout << message << std::endl;
		std::string							tmp_host;
		std::vector<std::string>			lines = HttpParser::parseHttpMessage( fullRequest, tmp_host );
		std::vector<std::string>::iterator	it = lines.begin();
		std::vector<std::string>::iterator	ite = lines.end();

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
		//anadir HOST PAIR 
    }
    return *this;
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


