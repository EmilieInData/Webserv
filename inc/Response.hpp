/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:29:43 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/31 11:49:49 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Utils.hpp"

class Response
{
	private:
	int					_clientFd;
	std::string			_response;
	std::string			_location;
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
	void		setContent(std::pair<std::string, std::string> fullPath);
	void		prepResponse();
	void		sendResponse();
	void		printRawResponse();
};

#endif