/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tron <cle-tron@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:03 by cle-tron          #+#    #+#             */
/*   Updated: 2025/06/22 14:28:58 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HTTPREQUEST_HPP
#define	HTTPREQUEST_HPP


#include "RequestLine.hpp"
#include <string>

class	RequestLine;

class	HttpRequest	{
private:
	
	RequestLine	*	req_line;
//	RequestHeaders	headers;
//	RequestBody		body;	

public:
	HttpRequest();
	HttpRequest( std::string const & message );
	HttpRequest( HttpRequest const & src );
	~HttpRequest();

	HttpRequest &	operator=( HttpRequest const & rhs );	
	
	std::string	getHttpMethod() const;
	std::string	getRequestUri() const;
	std::string	getHttpVersion() const;
};

#endif
