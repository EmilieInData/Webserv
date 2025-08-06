/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:29:43 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/06 10:56:57 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Utils.hpp"
#include "Header.hpp"

/* TODO maybe this class can inherit
from HttpRequest? */

class Response
{
private:
	int					_clientFd;
	std::string			_response;
	std::string			_location;
	std::string			_method;
	std::ostringstream	_output;
	std::string			_contentLength;

	Response &operator=(Response const &copy);

public:
	Response();
	Response(Response const &copy);
	~Response();

	std::string prepFile();
	std::string checkType();
	void		setClientFd(int _clientFd);
	void		setResponse(std::string response);
	void		setContent(std::pair<std::string, std::string> fullPath,
						   std::string						   method);
	void		prepResponse();
	void		sendResponse();
	std::string getResponse();
	void		printRawResponse();
};

#endif
