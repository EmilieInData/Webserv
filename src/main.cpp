/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 14:56:03 by cle-tron          #+#    #+#             */
/*   Updated: 2025/07/01 16:37:36 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "HttpParserTester.hpp"
#include "server/Server.hpp"

#include <iostream>

int	main() {


	Server testserv;
	
	HttpRequest test = HttpRequest();
	
	std::string	http_mess( "POST /form?querysinparseo HTTP/1.1\r\nHost: www.ejemplo.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nnombre=juan&apel\rido=perez" );

	HttpRequest	req = HttpRequest( http_mess );

	std::cout << "method: " << req.getHttpMethod() <<  std::endl;
	std::cout << "uri: " << req.getRequestUri() << std::endl;
	std::cout << "path: " << req.getPath() << std::endl;
	std::cout << "query: " << req.getQuery() << std::endl;
	std::cout << "version: " << req.getHttpVersion() << std::endl;

	testserv.servSetup();
	testserv.servRun();
	return 0;
}
