/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:08 by cle-tron          #+#    #+#             */
/*   Updated: 2025/09/05 19:21:55 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "ServerManager.hpp"
#include <stdlib.h>

HttpRequest::HttpRequest(std::pair<int, std::string> incoming, ServerManager &server)
	: req_line(NULL), uri(NULL), headers(NULL), body(""), body_len(0), boundary(""),
	  boundary_flag(false), code(200), state(SKIP), incoming(incoming), server(server)
{
	std::cout << "INCOMING FIRST: " << incoming.first << " SECOND: " << incoming.second << std::endl;

	headers = new Headers();
}

HttpRequest::HttpRequest(HttpRequest const &src) : server(src.server)
{
	*this = src;
}

HttpRequest::~HttpRequest()
{
	if (req_line)
		delete req_line;
	if (uri)
		delete uri;
	if (headers)
		delete headers;
}

HttpRequest &HttpRequest::operator=(const HttpRequest &rhs)
{
	if (this != &rhs)
	{
		if (this->req_line)
			delete this->req_line;
		this->req_line = rhs.req_line ? new RequestLine(*rhs.req_line) : NULL;
		if (this->uri)
			delete this->uri;
		this->uri = rhs.uri ? new Uri(*rhs.uri) : NULL;
		if (this->headers)
			delete this->headers;
		this->headers	  = rhs.headers ? new Headers(*rhs.headers) : NULL;
		this->host		  = rhs.host;
		this->_fullPath	  = rhs._fullPath;
		this->body		  = rhs.body;
		this->code		  = rhs.code;
		this->state		  = rhs.state;
		this->fullRequest = rhs.fullRequest;
		this->incoming	  = rhs.incoming;
		this->location	  = rhs.location;
		//check si falta algo
	}
	return *this;
}

void	HttpRequest::sendBuffer(char *buffer, ssize_t bytes)
{

	for (int i = 0; i < bytes; i++)
		this->fullRequest.push_back(buffer[i]);

	std::size_t found = fullRequest.find(CRLF);
	//	std::cout << "FULLREQUEST: " << fullRequest << std::endl;

	static int i = 0;

	try
	{
		while (found != std::string::npos && this->state != BODY)
		{
			std::string tmp = this->fullRequest.substr(0, found);
			this->fullRequest.erase(0, found + 2);

			std::cout << "TMP " << i++ << ": " << tmp << std::endl;
			if ( !HttpParser::isAsciiPrintable( tmp )) throw std::invalid_argument( E_400 ); 

			switch (this->state)
			{
			case SKIP:
				if (tmp.empty())
					break;
				else
					this->state = REQ_LINE;
			case REQ_LINE:
				this->req_line = new RequestLine(HttpParser::parseRequestLine(tmp));
				this->state	   = HEADERS;
				break;
			case HEADERS:
				if (tmp.empty())
				{
					finalHeadersParsingRoutine();
					break;
				}
				this->headers->setHeader(tmp);
				this->state = HEADERS;
				break;
			}
			found = fullRequest.find(CRLF);
		}

		if (this->state == BODY)
		{
			if (!this->boundary.empty()) // TODO create newBody here and pass it to routine?
				manyBodiesRoutine(found);
			else
			{
				this->body = this->fullRequest;
				//	std::cout << "BODY: " << this->body << std::endl;
				//	std::cout << "LEN: " << this->body.length() << " LEN IN STRUCT: " << this->body_len << std::endl;
				if (this->body.length() >= this->body_len)
				{
					if (this->body.length() > this->body_len)
						this->body.erase(this->body_len, this->body.length());
					this->state = DONE;
				}
			}
		}
	}
	catch (std::invalid_argument e)
	{
		this->setStatusCode(e.what());
	}



	// FABIO end of request parsing here
	
	//	std::cout << "STATE IN FCT: " << this->state << std::endl;

	//	this->state = DONE; //solo poner en caso de debug para que no se quede colgado
}

void HttpRequest::printBodies()
{
	std::cout << "Bodies size: " << _bodies.size() << std::endl;
	// for (std::vector<MultiBody>::const_iterator it = _bodies.begin(); it != _bodies.end(); it++)
	// {
	// 	std::cout << GREEN << "Body content" << std::endl;
	// 	it->bodyHeader.printHeader();
	// 	std::cout << it->bodyContent << std::endl;
	// 	std::cout << RESET << std::endl;
	// }
}

void HttpRequest::finalHeadersParsingRoutine()
{
	if (this->headers->getHeader("cookie") != this->headers->getHeaderEnd())
		this->headers->setManyValuesHeader("cookie");

	if (this->headers->getHeader("content-type") != this->headers->getHeaderEnd())
		this->headers->setManyValuesHeader("content-type");
	//this->headers->printHeader();
	
	ServerData serv = HttpParser::checkIfServerExist(this->server.getServersList(), this->incoming);
	
	checkHost(this->headers->getHeader("host"), serv);
	
	this->uri		= new Uri(req_line->getReqTarget(), this->host.first);

	setFullPath(serv);
	
	this->max_body_size = serv.getBodySize();
	//	std::cout << "FULLPATH: " << this->_fullPath.first << " " << this->_fullPath.second << std::endl;
	//	std::cout << "PATH: " << this->uri->getPath() << std::endl;
	setLocation(serv.getLocations(), this->_fullPath.second);

	HttpParser::notAllowedMethod(serv.getItLocations(this->location), serv.getAllowedMethods(), this->req_line->getMethod());
	HttpParser::checkIfPathExist(this->_fullPath, getAutoindex(), serv.getLocations(), this->getHttpMethod() );
	
	if (this->headers->getHeader("content-type") != this->headers->getHeaderEnd())
	{
		this->boundary = HttpParser::parseContentTypeBoundary(this->headers->getHeaderValue("content-type"));
		if (!this->boundary.empty())
		{
			this->state			= BODY;
			this->body_state	= BOUNDARY;
			this->boundary_flag = true;
		}
	}
	if (this->headers->getHeader("content-length") != this->headers->getHeaderEnd())
	{
		this->body_len = HttpParser::parseContentLengthHeader(this->headers->getHeaderOnlyOneValue("content-length", 0),
															  this->max_body_size);
		this->state = BODY;
	}
	if (this->headers->getHeader("content-type") != this->headers->getHeaderEnd() ||
		this->headers->getHeader("content-length") != this->headers->getHeaderEnd())
		return;
	this->state = DONE;
}

void HttpRequest::manyBodiesRoutine(std::size_t found)
{
	static int k = -1;			// TODO no needed
	static bool makeNew = true; // FABIO created boolean so that it only creates the bodies when needed
	static MultiBody newBody;

	while (found != std::string::npos && this->body_state != BODY2)
	{
		std::string tmp = this->fullRequest.substr(0, found);
		this->fullRequest.erase(0, found + 2);
		if (makeNew)
		{
			newBody = MultiBody();
			makeNew = false;
		}
		switch (this->body_state)
		{
		case BOUNDARY:
			if (this->boundary_flag == true && tmp != "--" + this->boundary)
			{
				this->boundary_flag = false;
				throw std::invalid_argument(E_400); //si el primer boundary no corresponde: error
			}
			//FABIO create new struct body
			this->boundary_flag = false;
			k++;
			std::cout << std::endl << GREEN << "              NEWBODY " << k << RESET << std::endl;
			this->body_state = HEADERS2;
			break;
		case HEADERS2:
			if (tmp.empty())
			{
				this->body_state = BODY2;
				break;
			}
			std::cout << "HEADER " << k << ": " << tmp.substr(0, 70)
					  << std::endl; // FABIO structbody->header->setHeader( tmp )
			newBody.bodyHeader.setHeader(tmp);
			this->body_state = HEADERS2;
			break;
		}

		found = this->fullRequest.find(CRLF);
	}

	this->body = this->fullRequest;

	std::size_t f = this->body.find("\r\n--" + this->boundary + "--\r\n"); //ULTIMO BODY
	if (f != std::string::npos)
	{
		this->body = this->body.erase(f, this->body.length()); // FABIO poner body en la struct del ultimo body
		newBody.bodyContent = this->body;
		std::cout << "BODY   " << k << ": " << this->body.substr(0, 70) << std::endl;
		this->_bodies.push_back(newBody);
		makeNew		= true;
		this->state = DONE;
	}
	std::size_t f2 = this->body.find("\r\n--" + this->boundary + "\r\n"); //OTRO BODY
	if (f2 != std::string::npos)
	{
		this->body = this->body.erase(f2, this->body.length()); //FABIO poner body  en la struct
		newBody.bodyContent = this->body;
		std::cout << "BODY   " << k << ": " << this->body.substr(0, 70) << std::endl;
		this->_bodies.push_back(newBody);
		makeNew			  = true;
		this->fullRequest = this->fullRequest.erase(0, f2 + 6);
		this->body_state  = BOUNDARY;
	}
}

void HttpRequest::setStatusCode(std::string error)
{
	char code_str[4];

	std::strncpy(code_str, error.c_str(), 3);

	code_str[3] = '\0';

	this->state = ERR;

	this->code = std::atoi(code_str);
	std::cout << "ERROR CODE: " << code << std::endl;
	std::cout << error << std::endl;
}

void HttpRequest::setLocation(std::map<std::string, LocationConf> const& location, std::string const &path)
{
	std::size_t found = path.rfind("/");
	std::cout << PINK  << "request path: " << path << std::endl << RESET; //TO BORROW

	//	if ( found == std::string::npos ) //BADREQUEST

	this->location = path.substr(0, found + 1);
	std::cout << PINK << "LOCATION REQ: " << this->location << std::endl;
// EMILIE this not ok because for ./error_pages/ esta buscando / solo
	std::map<std::string, LocationConf>::const_iterator it = location.find(this->location);

	//	for ( std::map<std::string, LocationConf>::iterator itt = location.begin(); itt != location.end(); ++itt)
	//		std::cout << "LOCATION CONF: " << itt->first << std::endl;
	setRspType();							// FABIO added here, seems the best place for now
	if (_rspType == "cgi-script")
		server.getScript().runScript(*this);
		// TODO if script fails throw error here
	if (it == location.end())
		throw std::invalid_argument(E_404);
	_autoindex = it->second.getAutoindex(); //ADD by EMILIE
	//	std::cout << "LOCATION EXIST IN SERVER: " << (*it).first << std::endl;
}

void HttpRequest::setRspType()
{
	std::string location = _fullPath.first + _fullPath.second;
	
	if (isFolder(location)) // && _autoindex)
	{
		_rspType = "text/html";
		return ;
	}

	std::string extension;
	size_t		dotPos = location.find_last_of('.');
	if (dotPos != std::string::npos)
		extension = location.substr(dotPos);
	

	if (extension == ".html" || extension == ".htm")
		_rspType = "text/html";
	else if (extension == ".jpg" || extension == ".jpeg")
		_rspType = "image/jpeg";
	else if (extension == ".png")
		_rspType = "image/png";
	else if (extension == ".gif")
		_rspType = "image/gif";
	else if (extension == ".css")
		_rspType = "text/css";
	else if (extension == ".py" || extension == ".php") // TODO check if valid
		_rspType = "cgi-script";
	else
		_rspType = "application/octet-stream";
}

void HttpRequest::checkHost(std::map<std::string, std::vector<std::string> >::const_iterator it, ServerData & serv)
{
	if (it == this->headers->getHeaderEnd())
		throw std::invalid_argument(E_400);

	if (this->headers->getHeaderOnlyOneValue("host", 0).empty())
		throw std::invalid_argument(E_400);

	this->host = HttpParser::parseHost(this->headers->getHeaderOnlyOneValue("host", 0));

	if ( !HttpParser::checkIfHostNameExistInServer( this->host.first, serv.getServerName()))
		throw std::invalid_argument(E_400);
}

std::string HttpRequest::getHttpMethod() const
{
	return this->req_line->getMethod();
}

std::string HttpRequest::getRequestUri() const
{
	return this->uri->getUri();
}

std::string HttpRequest::getPath() const
{
	return this->uri->getPath();
}

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

std::string HttpRequest::getQuery() const
{
	return this->uri->getQuery();
}

std::string HttpRequest::getHttpVersion() const
{
	return this->req_line->getVersion();
}

int HttpRequest::getStatusCode() const
{
	return this->code;
}

bool HttpRequest::getAutoindex() const
{
	return this->_autoindex;
}

int HttpRequest::getParsingState() const
{
	return this->state;
}

std::string HttpRequest::getRspType() const
{
	return _rspType;
}


ServerManager &HttpRequest::getServ() const
{
	return this->server;
}

void HttpRequest::fileUpload() // TODO check if maybe we should put it in response
{
	for (std::vector<MultiBody>::const_iterator it = _bodies.begin(); it != _bodies.end(); it++)
	{
		if (it->bodyHeader.getHeaderSize() > 1)
		{
			std::cout << "IS FILE" << std::endl; // DBG
			std::string	  locationUp = _fullPath.first + _fullPath.second;
			std::string	  content	 = it->bodyHeader.getHeader("content-disposition")->second[0];
			size_t		  startName	 = content.find("filename=") + 10;
			size_t		  endName	 = content.find("\"", startName);
			std::string	  fileName	 = locationUp + content.substr(startName, endName - startName);
			std::ofstream fileUp(fileName.c_str());
			if (fileUp.is_open())
			{
				fileUp << it->bodyContent;
				fileUp.close();
			}
		}
		else
			std::cout << "IS FORM" << std::endl; // DBG
	}
}

Headers	*HttpRequest::getReqHeaders() const
{
	return this->headers;
}

std::pair<int, std::string> HttpRequest::getAddrPort() const
{
	return incoming;
}

std::pair<std::string, std::string>  HttpRequest::getHost() const
{
	return host;
}

/* TODO
	fileUpload
	{
		for every body in request
		get file name and extension
		consider different cases for 
		form and file
		use getHeaderOnlyOneValue
		try and access location (it should already be checked in request)
		create file with name and copy bits
	} */
