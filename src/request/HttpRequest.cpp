/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:08 by cle-tron          #+#    #+#             */
/*   Updated: 2025/09/10 15:48:16 by fdi-cecc         ###   ########.fr       */
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

void HttpRequest::sendBuffer(char *buffer, ssize_t bytes)
{

	for (int i = 0; i < bytes; i++)
		this->fullRequest.push_back(buffer[i]);

	std::size_t found = fullRequest.find(CRLF);
	//	std::cout << "FULLREQUEST: " << fullRequest << std::endl;

	// static int i = 0;

	try
	{
		while (found != std::string::npos && this->state != BODY)
		{
			std::string tmp = this->fullRequest.substr(0, found);
			this->fullRequest.erase(0, found + 2);

			// std::cout << "TMP " << i++ << ": " << tmp << std::endl;
			if (!HttpParser::isAsciiPrintable(tmp))
				throw std::invalid_argument(E_400);

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
			if (!this->boundary.empty() && this->getRspType() != "cgi-script")
			{
				manyBodiesRoutine(found);
			}
			// For simple bodies OR for ANY request going to a CGI script,
			// treat the entire body as a single raw string.
			else
			{
				// Append the received data to the body and clear the temporary buffer.
				// This is safer than 'this->body = this->fullRequest' if data arrives in chunks.
				this->body.append(this->fullRequest);
				this->fullRequest.clear();

				// Check if the full body has been received according to Content-Length.
				if (this->body.length() >= this->body_len)
				{
					if (this->body.length() > this->body_len)
						this->body.erase(this->body_len);

					this->state = DONE;
				}
			}
		}
	}
	catch (std::invalid_argument e)
	{
		this->setStatusCode(e.what());
	}
	/*	if (this->state == DONE)
	{
		if (getRspType() == "cgi-script")
			server.getScript().runScript(*this, _cgiInterpreterPath);
		else
			fileUpload();
	}*/
	// FABIO end of request parsing here

	//	std::cout << "STATE IN FCT: " << this->state << std::endl;

	//	this->state = DONE; //solo poner en caso de debug para que no se quede colgado
}

void HttpRequest::printBodies() // DBG
{
	std::cout << "Bodies size: " << _bodies.size() << std::endl;

	for (std::vector<MultiBody>::const_iterator it = _bodies.begin(); it != _bodies.end(); ++it)
	{
		std::cout << GREEN << "Body content" << std::endl;
		it->bodyHeader.printHeader();

		std::map<std::string, std::vector<std::string> >::const_iterator cd_it = it->bodyHeader.getHeader(
			"content-disposition");

		if (cd_it != it->bodyHeader.getHeaderEnd())
		{
			const std::string &contDisp		= cd_it->second[0];
			size_t			   filename_pos = contDisp.find("filename=");

			if (filename_pos != std::string::npos)
			{
				// Extract filename
				size_t start_quote = contDisp.find("\"", filename_pos);
				size_t end_quote   = contDisp.find("\"", start_quote + 1);

				if (start_quote != std::string::npos && end_quote != std::string::npos)
				{
					std::string filename = contDisp.substr(start_quote + 1,
														   end_quote - (start_quote + 1));
					std::cout << "[FILE UPLOAD] Filename: " << filename
							  << " (Size: " << it->bodyContent.size() << " bytes)" << std::endl;
				}
				else
				{
					std::cout << "[FILE UPLOAD] Binary content (Size: " << it->bodyContent.size()
							  << " bytes)" << std::endl;
				}
			}
			else
				std::cout << it->bodyContent << std::endl;
		}
		else
			std::cout << it->bodyContent << std::endl;

		std::cout << RESET << std::endl;
	}
}

LocationConf const *HttpParser::findLocation(std::string								path,
											 std::map<std::string, LocationConf> const &loc)
{
	std::map<std::string, LocationConf>::const_iterator it;
	std::map<std::string, LocationConf>::const_iterator tmp = loc.end();

	for (it = loc.begin(); it != loc.end(); it++)
	{
		if (path.find(it->first) == 0)
		{
			if (tmp == loc.end() || tmp->first.length() < it->first.length())
				tmp = it;
		}
	}
	if (tmp == loc.end())
		return NULL;
	return &tmp->second;
}

