/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:29:43 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/09/02 11:39:59 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "HeadRsp.hpp"
#include "Utils.hpp"

/* TODO maybe this class can inherit
from HttpRequest? */

class HttpRequest;

class Response
{
private:
	const HttpRequest *	_request;
	bool				_autoindex;
	int				   	_clientFd;
	std::string		   	_response;
	std::string		   	_location;
	std::string		   	_method;
	std::ostringstream 	_output;
	std::string		   	_contentType;
	std::string		   	_contentLength;

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
	std::string getResponse();
	void		printRawResponse();
	bool		getAutoindex() const;
	std::string doAutoindex(std::string str, DIR *dir);
	void		doHtmlAutoindex(std::string &uri, std::ostringstream &html);
};

#endif
