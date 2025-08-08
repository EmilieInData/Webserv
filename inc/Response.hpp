/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:29:43 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/08 11:32:54 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Header.hpp"
#include "Utils.hpp"

#define PIPE_READ 0
#define PIPE_WRITE 1

/* TODO maybe this class can inherit
from HttpRequest? */

class HttpRequest;

class Response
{
private:
	const HttpRequest *_request;
	int				   _clientFd;
	std::string		   _response;
	std::string		   _location;
	std::string		   _method;
	std::ostringstream _output;
	std::string		   _contentType;
	std::string		   _contentLength;

	Response &operator=(Response const &copy);
	Response();

public:
	Response(HttpRequest const &request);
	Response(Response const &copy);
	~Response();

	std::string prepFile();
	std::string checkType();
	void		setClientFd(int _clientFd);
	void		setResponse(std::string response);
	void		setContent(std::pair<std::string, std::string> fullPath, std::string method);
	void		prepResponse();
	void		sendResponse();
	std::string getType();
	std::string getLength();
	void		runScript(std::string const &cgiPath);
	std::string getResponse();
	void		printRawResponse();
};

#endif