void HttpRequest::finalHeadersParsingRoutine()
{
	if (this->headers->getHeader("cookie") != this->headers->getHeaderEnd())
		this->headers->setManyValuesHeader("cookie");

	if (this->headers->getHeader("content-type") != this->headers->getHeaderEnd())
		this->headers->setManyValuesHeader("content-type");

	ServerData serv = HttpParser::checkIfServerExist(this->server.getServersList(), this->incoming);

	checkHost(this->headers->getHeader("host"), serv);

	this->uri = new Uri(req_line->getReqTarget(), this->host.first);

	setFullPath(serv);

	this->max_body_size = serv.getBodySize();

	blockLoc = findLocation(this->_fullPath.second, serv.getLocations());

	if (blockLoc.getKey().empty())
		throw std::invalid_argument(E_404);

	HttpParser::checkIfPathExist(this->_fullPath, blockLoc, this->getHttpMethod());

	if (this->headers->getHeader("content-type") != this->headers->getHeaderEnd())
	{
		this->boundary = HttpParser::parseContentTypeBoundary(
			this->headers->getHeaderValue("content-type"));
		if (!this->boundary.empty())
		{
			this->state			= BODY;
			this->body_state	= BOUNDARY;
			this->boundary_flag = true;
		}
	}
	if (this->headers->getHeader("content-length") != this->headers->getHeaderEnd())
	{
		this->body_len = HttpParser::parseContentLengthHeader(this->headers->getHeaderOnlyOneValue("content-length",
																								   0),
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
			this->body_state = HEADERS2;
			break;
		case HEADERS2:
			if (tmp.empty())
			{
				this->body_state = BODY2;
				break;
			}
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
		this->_bodies.push_back(newBody);
		makeNew		= true;
		this->state = DONE;
	}
	std::size_t f2 = this->body.find("\r\n--" + this->boundary + "\r\n"); //OTRO BODY
	if (f2 != std::string::npos)
	{
		this->body = this->body.erase(f2, this->body.length()); //FABIO poner body  en la struct
		newBody.bodyContent = this->body;
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

void HttpRequest::setLocation(std::map<std::string, LocationConf> const &location,
							  std::string const							&path)
{
	std::size_t found = path.rfind("/");
	std::cout << PINK << "request path: " << path << std::endl << RESET; //TO BORROW

	//	if ( found == std::string::npos ) //BADREQUEST

	this->location = path.substr(0, found + 1);
	std::cout << PINK << "LOCATION REQ: " << this->location << std::endl;
	// EMILIE this not ok because for ./error_pages/ esta buscando / solo
	std::map<std::string, LocationConf>::const_iterator it = location.find(this->location);

	//	for ( std::map<std::string, LocationConf>::iterator itt = location.begin(); itt != location.end(); ++itt)
	//		std::cout << "LOCATION CONF: " << itt->first << std::endl;
	setRspType(); // FABIO added here, seems the best place for now
	if (_rspType == "cgi-script")
	{
		// 1. Get the file extension from the full path
		std::string requestFile = _fullPath.second;
		size_t		dotPos		= requestFile.find_last_of('.');
		if (dotPos != std::string::npos)
		{
			std::string extension = requestFile.substr(dotPos);

			const std::map<std::string, std::string>		  &cgiPassMap = it->second.getCgiPass();
			std::map<std::string, std::string>::const_iterator cgi_it = cgiPassMap.find(extension);

			if (cgi_it != cgiPassMap.end())
			{
				this->_cgiInterpreterPath = cgi_it->second;
				std::cout << GREEN << "Found CGI interpreter for " << extension << ": "
						  << _cgiInterpreterPath << RESET << std::endl;
			}
			else
				throw std::invalid_argument(E_404); // TODO enter right error code
		}
	}
	std::cout << RED << "[SOMEWHAT HERE] > " << __func__ << RESET << std::endl; // DBG
	if (it == location.end())
		throw std::invalid_argument(E_404);
	// _autoindex = it->second.getAutoindex(); //ADD by EMILIE
	//	std::cout << "LOCATION EXIST IN SERVER: " << (*it).first << std::endl;
}

void HttpRequest::setRspType()
{
	std::string location = _fullPath.first + _fullPath.second;

	if (isFolder(location)) // && _autoindex)
	{
		_rspType = "text/html";
		return;
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

void HttpRequest::checkHost(std::map<std::string, std::vector<std::string> >::const_iterator it,
							ServerData														&serv)
{
	if (it == this->headers->getHeaderEnd())
		throw std::invalid_argument(E_400);

	if (this->headers->getHeaderOnlyOneValue("host", 0).empty())
		throw std::invalid_argument(E_400);

	this->host = HttpParser::parseHost(this->headers->getHeaderOnlyOneValue("host", 0));

	if (!HttpParser::checkIfHostNameExistInServer(this->host.first, serv.getServerName()))
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
	//	std::cout << RED << __func__ << " > " << this->code << RESET << std::endl;
	return this->code;
}

// bool HttpRequest::getAutoindex() const
// {
// 	return this->_autoindex;
// }

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

void HttpRequest::fileUpload()
{
	std::map<std::string, std::string> formData;

	for (std::vector<MultiBody>::const_iterator it = _bodies.begin(); it != _bodies.end(); ++it)
	{
		std::map<std::string, std::vector<std::string> >::const_iterator cd_it = it->bodyHeader.getHeader(
			"content-disposition"); // checks if file
		if (cd_it == it->bodyHeader.getHeaderEnd())
		{
			continue;
		}

		const std::string &contDisp = cd_it->second[0];

		size_t filename_pos = contDisp.find("filename=");

		if (filename_pos != std::string::npos)
		{
			// FABIO here if upload is file
			size_t start_quote = contDisp.find("\"", filename_pos);
			size_t end_quote   = contDisp.find("\"", start_quote + 1);
			if (start_quote == std::string::npos || end_quote == std::string::npos)
				continue;

			std::string nameOriginal = contDisp.substr(start_quote + 1,
													   end_quote - (start_quote + 1));
			size_t		lastSlash	 = nameOriginal.find_last_of("/");
			std::string nameClean	 = (lastSlash == std::string::npos)
										   ? nameOriginal
										   : nameOriginal.substr(lastSlash + 1);

			if (nameClean.empty())
				continue;

			std::string uploadPath = _fullPath.first + _fullPath.second + nameClean;

			// Save the file
			std::ofstream fileUp(uploadPath.c_str(), std::ios::binary);
			if (fileUp.is_open())
			{
				fileUp << it->bodyContent;
				fileUp.close();
				std::cout << GREEN << "SUCCESS: Saved file to " << uploadPath << RESET << std::endl;
			}
			else
			{
				std::cerr << "ERROR: Could not open file for writing: " << uploadPath << std::endl;
				//	TODO send error status
			}
		}
		else
		{
			//FABIO here if it's form
			size_t name_pos = contDisp.find("name=");
			if (name_pos != std::string::npos)
			{
				size_t start_quote = contDisp.find("\"", name_pos);
				size_t end_quote   = contDisp.find("\"", start_quote + 1);
				if (start_quote != std::string::npos && end_quote != std::string::npos)
				{
					std::string field_name = contDisp.substr(start_quote + 1,
															 end_quote - (start_quote + 1));
					formData[field_name]   = it->bodyContent;
				}
			}
		}
	}

	std::cout << RED << "[Form Data Received]" << RESET << std::endl;
	for (std::map<std::string, std::string>::const_iterator map_it = formData.begin();
		 map_it != formData.end(); ++map_it)
	{
		std::cout << map_it->first << ": " << map_it->second << std::endl;
	}

	/* 	TODO Generate a Response: After processing, set a success status code.
	The Response class will then use this to build the final reply.
	For a successful creation of a resource, 201 Created is appropriate. */
	this->code = 201; // Or 200 OK
}

Headers *HttpRequest::getReqHeaders() const
{
	return this->headers;
}

std::pair<int, std::string> HttpRequest::getAddrPort() const
{
	return incoming;
}

std::pair<std::string, std::string> HttpRequest::getHost() const
{
	return host;
}

std::string HttpRequest::getRawBody() const
{
	return this->body;
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

LocationConf HttpRequest::findLocation(std::string								  path,
									   std::map<std::string, LocationConf> const &loc)
{
	std::map<std::string, LocationConf>::const_iterator it;
	std::map<std::string, LocationConf>::const_iterator tmp = loc.end();

	for (it = loc.begin(); it != loc.end(); it++)
	{
		if (path.find(it->first) == 0)
		{
			if (tmp == loc.end() || tmp->first.length() < it->first.length())
				tmp = it;
		}
	}
	if (tmp == loc.end())
		return LocationConf();
	return tmp->second;
}

LocationConf HttpRequest::getBlockLoc() const
{
	return blockLoc;
}
