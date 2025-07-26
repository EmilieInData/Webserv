/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:29:43 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/26 12:35:18 by fdi-cecc         ###   ########.fr       */
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
	std::string			_content;
	std::ostringstream	_output;
	std::string			_contentLength;
	
	Response(Response const &copy);
	Response &operator=(Response const &copy);
	
	public:
	Response();
	~Response();

	void		setClientFd(int _clientFd);
	void		setResponse(std::string response);
	void		setContent(std::string content);
	void		prepResponse();
	void		sendResponse();
};

#endif