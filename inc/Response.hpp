/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:29:43 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/05 11:22:32 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Utils.hpp"

/* TODO right now the response is very basic
and only provides with responses to GET giving
back 200 */

class Response
{
	private:
	int					_clientFd;
	std::string			_response;
	std::string			_location;
	std::string			_method;
	std::ostringstream	_output;
	std::string			_contentLength;
	
	Response(Response const &copy);
	Response &operator=(Response const &copy);
	
	public:
	Response();
	~Response();

	std::string	prepFile();
	std::string	checkType();
	void		setClientFd(int _clientFd);
	void		setResponse(std::string response);
	void		setContent(std::pair<std::string, std::string> fullPath, std::string method);
	void		prepResponse();
	void		sendResponse();
	std::string	getResponse();
	void		printRawResponse();
};

#endif
