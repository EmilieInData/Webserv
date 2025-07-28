/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:03:03 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/28 10:57:10 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HTTPREQUEST_HPP
#define	HTTPREQUEST_HPP

#include <string>

class	RequestLine;
class	Uri;
class	ServerManager;

class	HttpRequest	{
private:	
	RequestLine	*						req_line;
	Uri *								uri;
	std::pair<std::string, std::string>	host;
//	RequestHeaders	headers;
//	RequestBody		body;	

public:
	HttpRequest();
	HttpRequest( std::string const & message, ServerManager & server );
	HttpRequest( HttpRequest const & src );
	~HttpRequest();

	HttpRequest &	operator=( HttpRequest const & rhs );	
	
	std::string	getHttpMethod() const;
	std::string	getRequestUri() const;
	std::string	getPath() const;
	std::string	getQuery() const;
	std::string	getHttpVersion() const;
};

#endif
